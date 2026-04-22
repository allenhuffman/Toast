#include "display.h"
#include "screen.h"

static char *screenp;
static char pattern[32*32];

static char *DrawBML(char **sp,char **p,int x,int dx,int *r)
{
#asm
 ldu [6,s]
 lda 11,s
 ldb 9,s
 ldx 12,s
 pshs y
 ldy [6,s]
 pshs d
 lda 1,x
_dbml5
 ldx #0
 suba #32
 bcs _dbml1b
 suba #32
 bcs _dbml1d
 suba #32
 bcs _dbml1e
 suba #32
 bcs _dbml1f
 ldb ,u
 stb ,y+
 dec 1,s
 bne _dbml5
 bra _dbml6
_dbml1f
 adda ,s
 ldb ,u+
 stb ,y+
 dec 1,s
 bne _dbml5
 bra _dbml6
_dbml1e
 adda ,s
 ldb ,u+
 andb #$fc
 abx
 bra _dbml3c
_dbml1d
 adda ,s
 ldb ,u+
 andb #$f0
 abx
 bra _dbml2c
_dbml1b
 adda ,s
 ldb ,u+
 andb #$c0
 abx
 suba #32
 bcs _dbml2b
 suba #32
 bcs _dbml2d
 suba #32
 bcs _dbml2e
 ldb ,u
 andb #$3f
 abx
 bra _dbml4c
_dbml2e
 adda ,s
 ldb ,u+
 andb #$3f
 abx
 bra _dbml4c
_dbml2d
 adda ,s
 ldb ,u+
 andb #$3c
 abx
 bra _dbml3c
_dbml2b
 adda ,s
 ldb ,u+
_dbml2
 andb #$30
 abx
_dbml2c
 suba #32
 bcs _dbml3b
 suba #32
 bcs _dbml3d
 ldb ,u
 andb #$0f
 abx
 bra _dbml4c
_dbml3d
 adda ,s
 ldb ,u+
 andb #$0f
 abx
 bra _dbml4c
_dbml3b
 adda ,s
 ldb ,u+
_dbml3
 andb #$0c
 abx
_dbml3c
 suba #32
 bcs _dbml4b
 ldb ,u
 bra _dbml4
_dbml4b
 adda ,s
 ldb ,u+
_dbml4
 andb #$03
 abx
_dbml4c
 pshs a
 tfr x,d
 stb ,y+
 puls a
 dec 1,s
 bne _dbml5
_dbml6
 leas 2,s
 sty [6,s]
 puls y
 stu [6,s]
 ldx 12,s
 sta 1,x
#endasm
}

static DrawIt(char *posp,int dx)
{
  char *cp = pattern;
  int y = 32;
  while (y>0) {
    char *sp = posp;
    int x = dx>>2;
    int r = dx-16-1;
    if (x>0)
      DrawBML(&sp,&cp,x,dx,&r);
    x = dx & 3;
    while (x>0) {
      int t = *cp & 0xc0;
      if ((r-=32) < 0) {
        r += dx;
        ++cp;
      }
      if (--x == 0) {
        *sp = (*sp & 0x3f) | t;
        break;
      }
      t |= *cp & 0x30;
      if ((r-=32) < 0) {
        r += dx;
        ++cp;
      }
      if (--x == 0) {
        *sp = (*sp & 0xf) | t;
        break;
      }
      t |= *cp & 0x0c;
      if ((r-=32) < 0) {
        r += dx;
        ++cp;
      }
      *sp = (*sp & 0x3) | t;
      break;
    }
    posp += 80;
    --y;
  }
}

int main(void)
{
  {
    char *cp = pattern;
    int y = 0;
    while (y<32) {
      int x=0;
      while (x<32) {
        int c = (x+y) & 3;
        *cp = c | (c<<2) | (c<<4) | (c<<6);
        ++cp;
        ++x;
      }
      ++y;
    }
  }
  InitDisplay();
  FlushWin();
  {
    WInfo_t winfo;
    screenp = MapWPath(WinPath(),&winfo);
    if (!screenp) {
      printf("Cannot map screen\n");
      exit(0);
    }
    printf("window type = %d\n",winfo.W_Type);
    printf("window szx  = %d\n",winfo.W_SzX);
    printf("window szy  = %d\n",winfo.W_SzY);
    printf("block   = %d\n",winfo.W_MapDat.block);
    printf("size    = %d\n",winfo.W_MapDat.size);
    printf("offset  = %d\n",winfo.W_MapDat.offset);
    printf("count   = %d\n",winfo.W_MapDat.count);
    printf("addr    = %x\n",winfo.W_MapDat.addr);
    printf("screenp = %x\n",screenp);
  }
  {
    int rep = 4;
    while (rep>0) {
      int dx = 32;
      while (dx<256) {
        DrawIt(screenp,dx);
        ++dx;
      }
      while (dx>32) {
        --dx;
        DrawIt(screenp,dx);
      }
      --rep;
    }
  }
  ReadKey();
  return 0;
}
