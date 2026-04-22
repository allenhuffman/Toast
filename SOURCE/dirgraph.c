DVLine(p,x,y1,y2)
char *p;
int x,y1,y2;
{
#asm
 lda #80
 ldb 9,s
 mul
 addd 4,s
 tfr d,x
 ldd 6,s
 lsra
 rorb
 bcs _dvline5
 lsrb
 bcs _dvline6
 leax d,x
 ldb 11,s
 subb 9,s
 incb
_dvline1
 pshs b
 andb #3
 beq _dvline1b
_dvline1a
 lda ,x
 anda #$3f
 sta ,x
 leax 80,x
 decb
 bne _dvline1a
_dvline1b
 puls b
 lsrb
 lsrb
 beq _dvlinee
_dvline1c
 lda ,x
 anda #$3f
 sta ,x
 lda 80,x
 anda #$3f
 sta 80,x
 leax 160,x
 lda ,x
 anda #$3f
 sta ,x
 lda 80,x
 anda #$3f
 sta 80,x
 leax 160,x
 decb
 bne _dvline1c
 bra _dvlinee
_dvline5
 lsrb
 bcs _dvline7
 leax d,x
 ldb 11,s
 subb 9,s
 incb
_dvline2
 pshs b
 andb #3
 beq _dvline2b
_dvline2a
 lda ,x
 anda #$cf
 sta ,x
 leax 80,x
 decb
 bne _dvline2a
_dvline2b
 puls b
 lsrb
 lsrb
 beq _dvlinee
_dvline2c
 lda ,x
 anda #$cf
 sta ,x
 lda 80,x
 anda #$cf
 sta 80,x
 leax 160,x
 lda ,x
 anda #$cf
 sta ,x
 lda 80,x
 anda #$cf
 sta 80,x
 leax 160,x
 decb
 bne _dvline2c
 bra _dvlinee
_dvline6
 leax d,x
 ldb 11,s
 subb 9,s
 incb
_dvline3
 pshs b
 andb #3
 beq _dvline3b
_dvline3a
 lda ,x
 anda #$f3
 sta ,x
 leax 80,x
 decb
 bne _dvline3a
_dvline3b
 puls b
 lsrb
 lsrb
 beq _dvlinee
_dvline3c
 lda ,x
 anda #$f3
 sta ,x
 lda 80,x
 anda #$f3
 sta 80,x
 leax 160,x
 lda ,x
 anda #$f3
 sta ,x
 lda 80,x
 anda #$f3
 sta 80,x
 leax 160,x
 decb
 bne _dvline3c
 bra _dvlinee
_dvline7
 leax d,x
 ldb 11,s
 subb 9,s
 incb
_dvline4
 pshs b
 andb #3
 beq _dvline4b
_dvline4a
 lda ,x
 anda #$fc
 sta ,x
 leax 80,x
 decb
 bne _dvline4a
_dvline4b
 puls b
 lsrb
 lsrb
 beq _dvlinee
_dvline4c
 lda ,x
 anda #$fc
 sta ,x
 lda 80,x
 anda #$fc
 sta 80,x
 leax 160,x
 lda ,x
 anda #$fc
 sta ,x
 lda 80,x
 anda #$fc
 sta 80,x
 leax 160,x
 decb
 bne _dvline4c
_dvlinee
#endasm
}

