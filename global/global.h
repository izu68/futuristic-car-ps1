#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <sys/types.h>
#include <libetc.h>	// Includes some functions that controls the display
#include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	// GPU library header

extern DISPENV disp[2];
extern DRAWENV draw[2];
extern int db;

#endif