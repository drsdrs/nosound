#ifndef ENTITY_COLLECTION_H
#define ENTITY_COLLECTION_H

typedef struct {
	uint32_t pos;
	int8_t type;
	uint8_t variation;
	uint8_t data0;
	uint8_t data1;
	uint8_t data2;
} Entity;

// Structure to represent the collection
typedef struct {
    Entity *data;
    int size;
    int capacity;
} EntityCollection;

// Function declarations
void initialize_collection(EntityCollection *collection);
void entity_add(EntityCollection *collection, Entity item);
void remove_entity(EntityCollection *collection, Entity *item);
void display_collection(EntityCollection *collection);
void cleanup_collection(EntityCollection *collection);

#endif /* ENTITY_COLLECTION_H */