/* Draw a NNE line */
/* p = pointer to beginning of screen *
/* x,y = location of first pixel */
/* dx,dy = offset to last pixel */
DLine1(p,x,y,dx,dy)
char *p;
int x,y,dx,dy;
{
#asm
 ldx 12,s
 lda 9,s
 ldb #80
 mul
 addd 4,s
 tfr d,u
 ldd 6,s
 lsra
 rorb
 lsrb
 leau b,u
 pshs x
 leax 1,x
 ldb 13,s
 stb ,s
 ldd 8,s
 lsra
 rorb
 bcs _dline18
 lsrb
 bcs _dline17
 ldb 1,s
 lsrb
 bra _dline111
_dline17
 ldb 1,s
 lsrb
 bra _dline113
_dline18
 lsrb
 bcs _dline19
 ldb 1,s
 lsrb
 bra _dline112
_dline19
 ldb 1,s
 lsrb
 bra _dline114
_dline11
 leax -1,x
 beq _dline15
_dline16
 leau -80,u
_dline111
 lda ,u
 anda #$3f
 sta ,u
 subb ,s
 bcc _dline11
 addb 1,s
_dline12
 leax -1,x
 beq _dline15
 leau -80,u
_dline112
 lda ,u
 anda #$cf
 sta ,u
 subb ,s
 bcc _dline12
 addb 1,s
_dline13
 leax -1,x
 beq _dline15
 leau -80,u
_dline113
 lda ,u
 anda #$f3
 sta ,u
 subb ,s
 bcc _dline13
 addb 1,s
_dline14
 leax -1,x
 beq _dline15
 leau -80,u
_dline114
 lda ,u
 anda #$fc
 sta ,u
 subb ,s
 bcc _dline14
 addb 1,s
 leau 1,u
 leax -1,x
 bne _dline16
_dline15
 leas 2,s
#endasm
}

/* Draw a ENE line */
DLine2(p,x,y,dx,dy)
char *p;
int x,y,dx,dy;
{
#asm
 ldx 10,s
 lda 9,s
 ldb #80
 mul
 addd 4,s
 tfr d,u
 ldd 6,s
 lsra
 rorb
 lsrb
 leau b,u
 pshs x
 leax 1,x
 ldb 15,s
 stb ,s
 ldd 8,s
 lsra
 lda ,u
 rorb
 bcs _dline216
 lsrb
 bcs _dline215
 ldb 1,s
 lsrb
 bra _dline211
_dline215
 ldb 1,s
 lsrb
 bra _dline213
_dline216
 lsrb
 bcs _dline217
 ldb 1,s
 lsrb
 bra _dline212
_dline217
 ldb 1,s
 lsrb
 bra _dline214
_dline22
 addb 1,s
 sta ,u
 leau -80,u
 lda ,u
 bra _dline23
_dline21
_dline28
 lda ,u
_dline211
 anda #$3f
 leax -1,x
 beq _dline221
 subb ,s
 bcs _dline22
_dline23
_dline212
 anda #$cf
 leax -1,x
 beq _dline221
 subb ,s
 bcs _dline24
_dline25
_dline213
 anda #$f3
 leax -1,x
 beq _dline221
 subb ,s
 bcs _dline26
_dline27
_dline214
 anda #$fc
 sta ,u+
 leax -1,x
 beq _dline2e
 subb ,s
 bcc _dline21
 addb 1,s
 leau -80,u
 bra _dline28
_dline24
 addb 1,s
 sta ,u
 leau -80,u
 lda ,u
 bra _dline25
_dline26
 addb 1,s
 sta ,u
 leau -80,u
 lda ,u
 bra _dline27
_dline221
 sta ,u
_dline2e
 leas 2,s
#endasm
}

/* Draw ESE line */
DLine3(p,x,y,dx,dy)
char *p;
int x,y,dx,dy;
{
#asm
 ldx 10,s
 lda 9,s
 ldb #80
 mul
 addd 4,s
 tfr d,u
 ldd 6,s
 lsra
 rorb
 lsrb
 leau b,u
 pshs x
 leax 1,x
 ldb 15,s
 stb ,s
 ldd 8,s
 lsra
 lda ,u
 rorb
 bcs _dline316
 lsrb
 bcs _dline315
 ldb 1,s
 lsrb
 bra _dline311
_dline315
 ldb 1,s
 lsrb
 bra _dline313
_dline316
 lsrb
 bcs _dline317
 ldb 1,s
 lsrb
 bra _dline312
_dline317
 ldb 1,s
 lsrb
 bra _dline314
_dline32
 addb 1,s
 sta ,u
 leau 80,u
 lda ,u
 bra _dline33
_dline31
_dline38
 lda ,u
_dline311
 anda #$3f
 leax -1,x
 beq _dline321
 subb ,s
 bcs _dline32
_dline33
_dline312
 anda #$cf
 leax -1,x
 beq _dline321
 subb ,s
 bcs _dline34
_dline35
_dline313
 anda #$f3
 leax -1,x
 beq _dline321
 subb ,s
 bcs _dline36
_dline37
_dline314
 anda #$fc
 sta ,u+
 leax -1,x
 beq _dline3e
 subb ,s
 bcc _dline31
 addb 1,s
 leau 80,u
 bra _dline38
_dline34
 addb 1,s
 sta ,u
 leau 80,u
 lda ,u
 bra _dline35
_dline36
 addb 1,s
 sta ,u
 leau 80,u
 lda ,u
 bra _dline37
_dline321
 sta ,u
_dline3e
 leas 2,s
#endasm
}

