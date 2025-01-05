// wagon.h
#ifndef WAGON_H
#define WAGON_H
#include "material.h"

typedef struct Wagon
{
    int wagon_id;                     // Unique ID for the wagon
    float max_weight;                 // Maximum weight capacity of the wagon
    float current_weight;             // Current weight of the wagon
    LoadedMaterial *loaded_materials; // Linked list of loaded materials
    struct Wagon *next;               // Next wagon in the train
    struct Wagon *prev;               // Previous wagon in the train
} Wagon;

Wagon *create_wagon(int id);

#endif
