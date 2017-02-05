#include<dos.h>
#include<conio.h>

void mode(int mode_code)
{
	union REGS r;
	r.h.al = mode_code;
	r.h.ah = 0x0;
	int86(0x10,&r,&r);
}
//===========================================
void palette(int pnum)
{
	union REGS r;
	r.h.bh = 1; /*code for mode 1*/
	r.h.bl = pnum;
	r.h.ah = 0xb;
	int86(0x10,&r,&r);
}
//===========================================
void mempoint(int x,int y, int color)
{
	union mask{
					char c[2];
					int i;
					} bit_mask;
	int i, index, bitpos;
	unsigned char t;
	char xor;
	char far *ptr=(char far *)0xb8000000;
	bit_mask.i = 0xFF3F; /*11111111 00111111*/
	if ( x<0 || x>199 || y<0 || y> 319)
		return;
	xor   = color & 128;
	color = color & 127;
	bitpos= y % 4;
	color      <<= 2*(3-bitpos);
	bit_mask.i >>= 2 * bitpos;
	index = x * 40 + (y/4);
	if( x % 2)
		index+=8152;
	if(!xor)
	{
		t=*(ptr+index) & bit_mask.c[0];
//		*(ptr+index) = t | color;
	}
	else
	{
		t=*(ptr+index) | (char)0 ;
//		*(ptr+index) = t ^ color;
	}
	*(ptr+10)=YELLOW;
	*(ptr+12)=YELLOW;
	*(ptr+13)=YELLOW;

//	*(ptr+8192)=0xff;
//	*(ptr+80)=0xff;
//	*(ptr+80+8192)=0xff;

}
//==================================================
void main(void)
{
	mode(18);
	palette(0);
//	for(int i=0 ; i<200 ; i++)
//		for(int j=0 ; j<320 ; j++)
			mempoint(10,10,2);
	getch();
}