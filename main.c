#include "global/global.h"
#include "init/init.h"
#include "renderer/display.h"
#include "renderer/objects.h"



int main () {
    init ();
    obj_tile newtile;
    
    extern u_long tim_player_car[];
    obj_sprite player_car;

    
    
    while(1) {
        FntPrint("HELLO WORLD!");
        ClearOTagR(ot[db], OTLEN);  // Clear ordering table

        drawobj_sprite (&player_car, 32, 32, 64, 64);

        update_display ();
    }
    return 0;
}