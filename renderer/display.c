#include "display.h"
#include "../global/global.h"

void update_display (void) {
    // Wait for GPU to finish drawing and V-Blank
    DrawSync (0);
    VSync (0);
    
    // Flip buffer counter
    db = !db;
    
    // Apply environments
    PutDispEnv (&disp[db]);
    PutDrawEnv (&draw[db]);
    
    // Enable display
    SetDispMask (1);
}