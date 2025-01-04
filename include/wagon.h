#ifndef WAGON_H
#define WAGON_H

typedef struct LoadedMaterial
{
    char material_name[50];
    float weight;
    struct LoadedMaterial *next;
} LoadedMaterial;

typedef struct Wagon
{
    int wagon_id;
    float max_weight;
    float current_weight;
    LoadedMaterial *loaded_materials;
    struct Wagon *next;
    struct Wagon *prev;
} Wagon;

Wagon *create_wagon(int id);

#endif
