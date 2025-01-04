#include "material.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void add_material_to_list(MaterialList *list, Material *material)
{
    if (!list->head)
    {
        list->head = material;
    }
    else
    {
        Material *current = list->head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = material;
    }
}

void display_material_status(const MaterialList *list)
{
    if (!list->head)
    {
        printf("No materials available.\n");
        return;
    }

    Material *current = list->head;
    while (current)
    {
        printf("Material: %s, Weight: %.2f, Quantity: %d\n",
               current->name, current->weight, current->quantity);
        current = current->next;
    }
}

void initialize_material_list(MaterialList *list)
{
    list->head = NULL;
}

void free_material_list(MaterialList *list)
{
    Material *current = list->head;
    while (current)
    {
        Material *next = current->next;
        free(current);
        current = next;
    }
}