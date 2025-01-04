#include "train.h"
#include <stdio.h>
#include <stdlib.h>

void add_wagon_to_train(Train *train, Wagon *wagon)
{
    if (!train->first_wagon)
    {
        train->first_wagon = wagon;
    }
    else
    {
        Wagon *current = train->first_wagon;
        while (current->next)
        {
            current = current->next;
        }
        current->next = wagon;
        wagon->prev = current;
    }
    train->wagon_count++;
    printf("Wagon %d added to train.\n", wagon->wagon_id);
}

void empty_train(Train *train)
{
    Wagon *current = train->first_wagon;
    while (current)
    {
        Wagon *next = current->next;
        free(current);
        current = next;
    }
    train->first_wagon = NULL;
    train->wagon_count = 0;
    printf("Train has been emptied.\n");
}

void display_train_status(const Train *train)
{
    if (!train->first_wagon)
    {
        printf("Train is empty.\n");
        return;
    }

    Wagon *current = train->first_wagon;
    while (current)
    {
        printf("Wagon ID: %d, Current Weight: %.2f/%.2f\n",
               current->wagon_id, current->current_weight, current->max_weight);
        current = current->next;
    }
}

void unload_material_from_specific_wagon(Train *train)
{
    printf("Unloading material from a specific wagon...\n");
}

void load_material_to_specific_wagon(Train *train, MaterialList *materials)
{
    printf("Loading material to a specific wagon...\n");
}

void load_material_to_train(Train *train, MaterialList *materials)
{
    Material *material = materials->head;
    while (material)
    {
        if (material->quantity > 0)
        {
            Wagon *wagon = train->first_wagon;
            while (wagon)
            {
                if (wagon->current_weight + material->weight <= wagon->max_weight)
                {
                    wagon->current_weight += material->weight;
                    material->quantity--;
                    printf("Loaded %s into Wagon %d.\n", material->name, wagon->wagon_id);
                    return;
                }
                wagon = wagon->next;
            }

            // Eğer uygun vagon yoksa yeni bir vagon oluştur
            Wagon *new_wagon = create_wagon(train->wagon_count++);
            add_wagon_to_train(train, new_wagon);
            printf("New wagon created.\n");
        }
        material = material->next;
    }
}

void unload_material_from_train(Train *train)
{
    Wagon *wagon = train->first_wagon;
    while (wagon)
    {
        if (wagon->current_weight > 0)
        {
            wagon->current_weight = 0; // Yükleri boşalt
            printf("Unloaded Wagon %d.\n", wagon->wagon_id);
        }
        wagon = wagon->next;
    }
}

void initialize_train(Train *train)
{
    train->first_wagon = NULL;
    train->wagon_count = 0;
    initialize_material_list(&train->materials);
}

void free_train(Train *train)
{
    Wagon *current = train->first_wagon;
    while (current)
    {
        Wagon *next = current->next;
        free(current);
        current = next;
    }

    free_material_list(&train->materials);
    train->first_wagon = NULL;
    train->wagon_count = 0;
}