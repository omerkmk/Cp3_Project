// wagon.c
#include "wagon.h"
#include <stdlib.h>

Wagon *create_wagon(int id)
{
    Wagon *new_wagon = (Wagon *)malloc(sizeof(Wagon));
    new_wagon->wagon_id = id;
    new_wagon->max_weight = 1000.0;
    new_wagon->current_weight = 0.0;
    new_wagon->loaded_materials = NULL;
    new_wagon->next = NULL;
    new_wagon->prev = NULL;
    return new_wagon;
}
