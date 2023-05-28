#include "global/global.h"
#include "init/init.h"
#include "renderer/display.h"



int main () {
    // Initialize graphics and stuff
    init ();
    
    // Main loop
    while(1)
    {
        FntPrint("HELLO WORLD!");

        FntFlush (-1);
        update_display ();
    }
    
    return 0;
}