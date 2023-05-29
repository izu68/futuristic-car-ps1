#include "display.h"
#include "../global/global.h"

void update_display (void) {
    DrawSync (0);                // Wait for any graphics processing to finish
    
    VSync (0);                   // Wait for vertical retrace

    PutDispEnv (&disp[db]);      // Apply the DISPENV/DRAWENVs
    PutDrawEnv (&draw[db]);

    SetDispMask (1);             // Enable the display

    DrawOTag (ot[db]+OTLEN-1);   // Draw the ordering table
    
    db = !db;                    // Swap buffers on every pass (alternates between 1 and 0)
    nextpri = pribuff[db];       // Reset next primitive pointer
}