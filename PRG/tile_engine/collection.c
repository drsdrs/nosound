#include "collection.h"
#include <stdio.h>
#include <stdlib.h>

void initialize_collection(EntityCollection *collection) {
    collection->size = 0;
    collection->capacity = 10;  // Initial capacity
    collection->data = (Entity *)malloc(collection->capacity * sizeof(Entity));
}

void entity_add(EntityCollection *collection, Entity item) {
    if (collection->size >= collection->capacity) {
        collection->capacity *= 2;
        collection->data = (Entity *)realloc(collection->data, collection->capacity * sizeof(Entity));
    }
    collection->data[collection->size++] = item;
    //printf("Entity %d added successfully.\n", item.pos);
}

Entity *entity_from_index( EntityCollection *collection, int index ){
	//if( collection->data[index].pos<0 ) return Entity{};
	return &collection->data[index];
}

int entity_index_from_pos( EntityCollection *collection, uint32_t pos ){ // returns -1 on NOT found
    for (int i = 0; i < collection->size; ++i) {
        if (collection->data[i].pos == pos) {
            return i;
        }
    }
    return -1;
}

Entity entity_create( uint32_t pos, uint8_t type, uint8_t variation, uint8_t data0, uint8_t data1, uint8_t data2 ){
	Entity e = { pos, type, variation, data0, data1, data2 };
	return e;
}

void remove_entity(EntityCollection *collection, Entity *item) {
    int found = 0;
    for (int i = 0; i < collection->size; ++i) {
        if (collection->data[i].pos == item->pos) {
            found = 1;
            for (int j = i; j < collection->size - 1; ++j) {
                collection->data[j] = collection->data[j + 1];
            }
            --collection->size;
            printf("Entity %d removed successfully.\n", item->pos);
            break;
        }
    }
    if (!found) {
        printf("Error: Entity not found in the collection.\n");
        exit(1);
    }
}


void display_collection(EntityCollection *collection) {
    printf("Collection: ");
    for (int i = 0; i < collection->size; ++i) {
        printf("%d ", collection->data[i].pos);
    }
    printf("\n");
}

void cleanup_collection(EntityCollection *collection) {
    free(collection->data);
    collection->data = NULL;
}
