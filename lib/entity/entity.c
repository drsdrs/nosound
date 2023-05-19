#include <SDL2/SDL.h>
#include "../color12/color12.h"

SDL_Rect rect_source;
SDL_Rect rect_target;


typedef struct MonoIMG{
  uint16_t width;
  uint16_t height;
  Color12 color;
  SDL_Texture *texture;
  uint16_t frames;
  uint8_t* data;
} MonoIMG;


typedef struct Entity{
  uint32_t id;
  Vect2 pos;
  Vect2 vel;
  MonoIMG img;
} Entity;

void Entity_move( Entity* this ){
  this->pos.x += this->vel.x;
  this->pos.y += this->vel.y;
};

Entity Entity_new( uint32_t id, Vect2 pos, Vect2 vel ){
  return (Entity) { .id=id, .pos=pos, .vel=vel};
}

void Entity_destroy( Entity* this ){
  //MonoIMG_destroy( this->img );
}

void Entity_draw( Entity* this ){

};

void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel){
  Uint32 * const target_pixel =
    (Uint32 *) ((Uint8 *) surface->pixels +
      y * surface->pitch +
      x * surface->format->BytesPerPixel);
  *target_pixel = pixel;
}


uint8_t imgData[3+8] = {
  8,4,2,
  0b11111111,
  0b11110000,
  0b10011001,
  0b11100111,
  0b10111101,
  0b01000010,
  0b00001111,
  0b11111111
};

uint8_t imgData1[3+8] = {
  16, 4, 1,
  0b11111111, 0b11111111,
  0b11000000, 0b00000000,
  0b11000000, 0b00000000,
  0b11111111, 0b11111111
};


void MonoIMG_draw_to_surface(SDL_Surface* surface, MonoIMG* img){

  for (int y = 0; y < img->height; ++y){
    for (int x = 0; x < img->width; ++x){
      int srcX = x & (img->width-1);
      int srcY = y;//(y+x) / (img.width);
      uint8_t sourcePixel = ( img->data[srcY]>>(7-srcX) )&1;
      //printf("%i\n", y);
      set_pixel( surface, x, y, 0xFFFFffff*sourcePixel );
    }
  }

}

void MonoIMG_destroy( MonoIMG* img ){
  SDL_DestroyTexture( img->texture );
  free(img->data);
}

void MonoIMG_texture_create( MonoIMG* img ){
  SDL_Surface* surface = SDL_CreateRGBSurface(0, img->width, img->height, 32, 0, 0, 0, 0);

  for (int y = 0; y < img->height; ++y){
    for (int x = 0; x < img->width; ++x){
      int srcX = x & (img->width-1);
      int srcY = y;//(y+x) / (img->width);
      uint8_t sourcePixel = ( img->data[srcY]>>(7-srcX) )&1;
      //printf("%i\n", y);
      set_pixel( surface, x, y, 0xFFFFffff*sourcePixel );
    }
  }

  img->texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_SetTextureColorMod( img->texture, 255,255,255);

  SDL_FreeSurface(surface);
}



MonoIMG MonoIMG_create( uint8_t* newImgData ){
  MonoIMG newImg;
  newImg.width = newImgData[0];
  newImg.height = newImgData[1];
  newImg.frames = newImgData[2];

  int newImgByteSize = newImg.height*sizeof(uint8_t);
  newImg.data = (uint8_t*) malloc( newImgByteSize );
  memcpy( newImg.data, newImgData+sizeof(uint8_t)*3, newImgByteSize );

  MonoIMG_texture_create( &newImg );

  return newImg;
}

void MonoIMG_draw(MonoIMG* img, Vect2 drawPos){
  rect_source.x = drawPos.x;
  rect_source.y = drawPos.y;
  rect_target.x = 0;
  rect_target.y = 0;
  rect_source.w = img->width;
  rect_source.h = img->height;

  rect_target.w = img->width;
  rect_target.h = img->height;

  SDL_RenderCopy(renderer, img->texture, &rect_target, &rect_source);
}

void testEntity(int x){
  x %= WIDTH;
  //printf("x: %i\n", x);
  Vect2 pos = vect2_new(x,0);
  MonoIMG i = MonoIMG_create( imgData );
  MonoIMG_draw( &i, pos );
  MonoIMG_destroy( &i );
}