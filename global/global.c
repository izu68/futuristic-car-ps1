#include "global.h"

DISPENV disp[2];
DRAWENV draw[2];
int db;

DR_TPAGE *tpage;
u_long ot[2][OTLEN];    // Ordering table length
char pribuff[2][32768]; // Primitive buffer
char *nextpri;          // Next primitive pointer