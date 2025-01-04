#include "file_ops.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load_train_from_file(Train *train, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error: Could not open file '%s'.\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        if (strncmp(line, "TRAIN:", 6) == 0)
        {
            sscanf(line, "TRAIN:%*[^:]:%d", &train->wagon_count);
            initialize_train(train);
        }
        else if (strncmp(line, "MATERIALS:", 10) == 0)
        {
            continue; // Materials are loaded elsewhere
        }
        else if (strncmp(line, "M:", 2) == 0)
        {
            Material *material = malloc(sizeof(Material));
            sscanf(line, "M:%[^:]:%*[^:]:%f:%d:%*d", material->name, &material->weight, &material->quantity);
            material->next = NULL;
            add_material_to_list(&train->materials, material);
        }
        else if (strncmp(line, "WAGON:", 6) == 0)
        {
            Wagon *wagon = create_wagon(train->wagon_count++);
            sscanf(line, "WAGON:%*d:%f:%f", &wagon->max_weight, &wagon->current_weight);
            add_wagon_to_train(train, wagon);
        }
    }

    fclose(file);
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

    fprintf(file, "TRAIN:T123:%d\n", train->wagon_count);
    fprintf(file, "MATERIALS:3\n");

    Material *material = train->materials.head;
    while (material)
    {
        fprintf(file, "M:%s:%s:%.2f:%d:%d\n",
                material->name, "GenericType", material->weight, material->quantity, material->loaded);
        material = material->next;
    }

    Wagon *wagon = train->first_wagon;
    while (wagon)
    {
        fprintf(file, "WAGON:%d:%.2f:%.2f\n", wagon->wagon_id, wagon->max_weight, wagon->current_weight);
        LoadedMaterial *loaded = wagon->loaded_materials;
        while (loaded)
        {
            fprintf(file, "L:%s\n", loaded->material_name);
            loaded = loaded->next;
        }
        wagon = wagon->next;
    }

    fclose(file);
    printf("Train and materials saved successfully to '%s'.\n", filename);
}
