#ifdef IGNORE

mapscreen.c:
  This is a module that contains all the functions neccesary to map a text
  screen into the users process address space.  The main function is the
  MapWPath routine which accepts the path number of a window and a pointer
  to a structure to return information about the window in.  This function
  maps in the specified window and returns a pointer to the beginning of
  the window.

#endif IGNORE

#include "screen.h"
#define NULL 0

#define NumDTEs  32    /* Number of device table entries */
#define DrvNamSz 8
#define DscNamSz 4

#define P_DATImg 64
#define V_Type   6
#define M_Name   4
#define D_DevTbl 0x80
#define PD_TBL   0x3b

#define MAX(a,b) ((a>b) ? (a) : (b))
#define BYTE(a) (a & 0xff)

typedef struct {
  char *V_DRIV;
  char *V_STAT;
  char *V_DESC;
  char *V_FMGR;
  char  V_USRS;
} DTE_t;

static char *SysDATp;
static int linesize[] = {0,80,160,0,0,640,640,1280,1280};

/* Set errno according to last OS9 system call error. */

static SetD()
{
#asm
 bcs _SetD1
 clra
 clrb
 bra _SetD2
_SetD1:
 clra
 std errno,y
 ldd #-1
_SetD2:
#endasm
}

GPrDsc(procid, buffer)
int procid;
char *buffer;
{
#asm
 lda 5,s
 ldx 6,s
 os9 $18
 bsr SetD
#endasm
}

GetOpt(path,buffer)
int path;
char *buffer;
{
#asm
 lda 5,s
 ldx 6,s
 clrb
 os9 $8d
 bsr SetD
#endasm
}

CpyMem(dest,source,srcdat,numbytes)
char *dest,*source,*srcdat;
int numbytes;
{
#asm
 pshs y
 ldd 10,s
 ldx 8,s
 ldy 12,s
 ldu 6,s
 os9 $1b
 puls y
 bsr SetD
#endasm 
}

CpySys(dest,source,numbytes)
char *dest,*source;
int numbytes;
{
  CpyMem(dest,source,SysDATp,numbytes);
}

char *GetDTEp(path)
int path;
{
  char PDOptBuf[32];

  GetOpt(path,PDOptBuf);
  return *(char **)(PDOptBuf+PD_TBL-0x20);
}

GetSDATp()
{
  static char procbuf[512];

  GPrDsc(1,procbuf);
  SysDATp = procbuf+P_DATImg;
}

char *MapBlk(block,count)
int block;
int count;
{
#asm
 ldx 4,s
 ldb 7,s
 os9 $4f
 bcc _MapBlk1
 clra
 std errno,y
 ldd #0
 bra _MapBlk2
_MapBlk1:
 tfr u,d
_MapBlk2:
#endasm
}

ClrBlk(blockptr)
char *blockptr;
{
#asm
 ldb #1
 ldu 4,s
 os9 $50
 lbsr SetD
#endasm
}

static ClrMem(mapdat)
MapDat_t *mapdat;
{
  ClrBlk(mapdat->addr,mapdat->count);
  mapdat->addr = (char *)0;
  mapdat->count = 0;
}

/* Allocate memory as specified by the MapDat_t structure.  MapMem must */
/* be passed a mapdat structure with the block,offset,and size specified. */
/* MapMem will allocate enough blocks to allow access to <size> bytes at */
/* offset <offset> in block <blknum>.  The ClrMem function can later be */
/* called to unmap the blocks. */

static char *MapMem(mapdat)
MapDat_t *mapdat;
{
  int blknum = mapdat->block;
  int offset = mapdat->offset;
  int size = mapdat->size;
  int blkcount = (unsigned)(offset+size+8191)>>13;
  char *memp = MapBlk(blknum,blkcount);
  if (!memp)
    return (char *)0;
  if ((unsigned)(memp+offset+size)>(unsigned)0xfe00) {
    /* out memory area overlaps the top 512 bytes, we must map in a dummy
       block to take up the top 8k, then map our memory again and unmap
       the dummy block */
    char *dummyblk;
    ClrBlk(memp,blkcount);
    dummyblk = MapBlk(blknum,1);
    memp = MapBlk(blknum,2);
    ClrBlk(dummyblk,1);
    if (!memp)
      return (char *)0;
  }
  mapdat->addr = memp;
  mapdat->count = blkcount;
  return memp+offset;
}

char *MapWPath(path,WInfo)
int path;
WInfo_t *WInfo;
{
  char *DTEp;
  DTE_t DTE;
  int  Offset;
  char *Blockp;
  char *NewBlock;
  char WEntNum;
  char WinEnt[64];
  int  ScrnType;
  char ScrnBlk;
  char WinType;
  char byte;

  GetSDATp();
  DTEp = GetDTEp(path);
  CpySys(&DTE,DTEp,sizeof(DTE));
  CpySys(&WinType,DTE.V_STAT+V_Type,sizeof(WinType));
  if (WinType & 0x80 == 0)        /* VDG Screen */
    return NULL;
  CpySys(&WEntNum,DTE.V_STAT+0x35,sizeof(WEntNum));
  CpySys(WinEnt,0x1280+WEntNum*sizeof(WinEnt),sizeof(WinEnt));
  if (BYTE(WinEnt[0]) == 0xff)
    return NULL;
  CpySys(&byte,*(char **)WinEnt,sizeof(byte));
  ScrnType = BYTE(byte);
  CpySys(&ScrnBlk,*(char **)WinEnt+1,sizeof(ScrnBlk));
  WInfo->W_Type = (ScrnType <= 0x80) ? ScrnType+4 : 0x87-ScrnType;
  Offset = *(int *)(WinEnt+0x34) - 0x8000;
  WInfo->W_SzX = BYTE(WinEnt[7]);
  WInfo->W_SzY = BYTE(WinEnt[8]);
  if (WInfo->W_Type<0 || WInfo->W_Type>8)
    return (char *)0;
  WInfo->W_MapDat.block = ScrnBlk;
  WInfo->W_MapDat.size = linesize[WInfo->W_Type]*WInfo->W_SzY;
  WInfo->W_MapDat.offset = Offset;
  return MapMem(&WInfo->W_MapDat);
}
