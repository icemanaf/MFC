#pragma once
#include "windows.h"
#include "psapi.h"

#ifndef _SYS_INFO
#define _SYS_INFO

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void InitSysInfo();

double GetProcessCPU();

double GetProcessMemory();


#endif

