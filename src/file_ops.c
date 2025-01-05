#include "file_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_WAGONS 24

void load_train_from_file(Train *train, const char *filename, MaterialList *materials)
{
    free_train(train);
    initialize_train(train);

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Could not open file '%s'.\n", filename);
        return;
    }

    char line[256];
    int loaded_wagon_count = 0;

    // Dosya işleme ve mevcut vagonların eklenmesi
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "TRAIN:", 6) == 0)
        {
            sscanf(line, "TRAIN:%[^:]:%d", train->train_id, &train->wagon_count);
        }
        else if (strncmp(line, "M:", 2) == 0)
        {
            Material *material = malloc(sizeof(Material));
            sscanf(line, "M:%[^:]:%*[^:]:%f:%d:%d",
                   material->name, &material->weight,
                   &material->quantity, &material->loaded);
            material->next = NULL;
            add_material_to_list(&train->materials, material);
        }
    }

    fclose(file);

    // Malzeme yükleme
    Material *current_material = train->materials.head;
    Wagon *current_wagon = NULL;

    while (current_material)
    {
        while (current_material->quantity > 0)
        {
            printf("Processing material: %s, Quantity: %d\n", current_material->name, current_material->quantity);

            // Eğer mevcut vagon yoksa veya dolmuşsa yeni bir vagon oluştur
            if (!current_wagon || current_wagon->current_weight >= current_wagon->max_weight)
            {
                if (loaded_wagon_count >= MAX_WAGONS)
                {
                    printf("Warning: Train is full. Remaining materials cannot be loaded.\n");
                    return;
                }

                current_wagon = create_wagon(loaded_wagon_count + 1);
                add_wagon_to_train(train, current_wagon);
                loaded_wagon_count++;
            }

            // Mevcut malzeme vagona sığabiliyorsa yükle
            if (current_wagon->current_weight + current_material->weight <= current_wagon->max_weight)
            {
                // Create a LoadedMaterial node and add it to the loaded_materials list of the wagon
                LoadedMaterial *new_loaded_material = malloc(sizeof(LoadedMaterial));
                new_loaded_material->type = current_material;
                new_loaded_material->next = NULL;
                new_loaded_material->prev = NULL;

                // Add the material to the wagon's loaded_materials list
                if (current_wagon->loaded_materials == NULL)
                {
                    current_wagon->loaded_materials = new_loaded_material;
                }
                else
                {
                    LoadedMaterial *last = current_wagon->loaded_materials;
                    while (last->next)
                    {
                        last = last->next;
                    }
                    last->next = new_loaded_material;
                    new_loaded_material->prev = last;
                }

                // Update the wagon's weight, decrement the material quantity and increment loaded count
                current_wagon->current_weight += current_material->weight;
                current_material->quantity--; // Decrease quantity after loading
                current_material->loaded++;

                printf("Loaded %s into Wagon %d. Current weight: %.2f/%.2f\n",
                       current_material->name, current_wagon->wagon_id,
                       current_wagon->current_weight, current_wagon->max_weight);
            }
            else
            {
                // Handle smaller materials if needed
                Material *smaller_material = current_material->next;
                while (smaller_material)
                {
                    if (smaller_material->quantity > 0 &&
                        current_wagon->current_weight + smaller_material->weight <= current_wagon->max_weight)
                    {
                        LoadedMaterial *new_loaded_material = malloc(sizeof(LoadedMaterial));
                        new_loaded_material->type = smaller_material;
                        new_loaded_material->next = NULL;
                        new_loaded_material->prev = NULL;

                        if (current_wagon->loaded_materials == NULL)
                        {
                            current_wagon->loaded_materials = new_loaded_material;
                        }
                        else
                        {
                            LoadedMaterial *last = current_wagon->loaded_materials;
                            while (last->next)
                            {
                                last = last->next;
                            }
                            last->next = new_loaded_material;
                            new_loaded_material->prev = last;
                        }

                        current_wagon->current_weight += smaller_material->weight;
                        smaller_material->quantity--; // Decrease quantity for smaller materials
                        smaller_material->loaded++;

                        printf("Loaded %s into Wagon %d (filling gap). Current weight: %.2f/%.2f\n",
                               smaller_material->name, current_wagon->wagon_id,
                               current_wagon->current_weight, current_wagon->max_weight);
                    }
                    smaller_material = smaller_material->next;
                }

                current_wagon = NULL;
            }
        }

        // Bu malzeme tamamen yüklendikten sonra sıradaki malzemeye geç
        current_material = current_material->next;
    }

    printf("Train and materials loaded successfully from '%s'.\n", filename);
}

void save_train_to_file(const Train *train, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
        printf("Error: Could not open file '%s' for writing.\n", filename);
        return;
    }

    // Save the train ID and wagon count
    fprintf(file, "TRAIN:%s:%d\n", train->train_id, train->wagon_count);

    // Save the materials
    Material *current_material = train->materials.head;
    while (current_material)
    {
        fprintf(file, "M:%s:%.2f:%d:%d\n", current_material->name,
                current_material->weight, current_material->quantity,
                current_material->loaded);
        current_material = current_material->next;
    }

    // Save the wagons and their contents
    Wagon *wagon = train->first_wagon;
    while (wagon)
    {
        // For each wagon, save its ID and current weight
        fprintf(file, "WAGON:%d:%.2f\n", wagon->wagon_id, wagon->current_weight);

        // Save the loaded materials in the current wagon
        LoadedMaterial *loaded_material = wagon->loaded_materials;
        while (loaded_material)
        {
            // For each loaded material in the wagon, save its information
            fprintf(file, "LOADED_MATERIAL:%s:%.2f\n", loaded_material->type->name,
                    loaded_material->type->weight);
            loaded_material = loaded_material->next;
        }

        wagon = wagon->next;
    }

    fclose(file);
    printf("Train data saved successfully to '%s'.\n", filename);
}