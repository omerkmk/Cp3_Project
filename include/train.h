#ifndef TRAIN_H
#define TRAIN_H

#include "wagon.h"
#include "material.h"

typedef struct
{
    char train_id[20];
    Wagon *first_wagon;
    int wagon_count;
    MaterialList materials;
} Train;

void add_wagon_to_train(Train *train, Wagon *wagon);
void initialize_train(Train *train);
void free_train(Train *train);
void load_material_to_train(Train *train, MaterialList *materials);
void load_material_to_specific_wagon(Train *train, MaterialList *materials);
void unload_material_from_train(Train *train);
void unload_material_from_specific_wagon(Train *train);
void display_train_status(const Train *train);
void empty_train(Train *train);

#endif
