#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Color12.h"
#include "file.h"
#include "jsmn.h"
#include "path.h"
#include "prg.h"
#include "spriteManager.h"
#include "tv.h"

#define SPRITESHEET_SIZE 1024

SDL_Rect            rect_source;
SDL_Rect            rect_target;
static SDL_Texture *texture = NULL;


Sprite *spriteManager_collection;
uint16_t sprite_cnt;

Sprite sprite_new( char *name, int width, int height, int frames, int *texture_positions ) {
    Sprite sprite = {};
    sprite.name   = name;
    sprite.size.x = width;
    sprite.frames = frames;
    sprite.size.y = height;
    if ( texture_positions == NULL ) {
        sprite.texture_positions = NULL;
        return sprite;
    }
    sprite.texture_positions = malloc( sizeof( int ) * frames * 2 );
    for ( size_t i = 0; i < frames * 2; i++ ) {
        sprite.texture_positions[i] = texture_positions[i];
    }
    return sprite;
};

void spriteManager_destroy() {
    SDL_DestroyTexture( texture );
    free( texture );
}

int json_equal( const char *json, jsmntok_t *tok, const char *s ) {
    if ( tok->type == JSMN_STRING && (int)strlen( s ) == tok->end - tok->start &&
         strncmp( json + tok->start, s, tok->end - tok->start ) == 0 ) {
        return 0;
    }
    return -1;
}

int json_number_from_token( char *JSON_STRING, jsmntok_t *tPos ) {
    return strtol( JSON_STRING + tPos->start, NULL, 10 );
}
int json_string_from_token( char *JSON_STRING, jsmntok_t *tPos ) {
    // TODO !!!
}

Sprite *spriteManager_find( char *spriteName ) {
	for (size_t i = 0; i < sprite_cnt; i++) {
		if( strcmp( spriteName, spriteManager_collection[i].name )==0 ) return &spriteManager_collection[i];
		//printf("1ERR: sprite: \"%s\" not found!!!\n", spriteManager_collection[i].name);
	}
	printf("ERR: sprite: \"%s\" not found!!!\n", spriteName);
	return NULL;
}

