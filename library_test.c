#include "library.h"
#include <stdio.h>
#include <sio.h>
#include <kernel.h>

volatile int flag = 0;
void level_2_c_handler()
{
	flag = 1;

	sio_puts("level_2_c_handler executed\n");
}

int main(void)
{
		printf("Hello, world!\n");
	set_func_ptr(level_2_c_handler);
	start();

	while(1)
	{
		int pc0;
		asm volatile("mfpc %0, 0" : "=r"(pc0));
		printf("counter value: %d\n", pc0);

		if(flag)
		{
			printf("flag is set\n");
			break;
		}
	}
	SleepThread();
}
