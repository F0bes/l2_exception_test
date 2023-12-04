#include "library.h"
#include <stdio.h>
#include <kernel.h>
#include <sio.h>

void sio_printf(const char *fmt, ...)
{
	char buf[256];
	va_list args;
	va_start(args, fmt);
	vsprintf(buf, fmt, args);
	va_end(args);
	sio_puts(buf);
}

func_ptr g_func_ptr;

void set_func_ptr(func_ptr ptr)
{
	g_func_ptr = ptr;
	sio_printf("set_func_ptr executed. Set global to %p\n", g_func_ptr);
}

void library_c_func()
{
	asm volatile(
		"mtps $zero, 0\n"
		"mtpc $zero, 0\n"
		"mtpc $zero, 1\n"
		"sync.p\n");

	//sio_printf("library_c_func executed. Executing %p\n", g_func_ptr);
	g_func_ptr();

}
extern void level_2_handler();

#define MIPS_J(func) (u32)((0x02 << 26) | (((u32)func) / 4))
void start()
{
	DI();
	ee_kmode_enter();
	*(vu32 *)UNCACHED_SEG(0x80000080) = MIPS_J(UNCACHED_SEG(level_2_handler));
	*(vu32 *)UNCACHED_SEG(0x80000084) = 0;

	// enable the instruction counter
	int pccr = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (12 << 5) | (1 << 31);

	asm volatile(
		"mtps %0, 0\n"
		"mtpc $zero, 0\n"
		"mtpc $zero, 1\n"
		"sync.p\n" ::"r"(pccr));
	ee_kmode_exit();
	EI();
	FlushCache(INVALIDATE_ICACHE);
}


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
