#include <SDL2/SDL.h>

#include "Color12.h"
#include "Vect2.h"
#include "sdlHelper.h"

SDL_Rect rect_source;
SDL_Rect rect_target;

typedef struct MonoIMG {
    uint16_t     width;
    uint16_t     height;
    Color12      color;
    SDL_Texture *texture;
    uint16_t     frames;
    uint8_t     *data;
} MonoIMG;

// void MonoIMG_draw_to_surface(SDL_Surface* surface, MonoIMG* img){

// for (int y = 0; y < img->height; ++y){
// for (int x = 0; x < img->width; ++x){
// int srcX = x & (img->width-1);
// int srcY = y;//(y+x) / (img.width);
// uint8_t sourcePixel = ( img->data[srcY]>>(7-srcX) )&1;
////printf("%i\n", y);
// set_pixel( surface, x, y, 0xFFFFffff*sourcePixel );
//}
//}
//}

void set_pixel( SDL_Surface *surface, int x, int y, Uint32 pixel ) {
    Uint32 *const target_pixel = (Uint32 *)( (Uint8 *)surface->pixels + y * surface->pitch +
                                             x * surface->format->BytesPerPixel );
    *target_pixel              = pixel;
}

void MonoIMG_destroy( MonoIMG *img ) {
    SDL_DestroyTexture( img->texture );
    free( img->data );
}

void MonoIMG_texture_create( MonoIMG *img ) {
    uint16_t     totalImgHeight = img->height * img->frames;
    SDL_Surface *surface = SDL_CreateRGBSurface( 0, img->width, totalImgHeight, 16, 0, 0, 0, 0 );

    for ( int y = 0; y < totalImgHeight; ++y ) {
        for ( int x = 0; x < img->width; ++x ) {
            int     srcX        = x & ( img->width - 1 );
            int     srcY        = y;    //(y+x) / (img->width);
            uint8_t sourcePixel = ( img->data [ srcY ] >> ( 7 - srcX ) ) & 1;
            // printf("%i\n", y);
            set_pixel( surface, x, y, 0xFFFFffff * sourcePixel );
        }
    }

    img->texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_SetTextureColorMod( img->texture, 255, 255, 255 );

    SDL_FreeSurface( surface );
}

MonoIMG MonoIMG_new( uint8_t *newImgData ) {
    MonoIMG newImg;
    newImg.width  = newImgData [ 0 ];
    newImg.height = newImgData [ 1 ];
    newImg.frames = newImgData [ 2 ];

    int newImgByteSize = newImg.height * sizeof( uint8_t );
    newImg.data        = (uint8_t *)malloc( newImgByteSize );
    memcpy( newImg.data, newImgData + sizeof( uint8_t ) * 3, newImgByteSize );

    MonoIMG_texture_create( &newImg );

    return newImg;
}

void MonoIMG_draw( MonoIMG *img, uint16_t draw_frame, Vect2 drawPos ) {
    if ( img->frames <= draw_frame - 1 ) {
        printf( "MonoIMG_draw error, frame no: %i isnt here. Img max frames: %i\n", draw_frame,
                img->frames );
        exit( 1 );
    }
    rect_source.x = drawPos.x;
    rect_source.y = drawPos.y;
    rect_source.w = img->width;
    rect_source.h = img->height;

    rect_target.x = 0;
    rect_target.y = img->height * draw_frame;
    rect_target.w = img->width;
    rect_target.h = img->height * ( draw_frame + 1 );

    SDL_RenderCopy( renderer, img->texture, &rect_target, &rect_source );
}