/* Draw a SSE line */
/* p = pointer to beginning of screen *
/* x,y = location of first pixel */
/* dx,dy = offset to last pixel */
DLine4(p,x,y,dx,dy)
char *p;
int x,y,dx,dy;
{
#asm
 ldx 12,s
 lda 9,s
 ldb #80
 mul
 addd 4,s
 tfr d,u
 ldd 6,s
 lsra
 rorb
 lsrb
 leau b,u
 pshs x
 leax 1,x
 ldb 13,s
 stb ,s
 ldd 8,s
 lsra
 rorb
 bcs _dline48
 lsrb
 bcs _dline47
 ldb 1,s
 lsrb
 bra _dline411
_dline47
 ldb 1,s
 lsrb
 bra _dline413
_dline48
 lsrb
 bcs _dline49
 ldb 1,s
 lsrb
 bra _dline412
_dline49
 ldb 1,s
 lsrb
 bra _dline414
_dline41
 leax -1,x
 beq _dline45
_dline46
 leau 80,u
_dline411
 lda ,u
 anda #$3f
 sta ,u
 subb ,s
 bcc _dline41
 addb 1,s
_dline42
 leax -1,x
 beq _dline45
 leau 80,u
_dline412
 lda ,u
 anda #$cf
 sta ,u
 subb ,s
 bcc _dline42
 addb 1,s
_dline43
 leax -1,x
 beq _dline45
 leau 80,u
_dline413
 lda ,u
 anda #$f3
 sta ,u
 subb ,s
 bcc _dline43
 addb 1,s
_dline44
 leax -1,x
 beq _dline45
 leau 80,u
_dline414
 lda ,u
 anda #$fc
 sta ,u
 subb ,s
 bcc _dline44
 addb 1,s
 leau 1,u
 leax -1,x
 bne _dline46
_dline45
 leas 2,s
#endasm
}

/* Draw WSW line */
DLine6(p,x,y,dx,dy)
char *p;
int x,y,dx,dy;
{
#asm
 ldx 10,s
 lda 9,s
 ldb #80
 mul
 addd 4,s
 tfr d,u
 ldd 6,s
 lsra
 rorb
 lsrb
 leau b,u
 pshs x
 leax 1,x
 ldb 15,s
 stb ,s
 ldd 8,s
 lsra
 lda ,u
 rorb
 bcs _dline616
 lsrb
 bcs _dline615
 ldb 1,s
 lsrb
 bra _dline614
_dline615
 ldb 1,s
 lsrb
 bra _dline612
_dline616
 lsrb
 bcs _dline617
 ldb 1,s
 lsrb
 bra _dline613
_dline617
 ldb 1,s
 lsrb
 bra _dline611
_dline62
 addb 1,s
 sta ,u
 leau 80,u
 lda ,u
 bra _dline63
_dline61
_dline68
 lda ,u
_dline611
 anda #$fc
 leax -1,x
 beq _dline621
 subb ,s
 bcs _dline62
_dline63
_dline612
 anda #$f3
 leax -1,x
 beq _dline621
 subb ,s
 bcs _dline64
_dline65
_dline613
 anda #$cf
 leax -1,x
 beq _dline621
 subb ,s
 bcs _dline66
_dline67
_dline614
 anda #$3f
 sta ,u
 leau -1,u
 leax -1,x
 beq _dline6e
 subb ,s
 bcc _dline61
 addb 1,s
 leau 80,u
 bra _dline68
_dline64
 addb 1,s
 sta ,u
 leau 80,u
 lda ,u
 bra _dline65
_dline66
 addb 1,s
 sta ,u
 leau 80,u
 lda ,u
 bra _dline67
_dline621
 sta ,u
_dline6e
 leas 2,s
#endasm
}

