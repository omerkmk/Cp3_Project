#ifndef MATERIAL_H
#define MATERIAL_H

typedef struct Material
{
    char name[50];
    float weight;
    int quantity;
    struct Material *next;
    int loaded;
} Material;

typedef struct MaterialList
{
    Material *head;
} MaterialList;

void add_material_to_list(MaterialList *list, Material *material);
void initialize_material_list(MaterialList *list);
void free_material_list(MaterialList *list);
void display_material_status(const MaterialList *list);

#endif
