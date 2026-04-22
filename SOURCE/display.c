static int window;

#define O_RDWR 3
#define WINTYPE 6

static char outbuf[256];
static char *outbufp = outbuf;

static void GetOpt(int path,char *opt)
{
#asm
 lda 5,s
 ldb #0
 ldx 6,s
 os9 $8d
#endasm
}

static void SetOpt(int path,char *opt)
{
#asm
 lda 5,s
 ldb #0
 ldx 6,s
 os9 $8e
#endasm
}

static void SetEcho(int path,int value)
{
  char opt[32];
  GetOpt(path,opt);
  opt[4] = value;
  SetOpt(path,opt);
}

void InitDisplay(void)
{
  window = open("/w",O_RDWR);
  { /* Create window */
    static char dwset[] = { 0x1b, 0x20, WINTYPE, 0, 0, 40, 24, 0, 2, 2 };
    write(window,dwset,sizeof dwset);
  }
  { /* select window */
    static char dwset[] = { 0x1b, 0x21 };
    write(window,dwset,sizeof dwset);
  }
  { /* turn off cursor */
    static char cursoff[] = { 0x5, 0x20 };
    write(window,cursoff,sizeof cursoff);
  }
  SetEcho(window,0);
}

int WinPath(void)
{
  return window;
}

void FlushWin(void)
{
  write(window,outbuf,outbufp-outbuf);
  outbufp = outbuf;
}

static void Write(char *p,int n)
{
  if (outbufp+n>outbuf+sizeof(outbuf))
    FlushWin();
  while (n>0) {
    *outbufp++ = *p++;
    --n;
  }
}

#ifdef IGNORE
void DrawLine(int x1,int y1,int x2,int y2)
{
  static char setdptr[] = { 0x1b, 0x40, 0, 0, 0, 0 };
  static char line[] = { 0x1b, 0x44, 0, 0, 0, 0 };
  if (x1<0)
    x1 = 0;
  if (x2<0)
    x2 = 0;
  if (x1>=639)
    x1 = 638;
  if (x2>=639)
    x2 = 638;
  if (y1<0)
    y1 = 0;
  if (y2<0)
    y2 = 0;
  if (y1>=192)
    y1 = 191;
  if (y2>=192)
    y2 = 191;
  *(int *)(setdptr+2) = x1;
  *(int *)(setdptr+4) = y1;
  *(int *)(line+2) = x2;
  *(int *)(line+4) = y2;
  Write(setdptr,sizeof setdptr);
  Write(line,sizeof line);
}
#endif /* IGNORE */

int ReadKey(void)
{
  char c;
  read(window,&c,1);
  return c;
}

void ClearScreen(void)
{
  char c = 0xc;
  Write(&c,1);
}
