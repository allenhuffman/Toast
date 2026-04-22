#include "asmutil.h"

/* Divide 65536 by i where i>1, return result */

unsigned Div64KI(i)
unsigned i;
{
#asm
 lda 4,s
 bne _divki17
 lda 5,s
 cmpa #128
 bhi _divki17
 clrb
 lda #2
 cmpa 5,s
 blo _divki1
 suba 5,s
 addb #$80
_divki1 
 lsla
 cmpa 5,s
 blo _divki2
 suba 5,s
 addb #$40
_divki2
 lsla
 cmpa 5,s
 blo _divki3
 suba 5,s
 addb #$20
_divki3
 lsla
 cmpa 5,s
 blo _divki4
 suba 5,s
 addb #$10
_divki4
 lsla
 cmpa 5,s
 blo _divki5
 suba 5,s
 addb #$08
_divki5
 lsla
 cmpa 5,s
 blo _divki6
 suba 5,s
 addb #$04
_divki6
 lsla
 cmpa 5,s
 blo _divki7
 suba 5,s
 addb #$02
_divki7
 lsla
 cmpa 5,s
 blo _divki8
 suba 5,s
 addb #$01
_divki8
 stb 4,s
 clrb
 lsla
 cmpa 5,s
 blo _divki9
 suba 5,s
 addb #$80
_divki9
 lsla
 cmpa 5,s
 blo _divki10
 suba 5,s
 addb #$40
_divki10
 lsla
 cmpa 5,s
 blo _divki11
 suba 5,s
 addb #$20
_divki11
 lsla
 cmpa 5,s
 blo _divki12
 suba 5,s
 addb #$10
_divki12
 lsla
 cmpa 5,s
 blo _divki13
 suba 5,s
 addb #$08
_divki13
 lsla
 cmpa 5,s
 blo _divki14
 suba 5,s
 addb #$04
_divki14
 lsla
 cmpa 5,s
 blo _divki15
 suba 5,s
 addb #$02
_divki15
 lsla
 cmpa 5,s
 blo _divki16
 suba 5,s
 addb #$01
_divki16
 lda 4,s
 bra _divkie
_divki17
 ldx #0
 ldd #$100
 cmpd 4,s
 blo _divki18
 subd 4,s
 leax $100,x
_divki18
 lslb
 rola
 cmpd 4,s
 blo _divki19
 subd 4,s
 leax $80,x
_divki19
 lslb
 rola
 cmpd 4,s
 blo _divki20
 subd 4,s
 leax $40,x
_divki20
 lslb
 rola
 cmpd 4,s
 blo _divki21
 subd 4,s
 leax $20,x
_divki21
 lslb
 rola
 cmpd 4,s
 blo _divki22
 subd 4,s
 leax $10,x
_divki22
 lslb
 rola
 cmpd 4,s
 blo _divki23
 subd 4,s
 leax $8,x
_divki23
 lslb
 rola
 cmpd 4,s
 blo _divki24
 subd 4,s
 leax $4,x
_divki24
 lslb
 rola
 cmpd 4,s
 blo _divki25
 subd 4,s
 leax $2,x
_divki25
 lslb
 rola
 cmpd 4,s
 blo _divki26
 subd 4,s
 leax $1,x
_divki26
 tfr x,d
_divkie
#endasm
}

/* Divide a non negative long by an unsigned int and return an unsigned
   int result */
unsigned DivLI(l,i)
long l;
unsigned i;
{
#asm
 pshs y
 ldd 6,s
 leau 8,s
 leay 10,s
 bsr _divlis1
 pshs x
 leau 1,u
 bsr _divlis1
 tfr x,d
 lda 1,s
 leas 2,s
 puls y
 bra _divlie
_divlis1
 ldx #0
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli1
 subd ,y
 leax $80,x
_divli1
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli2
 subd ,y
 leax $40,x
_divli2
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli3
 subd ,y
 leax $20,x
_divli3
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli4
 subd ,y
 leax $10,x
_divli4
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli5
 subd ,y
 leax $8,x
_divli5
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli6
 subd ,y
 leax $4,x
_divli6
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli7
 subd ,y
 leax $2,x
_divli7
 lsl ,u
 rolb
 rola
 cmpd ,y
 blo _divli8
 subd ,y
 leax $1,x
_divli8
 rts
_divlie
#endasm
}

/* Multpily two signed integers and return a long */
LMulII(lp,i1,i2)
long *lp;
int i1;
int i2;
{
#asm
 lda 6,s
 bmi _lmulii5
 lda 8,s
 bmi _lmulii3
 bsr _lmuliis1
 bra _lmuliie
_lmulii3
 ldb 9,s
 coma
 comb
 addd #1
 std 8,s
 bsr _lmuliis1
_lmulii6
 ldd 2,x
 coma
 comb
 addd #1
 std 2,x
 bcs _lmulii4
 ldd ,x
 coma
 comb
 std ,x
 bra _lmuliie
_lmulii4
 ldd ,x
 coma
 comb
 addd #1
 std ,x
 bra _lmuliie
_lmulii5
 ldb 7,s
 coma
 comb
 addd #1
 std 6,s
 lda 8,s
 bmi _lmulii7
 bsr _lmuliis1
 bra _lmulii6
_lmulii7
 ldb 9,s
 coma
 comb
 addd #1
 std 8,s
 bsr _lmuliis1
 bra _lmuliie
_lmuliis1
 ldx 6,s
 lda 9,s
 ldb 11,s
 mul
 std 2,x
 lda 8,s
 ldb 11,s
 mul
 addb 2,x
 adca #0
 std 1,x
 lda 9,s
 ldb 10,s
 mul
 addd 1,x
 std 1,x
 bcs _lmulii2
 lda 8,s
 ldb 10,s
 mul
 addb 1,x
 adca #0
 std ,x
 bra _lmulii1
_lmulii2
 lda 8,s
 ldb 10,s
 mul
 addb 1,x
 adca #1
 std ,x
_lmulii1
 rts
_lmuliie
#endasm
}

/* return (int)((*l1-*l2)>>8) */
int ISubLLS(l1,l2)
long *l1;
long *l2;
{
#asm
 ldx 4,s
 ldu 6,s
 ldb 3,x
 subb 3,u
 ldd 1,x
 sbcb 2,u
 sbca 1,u
#endasm
}

/* return (int)((*l1+*l2)>>8) */
int IAddLLS(l1,l2)
long *l1;
long *l2;
{
#asm
 ldx 4,s
 ldu 6,s
 ldb 3,x
 addb 3,u
 ldd 1,x
 adcb 2,u
 adca 1,u
#endasm
}