/* Draw a WNW line */
DLine7(p,x,y,dx,dy)
char *p;
int x,y,dx,dy;
{
#asm
 ldx 10,s
 lda 9,s
 ldb #80
 mul
 addd 4,s
 tfr d,u
 ldd 6,s
 lsra
 rorb
 lsrb
 leau b,u
 pshs x
 leax 1,x
 ldb 15,s
 stb ,s
 ldd 8,s
 lsra
 lda ,u
 rorb
 bcs _dline716
 lsrb
 bcs _dline715
 ldb 1,s
 lsrb
 bra _dline714
_dline715
 ldb 1,s
 lsrb
 bra _dline712
_dline716
 lsrb
 bcs _dline717
 ldb 1,s
 lsrb
 bra _dline713
_dline717
 ldb 1,s
 lsrb
 bra _dline711
_dline72
 addb 1,s
 sta ,u
 leau -80,u
 lda ,u
 bra _dline73
_dline71
_dline78
 lda ,u
_dline711
 anda #$fc
 leax -1,x
 beq _dline721
 subb ,s
 bcs _dline72
_dline73
_dline712
 anda #$f3
 leax -1,x
 beq _dline721
 subb ,s
 bcs _dline74
_dline75
_dline713
 anda #$cf
 leax -1,x
 beq _dline721
 subb ,s
 bcs _dline76
_dline77
_dline714
 anda #$3f
 sta ,u
 leau -1,u
 leax -1,x
 beq _dline7e
 subb ,s
 bcc _dline71
 addb 1,s
 leau -80,u
 bra _dline78
_dline74
 addb 1,s
 sta ,u
 leau -80,u
 lda ,u
 bra _dline75
_dline76
 addb 1,s
 sta ,u
 leau -80,u
 lda ,u
 bra _dline77
_dline721
 sta ,u
_dline7e
 leas 2,s
#endasm
}

DrawLine(p,x1,y1,x2,y2)
char *p;
int x1,y1,x2,y2;
{
  if (x1<0 || x1>=320 || x2<0 || x2>=320 ||
      y1<0 || y1>=192 || y2<0 || y2>=192) {
    printf("(%d,%d)-(%d,%d)\n",x1,y1,x2,y2);
    exit(1);
  }
  if (x1>x2) {
    int temp;
    temp = x1;
    x1 = x2;
    x2 = temp;
    temp = y1;
    y1 = y2;
    y2 = temp;
  }
  {
    int dx = x2-x1;
    int dy = y2-y1;
    if (dx==0) {
      if (dy<0)
        DVLine(p,x1,y2,y1);
      else
        DVLine(p,x1,y1,y2);
    }
    else {
      if (dy<0) {
        dy = -dy;
        if (dx<dy)
          DLine1(p,x1,y1,dx,dy);
        else {
          if (dx<256)
            DLine2(p,x1,y1,dx,dy);
          else {
            int dx2 = (dx>>1)+1;
            int dy2 = (dy>>1);
            DLine2(p,x1,y1,dx2,dy2);
            DLine6(p,x2,y2,dx2,dy2);
          }
        }
      }
      else {
        if (dx<dy)
          DLine4(p,x1,y1,dx,dy);
        else {
          if (dx<256)
            DLine3(p,x1,y1,dx,dy);
          else {
            int dx2 = (dx>>1)+1;
            int dy2 = (dy>>1);
            
            DLine3(p,x1,y1,dx2,dy2);
            DLine7(p,x2,y2,dx2,dy2);
          }
        }
      }
    }
  }
}
