#include <stdarg.h>
#include <stdio.h>
#include <ntifs.h>
#include <ntimage.h>
#include "io.hpp"

void driver::io::dbgprint(PCCH format, ...)
{

	//https://github.com/unmaewei/Full-Kernel-Driver/blob/main/full%20kernel%20bypass/io/io.cpp

	DbgPrintEx(0, 0, format);

	//CHAR message[512];
	//va_list _valist;
	//va_start(_valist, format);
	//const ULONG N = _vsnprintf_s(message, sizeof(message) - 1, format, _valist);
	//message[N] = L'\0';

	//vDbgPrintExWithPrefix("[Kernel Driver] ", DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, message, _valist);

	//va_end(_valist);
}
