// material.h
#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct Material
{
    char name[50];
    float weight;
    int quantity;
    int loaded;
    struct Material *next;
} Material;

typedef struct LoadedMaterial
{
    Material *type;              // Pointer to the material being loaded
    struct LoadedMaterial *next; // Next loaded material in the list
    struct LoadedMaterial *prev; // Previous loaded material in the list
} LoadedMaterial;

typedef struct MaterialList
{
    Material *head; // Head of the material list
} MaterialList;

void add_material_to_list(MaterialList *list, Material *material);
void initialize_material_list(MaterialList *list);
void free_material_list(MaterialList *list);
void load_material_from_file(MaterialList *materials, const char *filename);
void display_material_status(const MaterialList *list);

#endif
