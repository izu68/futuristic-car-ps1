#include "global/global.h"
#include "init/init.h"
#include "renderer/display.h"
#include "renderer/objects.h"



int main () {
    init ();
    
    obj_sprite player_car;

    loadobj_sprite (&player_car, "\\textures\\Pcar.tim;1");
    
    while(1) {  
        ClearOTagR(ot[db], OTLEN);  // Clear ordering table
        FntPrint("HELLO WORLD!");

        drawobj_sprite (&player_car, 0, 0, 64, 64);

        update_display ();
    }
    return 0;
}