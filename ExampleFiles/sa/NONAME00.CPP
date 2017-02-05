
#include <stdio.h>
#include <math.h>
#include <conio.h>

#include <conio.h>

int main(void)
{
	int k;
   clrscr();
	cprintf("Press any key to continue:");
	while (!kbhit()){k=kbhit() ;}
//	for(int i=0; i<10;i++)
//		k=kbhit();

	;
	cprintf("\nA key was pressed.%d\n");
	return 0;
}
