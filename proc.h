#pragma once

#include <wchar.h>

void* proc_handle;
unsigned long proc_id;

int proc_init(const char* window_name);
size_t proc_module(wchar_t* module_name);
void* proc_read(void* address);