#include <SDL2/SDL.h>

#include "Color12.h"
#include "MonoIMG.c"

typedef struct Entity {
    uint32_t id;
    Vect2    pos;
    Vect2    vel;
    MonoIMG  img;
} Entity;

Entity*  collection_entities;
uint64_t collection_pos        = 0;
uint64_t collection_length     = 0;
uint16_t collection_batch_size = 32;

void Entity_move( Entity* this ) {
    this->pos.x += this->vel.x;
    this->pos.y += this->vel.y;
    // remove this // for testing // WRAP
    if ( this->pos.x < 0 ) this->pos.x -= this->pos.x;
    if ( this->pos.x > WIDTH ) this->pos.x = 0;
};

Entity Entity_copy( Entity* this ) {
    return ( Entity ){ .id = this->id, .pos = this->pos, .vel = this->vel, .img = this->img };
}

Entity Entity_new( uint32_t id, Vect2 pos, Vect2 vel, uint8_t* newImgData ) {
    return ( Entity ){ .id = id, .pos = pos, .vel = vel, .img = MonoIMG_new( newImgData ) };
}

void Entity_destroy( Entity* this ) {
    // MonoIMG_destroy( this->img );
}

void Entity_draw( Entity* this ) {
    MonoIMG_draw( &this->img, rand() & 1, this->pos );
};

void Entity_collection_act_all() {
    for ( size_t i = 0; i < collection_pos; i++ ) {
        Entity_move( &collection_entities [ i ] );
        Entity_draw( &collection_entities [ i ] );
    }
}

void Entity_collection_add( Entity this ) {
    if ( collection_length == 0 ) {
        collection_entities = (Entity*)calloc( 1, sizeof( Entity ) );
        collection_length   = 1;
    }
    if ( collection_pos >= collection_length ) {    // add elements if needed
        collection_entities = (Entity*)realloc(
            collection_entities, ( collection_length + collection_batch_size ) * sizeof( Entity ) );
        collection_length += collection_batch_size;
        printf( "resizeArr: %i %i \n", collection_pos, collection_length );
    }
    collection_entities [ collection_pos ] = this;
    collection_pos++;
}
