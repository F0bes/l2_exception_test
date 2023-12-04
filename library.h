#pragma once

typedef void (*func_ptr)(void);

void set_func_ptr(func_ptr ptr);

void start(void);
