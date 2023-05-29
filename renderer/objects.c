#include "objects.h"
#include "../global/global.h"
#include <libcd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// CD loading function
char *loadfile( char *filename )
{
    CdlFILE filePos;
    int     numsecs;
    char    *buff;
    
    buff = NULL;
    
    /* locate the file on the CD */
    if( CdSearchFile( &filePos, filename ) == NULL )
    {
        /* print error message if file not found */
        printf ("file not found in CD fs");
    }
    else
    {
        /* calculate number of sectors to read for the file */
        numsecs = (filePos.size+2047)/2048;
        
        /* allocate buffer for the file (replace with malloc3() for PsyQ) */
        buff = (char*)malloc( 2048*numsecs );
        
        /* set read target to the file */
        CdControl( CdlSetloc, (u_char*)&filePos.pos, 0 );
        
        /* start read operation */
        CdRead( numsecs, (u_long*)buff, CdlModeSpeed );
        
        /* wait until the read operation is complete */
        CdReadSync( 0, 0 );
    }
    
    return( buff );
    
} /* loadfile */

// Texture upload function
void internal_loadobj_sprite_loadobj_texture (u_long *tim, TIM_IMAGE *tparam) {

    // Read TIM parameters (PsyQ)
    OpenTIM(tim);
    ReadTIM(tparam);

    // Read TIM parameters (PSn00bSDK)
    //GetTimInfo(tim, tparam);

    // Upload pixel data to framebuffer
    LoadImage(tparam->prect, (u_long*)tparam->paddr);
    DrawSync(0);

    // Upload CLUT to framebuffer if present
    if( tparam->mode & 0x8 ) {

        LoadImage(tparam->crect, (u_long*)tparam->caddr);
        DrawSync(0);

    }
}

void loadobj_sprite (obj_sprite *spr, char *filename) {
    TIM_IMAGE   my_image;       // TIM image parameters
    u_long       *filebuff;      // Pointer for the file loaded from the disc

    if( filebuff = (u_long*)loadfile( filename ) )
    {    
        // Load the texture
        internal_loadobj_sprite_loadobj_texture (filebuff, &spr->tim_param);

        // Copy the TIM coordinates
        spr->tim_prect   = *spr->tim_param.prect;
        spr->tim_crect   = *spr->tim_param.crect;
        spr->tim_mode    = spr->tim_param.mode;

        // Calculate U,V offset for TIMs that are not page aligned
        spr->tim_u_offset = (spr->tim_prect.x%64)<<(2-(spr->tim_mode&0x3));
        spr->tim_v_offset = (spr->tim_prect.y&0xff);
        
        // Free the file buffer
        free( filebuff );
    }
    else
    {
        // Output error text that the image failed to load
        printf ("resource loading failed");
    }    
}

void drawobj_tile (obj_tile tile, int x, int y, int w, int h, u_char r, u_char g, u_char b) {
    tile.tile = (TILE*)nextpri;      // Cast next primitive

    setTile(tile.tile);              // Initialize the primitive (very important)
    setXY0(tile.tile, x, y);       // Set primitive (x,y) position
    setWH(tile.tile, w, h);        // Set primitive size
    setRGB0(tile.tile, r, g, b); // Set color yellow
    addPrim(&ot[db], tile.tile);      // Add primitive to the ordering table
    
    nextpri += sizeof(TILE);    // Advance the next primitive pointer
}

void drawobj_sprite (obj_sprite *spr, int x, int y, int w, int h) {
    spr->spr = (SPRT*)nextpri;
    setSprt(spr->spr);                  // Initialize the primitive (very important)
    setXY0( spr->spr, x, y);           // Position the sprite at (48,48)
    setWH(  spr->spr, w, h);            // Set size to 64x64 pixels
    setUV0( spr->spr, spr->tim_u_offset, spr->tim_v_offset);                 // Set UV coordinates       
    setClut(spr->spr, spr->tim_crect.x, spr->tim_crect.y);                   // Set CLUT coordinates to sprite
    spr->r = 128; spr->g = 128; spr->b = 128;
    setRGB0(spr->spr, spr->r, spr->g, spr->b);                // Set primitive color
        
    addPrim(&ot[db], spr->spr);          // Sort primitive to OT
    nextpri += sizeof(SPRT);        // Advance next primitive address


    tpage = (DR_TPAGE*)nextpri;
    setDrawTPage(tpage, 0, 1, getTPage (spr->tim_param.mode&0x3, 0, spr->tim_param.prect->x, spr->tim_param.prect->y));     // Set TPage primitive
         
    addPrim(&ot[db], tpage);         // Sort primitive to OT
    nextpri += sizeof(DR_TPAGE);    // Advance next primitive address
}

void drawobj_rotsprite (obj_sprite *spr, int x, int y, int pw, int ph, int angle, int scale) {
    POLY_FT4 *quad;
    SVECTOR	s[4];
    SVECTOR	v[4];

    int i,cx,cy;

    // calculate the pivot point (center) of the sprite
    cx = pw>>1;
    cy = ph>>1;

    // increment by 0.5 on the bottom and right coords so scaling
    // would increment a bit smoother
    s[0].vx = -(((pw*scale)>>12)-cx);
    s[0].vy = -(((ph*scale)>>12)-cy);

    s[1].vx = (((pw*scale)+2048)>>12)-cx;
    s[1].vy = s[0].vy;

    s[2].vx = -(((pw*scale)>>12)-cx);
    s[2].vy = (((ph*scale)+2048)>>12)-cy;

    s[3].vx = (((pw*scale)+2048)>>12)-cx;
    s[3].vy = s[2].vy;
    
    // a simple but pretty effective optimization trick
    cx = ccos( angle );
    cy = csin( angle );
    
    // calculate rotated sprite coordinates
    for( i=0; i<4; i++ )
    {
        v[i].vx = (((s[i].vx*cx)
            -(s[i].vy*cy))>>12)+x;
        v[i].vy = (((s[i].vy*cx)
            +(s[i].vx*cy))>>12)+y;
    }

    // initialize the quad primitive for the sprite
    quad = (POLY_FT4*)nextpri;
    setPolyFT4( quad );

    // set CLUT and tpage to the primitive
    setTPage( quad, spr->tim_mode&0x3, 0, spr->tim_prect.x, spr->tim_prect.y );
    setClut( quad, spr->tim_crect.x, spr->tim_crect.y );

    // set color, screen coordinates and texture coordinates of primitive
    setRGB0( quad, 128, 128, 128 );
    setXY4( quad,
        v[0].vx, v[0].vy,
        v[1].vx, v[1].vy,
        v[2].vx, v[2].vy,
        v[3].vx, v[3].vy );
    setUVWH( quad, spr->tim_u_offset, spr->tim_v_offset, pw, ph );

    // add it to the ordering table
    addPrim( &ot[db], quad );
    nextpri += sizeof(POLY_FT4);



    tpage = (DR_TPAGE*)nextpri;
    setDrawTPage(tpage, 0, 1, getTPage (spr->tim_param.mode&0x3, 0, spr->tim_param.prect->x, spr->tim_param.prect->y));     // Set TPage primitive
         
    addPrim(&ot[db], tpage);         // Sort primitive to OT
    nextpri += sizeof(DR_TPAGE);    // Advance next primitive address
}