Sprite *spriteManager_get( int spriteNr ) {
	if(spriteNr<sprite_cnt && spriteNr>=0) return &spriteManager_collection[spriteNr];
	printf("ERR: spriteNr: \"%i\" not found!!!\n", spriteNr);
	exit(1);
}
void spriteManager_json_spritesheet_parse() {
	sprite_cnt = 0;
    spriteManager_collection = (Sprite *)calloc( 1, sizeof( Sprite ) );
    Sprite current_sprite           = sprite_new( NULL, -1, -1, -1, NULL );

    int         maxTokens = 1024;
    jsmn_parser p;
    jsmntok_t   t[maxTokens];
    jsmn_init( &p );
    char *JSON_STRING = file_read( path_spritesheetAtlas_get( prg_name_get() ) );
    int   r           = jsmn_parse( &p, JSON_STRING, strlen( JSON_STRING ), t, maxTokens );    // "s" is the char array holding the json content

    if ( r < 0 ) {
        printf( "\n\nFailed to parse JSON: %d\n", r );
        return;
    }

    if ( r < 1 || t[0].type != JSMN_OBJECT ) { /* Assume the top-level element is an object */
        printf( "\n\nObject expected\n" );
        return;
    }

    for ( size_t tokenPos = 1; tokenPos < r; tokenPos++ ) {
        if ( json_equal( JSON_STRING, &t[tokenPos], "width" ) == 0 ) { /* We may use strndup() to fetch string value */
            //printf( "- Width: %i\n", json_number_from_token( JSON_STRING, &t[tokenPos + 1] ) );
            current_sprite.size.x = json_number_from_token( JSON_STRING, &t[tokenPos + 1] );
            tokenPos++;
        } else if ( json_equal( JSON_STRING, &t[tokenPos], "height" ) == 0 ) {
            //printf( "- Height: %i\n", json_number_from_token( JSON_STRING, &t[tokenPos + 1] ) );
            current_sprite.size.y = json_number_from_token( JSON_STRING, &t[tokenPos + 1] );
            tokenPos++;
        } else if ( json_equal( JSON_STRING, &t[tokenPos], "frames" ) == 0 ) {
            size_t j;
            int    texture_pos_amount = t[tokenPos + 1].size;
            //printf( "- Frames %i: sprite_cnt: %i\n", texture_pos_amount, sprite_cnt );

            current_sprite.texture_positions = malloc( sizeof( int ) * texture_pos_amount * 2 );
            current_sprite.frames            = texture_pos_amount;
            int texture_pos                  = 0;
            for ( size_t j = 0; j < texture_pos_amount * 3; j += 3 ) {
                //printf( "  X: %i  - ", json_number_from_token( JSON_STRING, &t[tokenPos + j + 3] ) );
                //printf( "  Y: %i\n", json_number_from_token( JSON_STRING, &t[tokenPos + j + 4] ) );
                current_sprite.texture_positions[texture_pos++] = json_number_from_token( JSON_STRING, &t[tokenPos + j + 3] );
                current_sprite.texture_positions[texture_pos++] = json_number_from_token( JSON_STRING, &t[tokenPos + j + 4] );
            }
            tokenPos += texture_pos_amount * 3 + 1;
        } else if ( JSMN_STRING == t[tokenPos].type || tokenPos == r ) {
            if ( current_sprite.size.x == -1 || current_sprite.size.y == -1 || current_sprite.frames == -1 || current_sprite.texture_positions == NULL ) {    // err, sprite incomplete
                if ( sprite_cnt != 0 ) printf( "ERR, sprite not complete. Check spritesheet.json" );                                                          // dont increment pointer, dont assing

            } else {    // save sprite and inc
                        // alloc mem
                Sprite *tmp = (Sprite *)realloc( spriteManager_collection, sizeof( Sprite ) * ( sprite_cnt + 2 ) );
                if ( tmp == NULL ) {
                    printf( "Memory allocation error in spriteManager\n" );
                    exit( 1 );
                }
                spriteManager_collection = tmp;
                // save old current sprite
                spriteManager_collection[sprite_cnt] = current_sprite;
                printf( "- Saving sprite nr.: %i name: %s + %s\n", sprite_cnt, current_sprite.name, spriteManager_collection[sprite_cnt].name );
                // make new current sprite
                current_sprite = sprite_new( NULL, -1, -1, -1, NULL );
                //
                sprite_cnt++;
            }
            if ( current_sprite.name == NULL ) {
                char *spriteName    = strndup( JSON_STRING + t[tokenPos].start, t[tokenPos].end - t[tokenPos].start );
                current_sprite.name = spriteName;
                //printf( "\n## -- new sprite name: %s \n", spriteName );
            }

        } else {
            //printf( "?????? %.*s \n", t[tokenPos].end - t[tokenPos].start, JSON_STRING + t[tokenPos].start );
        }
    }
    spriteManager_collection[sprite_cnt] = current_sprite;
    printf( "- Saving sprite nr.: %i name: %s + %s\n", sprite_cnt, current_sprite.name, spriteManager_collection[sprite_cnt].name );
    for ( size_t ii = 0; ii < sprite_cnt + 1; ii++ ) {
        //printf( "YYY %s\n", spriteManager_collection[ii].name );
        //printf( "XX %i\n", spriteManager_collection[ii].size.x );
    }
    sprite_cnt++;
    free( JSON_STRING );
}

void spriteManager_setup() {
    /*		JSON-PARSE	end	*/
    spriteManager_json_spritesheet_parse();
    texture = IMG_LoadTexture( renderer, path_spritesheet_get( prg_name_get() ) );
    if ( texture == NULL ) {
        printf( "ERR!!!   texture: %ld\n", texture );
        exit( 1 );
    };
    // exit( 1 );
}

void spriteManager_draw( Sprite *sprite, Vect2 drawPos, uint16_t frame ) {
    rect_target.x = drawPos.x;
    rect_target.y = drawPos.y;
    rect_target.w = sprite->size.x;
    rect_target.h = sprite->size.y;

    rect_source.x = sprite->texture_positions[frame * 2];
    rect_source.y = sprite->texture_positions[frame * 2 + 1];
    rect_source.w = sprite->size.x;
    rect_source.h = sprite->size.y;

    SDL_RenderCopy( renderer, texture, &rect_source, &rect_target );
}
