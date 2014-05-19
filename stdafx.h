#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "ddkhelpers.h"
#include <ntddk.h>

#ifdef __cplusplus
}
#endif

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
#define PrintPrefix "[yy]["__FUNCTION__":"LINE_STRING"] "

#if _DEBUG
#define YyPrint(...) DbgPrint(PrintPrefix __VA_ARGS__)
#else
#define YyPrint(...)
#endif

#define YyPrintAPI(API) YyPrint(API##" failed, Status: 0x%08x\n", Status)
