typedef struct {
  uint32_t id;
  Vect2 pos;
  Vect2 vel;
} entity;

void entity_move( entity* this ){
  this->pos.x += this->vel.x;
  this->pos.y += this->vel.y;
};

entity entity_new( uint32_t id, Vect2 pos, Vect2 vel ){
  return (entity) { .id=id, .pos=pos, .vel=vel};
}
