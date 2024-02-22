#include "perling.c"
#include "collection.c"


#define TYPE_PLANT 	0
#define TYPE_ANIMAL 1
#define TYPE_WORKER 2

#define ANIMAL_COW 			0
#define ANIMAL_GOAT 		1
#define ANIMAL_PIG 			2
#define ANIMAL_SHEEP 		3
#define ANIMAL_GOOSE 		4
#define ANIMAL_DUCK 		5
#define ANIMAL_CHICKEN 		6
#define ANIMAL_CHICK		7
#define ANIMAL_FOX			8
#define ANIMAL_RACOON		9
#define ANIMAL_DOG0			10
#define ANIMAL_DOG1			11
#define ANIMAL_DOG2			12
#define ANIMAL_CAT0			13
#define ANIMAL_CAT1			14
#define ANIMAL_CAT2			15

#define PLANT_TURNIP 		0
#define PLANT_POTATOE 		1
#define PLANT_WHEAT 		2
#define PLANT_CARROT 		3
#define PLANT_EGG 			4

// What eats what and how many, what is happening if certain amount
// What see's what , what is happening if seen
// action -0 look
//	SRC-ACTOR	TRG-ACTOR	ACTION/S	DEL_TRG_ACTOR
// TYPE_A
/*
TODO
 *
 *
 * add buildable tiles like WALL(blockade) and STREET(fastWalk) or HUMUS/GARDEN or HOUSE or INDUSTRIAL...... ???.-'¿¿¿
 * */

EntityCollection entity_collection;

int i,map_x,map_y = 0;
int seed = 0xdebe;

uint16_t map_width;
uint16_t map_height;
uint32_t map_size;
uint8_t* tile_data;

void setup() {
    tv_clear( 1 );
    scale_set( 2 );
    fps_set( 10 );
    map_width = WIDTH/8;
    map_height = HEIGHT/8;
    map_size = map_width * map_height;
    tile_data = (uint8_t*) malloc(sizeof(uint8_t)*map_size);

    initialize_collection(&entity_collection);

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
				entity_add( &entity_collection, entity_create( map_x+map_y*map_width, rand()%3, rand()&0xF, rand()&3, 0, 0  ) );
				//entity_set( map_x+map_y*map_width, rand()%3, rand()%5, 0, 0, 0 );
			} else {
				//entity_set( map_x+map_y*map_width ) = NULL;
			}
		}
	}

	print_center( WIDTH/2, 32, 0x3A, "BOARD_GAME_001");
	render();
	loop_sleep_ms(650);



}

uint32_t pos_from_dir( uint32_t srcPos, uint8_t dir ){
	int xx = srcPos%map_width;
	int yy = srcPos/map_width;
	if( dir==0 ){ yy -= 1; }
	else if( dir==2 ){ yy += 1; }
	else if( dir==1 ){ xx += 1; }
	else if( dir==3 ){ xx -= 1; }
	if(xx<0) xx = 0;
	if(yy<0) yy = 0;
	if(xx >= map_width) xx = map_width-1;
	if(yy >= map_height) yy = map_height-1;
	return xx + yy*map_width;
}

uint8_t eat ( uint32_t pos, uint8_t eatableType, uint8_t eatableVariantion ){
	for ( i=0; i<4; i++){
		uint32_t trgPos = pos_from_dir( pos, i );

		int e_index = entity_index_from_pos( &entity_collection, pos );
		if( e_index==-1 ) continue;
		Entity *e = entity_from_index( &entity_collection, e_index );

		//if( eatableType==type_from_entity(e) && eatableVariantion==variant_from_entity(e)  ){ // EAT
			//entity_data[ trgPos ] = 0;
		//}
	}
}

uint8_t move( Entity *srcEntity, uint8_t dir ){
	uint32_t trgPos = pos_from_dir( srcEntity->pos, dir );
	if ( entity_index_from_pos( &entity_collection, trgPos )>=0 ) return false; // return false if target is occupied
	//entity_set( trgPos ) = entity_get( e.pos );
	//printf("move %i - %i \n", srcEntity->pos, trgPos  ); // DEBUG
	srcEntity->pos = trgPos;
	return true;
}

uint8_t spread( uint32_t srcPos, uint8_t dir ){
	uint32_t trgPos = pos_from_dir( srcPos, dir );
	if ( entity_index_from_pos( &entity_collection, trgPos )>=0 ) return false; // return false if target is occupied
	int e_index = entity_index_from_pos( &entity_collection, srcPos );
	if( e_index==-1 ){ printf("ERR: should not show EVER !!!!!!!");return false;}
	Entity *e = entity_from_index( &entity_collection, e_index );
	entity_add(  &entity_collection, entity_create( trgPos, e->type, e->variation, 0,0,0 )  );
 	return true;
}

void plant_action( Entity *e ){
	e->variation %= 5;

	uint8_t rnd_grow_chance = rand()&0xFF;
	if( rnd_grow_chance<8 ){
		if( e->data0<5 ){ e->data0++; }
		else { e->data0 = 5; }

		uint8_t rnd_spread_chance = rand()&0xFF;
		if( rnd_spread_chance<8 ) spread( e->pos, rand()&3 );
	}
	//entity_set( pos, 0, e.variation, e.data0, e.data1, e.data2);
}

void animal_action( Entity *e ){
	e->variation %= 16;
	if( e->data0<3 ){
		e->data0++;
		if( e->data0==2 ) move( e, rand()&3 );
	}

	if( (rand()&0x1F)==0) e->data0 = 0;

}

void worker_action( Entity *e ){
	e->variation %= 6;
	if( e->data0<3 ){ e->data0++; }
	else { e->data0 = 0; }

	//entity_set( e.pos, 2, e.variation, e.data0, e.data1, e.data2);

	if( rand()&7 == 0 ){
		move( e, 0);//rand()&3 );
	}



}

void loop() {



	for ( i=0; i<entity_collection.size; i++) {
		Entity *e = entity_from_index( &entity_collection, i );
		if( e->type == TYPE_PLANT ){
			plant_action( e );
		} else if( e->type == TYPE_ANIMAL ){
			animal_action( e );
		} else if( e->type == TYPE_WORKER ){
			worker_action( e );
		}

	}


    for ( map_y = 0; map_y < map_height; map_y++ ) { // draw
		for ( map_x = 0; map_x < map_width; map_x++ ) {
			uint16_t pos = map_x+map_y*map_width;
			uint8_t tile_data_value = tile_data[ pos ]&0xF;
			uint8_t tile_deco_value = (tile_data[ pos ]<<4)&0xFF;
			spriteManager_draw( spriteManager_get( 58+(tile_data_value&0xf) ), vect2_new( map_x*8, map_y*8 ), 0, true );

		}
	}

	for ( i=0; i<entity_collection.size; i++) {
		Entity *e = entity_from_index( &entity_collection, i );
		int e_x = e->pos%map_width;
		int e_y = e->pos/map_width;
		if( e->type == TYPE_PLANT ){
			spriteManager_draw( spriteManager_get( 70+e->variation ), vect2_new( e_x*8, e_y*8 ), e->data0, true  );
		} else if( e->type == TYPE_ANIMAL ){
			spriteManager_draw( spriteManager_get( e->variation ), vect2_new( e_x*8, e_y*8 ), e->data0, false );
		} else if( e->type == TYPE_WORKER ){
			spriteManager_draw( spriteManager_get( 78+e->variation ), vect2_new( e_x*8, e_y*8 ), e->data0, false );
		}

	}

	print( 1, 8, 0b11111111111, "BOARD_GAME_001");


}
