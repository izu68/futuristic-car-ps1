#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#define OTLEN 8

extern DISPENV disp[2];
extern DRAWENV draw[2];
extern int db;

extern DR_TPAGE *tpage;
extern u_long ot[2][OTLEN];    // Ordering table length
extern char pribuff[2][32768]; // Primitive buffer
extern char *nextpri;          // Next primitive pointer

#endif