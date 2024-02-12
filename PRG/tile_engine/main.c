#include "perling.c"

struct Tile {
	uint16_t id;
	int tileOptions;
	uint16_t itemNr;
};

struct Object { // can be item or entity
	uint16_t id;
	uint8_t amount;
};

struct Entity {	/* 2 types of entity = animals OR workers */
	int id;
	int type;
	int pos;
};

int i,map_x,map_y = 0;
int seed;

uint16_t map_width;
uint16_t map_height;
uint16_t map_size;
uint8_t* tile_data;
uint32_t* entity_data;

//struct Tile *tileCollection[map_size];


void entity_data_set( uint32_t pos, uint8_t type, uint8_t variant, uint8_t data0, uint8_t data1, uint8_t data2 ){
	entity_data[ pos ] = type | (variant<<4) | ( data0<<8 )| ( data1<<16 )| ( data2<<24 );
}

uint32_t entity_data_get ( uint32_t pos ){
	if(pos>map_size) return printf("SUCKS %i !\n", pos);
	entity_data[ pos ];
}


void setup() {
    tv_clear( 1 );
    scale_set( 2 );
    fps_set( 1 );
    map_width = WIDTH/8;
    map_height = HEIGHT/8;
    map_size = map_width * map_height;
    tile_data = (uint8_t*) malloc(sizeof(uint8_t)*map_size);
    entity_data = (uint32_t*) malloc(sizeof(uint32_t)*map_size);

	seed = 0x13;
	srand( seed );

	for ( map_y = 0; map_y < map_height; map_y++ ) {
		for ( map_x = 0; map_x < map_width; map_x++ ) {
			uint8_t tile_foreground = ( (uint8_t)(perlin2d( map_x, map_y, .07f, 12, seed )*12) );
			tile_data[map_x+map_y*map_width] = tile_foreground;
			uint8_t tile_background = 0;

			if( rand()%256 < 31 ){
				tile_background = rand()%7;
				tile_data[map_x+map_y*map_width] |= (tile_background)<<4;
			}

			if( rand()%256 < 15 ){
				entity_data_set( map_x+map_y*map_width, rand()%3, rand()%5, 0, 0, 0 );
			} else {
				entity_data[ map_x+map_y*map_width ] = 0;
			}
		}
	}

	print_center( WIDTH/2, 32, 0x3A, "BOARD_GAME_001");
	render();
	loop_sleep_ms(650);

}

uint32_t pos_from_dir( uint32_t srcPos, uint8_t dir ){
	int xx = map_x;
	int yy = map_y;
	if( dir==0 ){ yy -= 1; }
	else if( dir==2 ){ yy += 1; }
	else if( dir==1 ){ xx += 1; }
	else if( dir==3 ){ xx -= 1; }
	if(xx<0) xx += map_width;
	if(yy<0) yy += map_width;
	return (xx%(map_width)) + (yy%(map_height))*map_width;
}

uint32_t move( uint32_t srcPos, uint8_t dir ){
	uint32_t trgPos = pos_from_dir( srcPos, dir );
	printf("%i - %i \n", srcPos, pos_from_dir( srcPos, dir ));
	if ( entity_data_get( trgPos )>0 ) return false; // return false if target is occupied
	//if ( entity_data_get( trgPos )!=0 ) return false; // return false if target is occupied
	entity_data[ trgPos ] = entity_data_get( srcPos );
	entity_data[ srcPos ] = 0;
	printf("move %i - %i # %i - %i \n", srcPos, trgPos, entity_data[ trgPos ], entity_data[ srcPos ]  );
	return true;
}

void plant_action( uint16_t pos, uint8_t variant, uint8_t entityData0, uint8_t entityData1, uint8_t entityData2){
	variant %= 5;

	uint8_t rnd_grow_chance = rand()&0xFF;
	if( rnd_grow_chance<8 ){
		if( entityData0<5 ){ entityData0++; }
		else { entityData0 = 5; }
	}

	spriteManager_draw( spriteManager_get( 70+variant ), vect2_new( map_x*8, map_y*8 ), entityData0, true  );
	entity_data_set( pos, 0, variant, entityData0, entityData1, entityData2);
}

void animal_action( uint16_t pos, uint8_t variant, uint8_t entityData0, uint8_t entityData1, uint8_t entityData2){
	//variant %= 16;
	if( entityData0<3 ){ entityData0++; }
	else { entityData0 = 0; }

	move( pos, 0 );

	spriteManager_draw( spriteManager_get( variant ), vect2_new( map_x*8, map_y*8 ), entityData0, false );
	entity_data_set( pos, 1, variant, entityData0, entityData1, entityData2);
}

void worker_action( uint16_t pos, uint8_t variant, uint8_t entityData0, uint8_t entityData1, uint8_t entityData2){
	variant %= 6;
	if( entityData0<3 ){ entityData0++; }
	else { entityData0 = 0; }

	//if( rand()&3 == 1 ){
		move( pos, rand()&3 );
		//return;
	//}

	spriteManager_draw( spriteManager_get( 78+variant ), vect2_new( map_x*8, map_y*8 ), entityData0, false );
	entity_data_set( pos, 2, variant, entityData0, entityData1, entityData2);
}

void loop() {

    for ( map_y = 0; map_y < map_height; map_y++ ) {
		for ( map_x = 0; map_x < map_width; map_x++ ) {
			uint16_t pos = map_x+map_y*map_width;
			uint8_t tile_data_value = tile_data[ pos ]&0xF;
			uint8_t tile_deco_value = (tile_data[ pos ]<<4)&0xFF;
			spriteManager_draw( spriteManager_get( 58+(tile_data_value&0xf) ), vect2_new( map_x*8, map_y*8 ), 0, true );
			if( tile_deco_value>0 ){
				//spriteManager_draw( spriteManager_get( 75 ), vect2_new( map_x*8, map_y*8 ), tile_deco_value, true );
			}
			uint32_t entityData = entity_data[ pos ];
			if( entityData==0 ) continue;

			uint8_t entityType = entityData&0xF;
			uint8_t entityVariant = (entityData>>4)&0xF;
			uint8_t entityData0 = (entityData>>8)&0xFF;
			uint8_t entityData1 = (entityData>>16)&0xFF;
			uint8_t entityData2 = (entityData>>24)&0xFF;

			if( entityType == 0 ){
				plant_action( pos, entityVariant, entityData0, entityData1, entityData2);
			} else if( entityType == 1 ){
				animal_action( pos, entityVariant, entityData0, entityData1, entityData2);
			} else if( entityType == 2 ){
				worker_action( pos, entityVariant, entityData0, entityData1, entityData2);
			}

		}
	}
}
