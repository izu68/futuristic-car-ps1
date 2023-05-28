#include "init.h"
#include "../global/global.h"

void init (void) {
    // Reset GPU and enable interrupts
    ResetGraph (0);
    
    // Configures the pair of DISPENVs and DRAWENVs for 320x240 mode (NTSC)
    SetDefDispEnv (&disp[0], 0, 0, 640, 480);
    SetDefDispEnv (&disp[1], 0, 0, 640, 480);
    SetDefDrawEnv (&draw[0], 0, 0, 640, 480);
    SetDefDrawEnv (&draw[1], 0, 0, 640, 480);
    
    // Specifies the clear color of the DRAWENV
    setRGB0 (&draw[0], 0, 0, 0);
    setRGB0 (&draw[1], 0, 0, 0);
    
    // Enable background clear
    draw[0].isbg = 1;
    draw[1].isbg = 1;
    
    // Apply environments
    PutDispEnv (&disp[0]);
    PutDrawEnv (&draw[0]);

    // Make sure db starts with zero
    db = 0;
    
    // Load the internal font texture
    FntLoad (960, 0);
    // Create the text stream
    FntOpen (0, 8, 320, 224, 0, 100);
}