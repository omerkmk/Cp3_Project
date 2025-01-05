#include <stdio.h>
#include <stdlib.h>
#include "train.h"
#include "wagon.h"
#include "material.h"
#include "file_ops.h"
#include "utils.h"
#include <string.h>

void display_menu()
{
    printf("=== Train Loading Management System ===\n");
    printf("1. Load train status from file\n");
    printf("2. Load material, starting from first suitable wagon from head of the Train\n");
    printf("3. Load material to specific wagon\n");
    printf("4. Unload material, starting from first suitable wagon from tail of the Train\n");
    printf("5. Unload material from specific wagon\n");
    printf("6. Display train status\n");
    printf("7. Display materials status\n");
    printf("8. Empty train\n");
    printf("9. Save train status to file\n");
    printf("10. Exit\n");
}

void load_material_from_file(MaterialList *materials, const char *filename)
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
        // "M:" ile başlayan satırları işle
        if (strncmp(line, "M:", 2) == 0)
        {
            Material *material = malloc(sizeof(Material));
            if (material == NULL)
            {
                printf("Error: Memory allocation failed.\n");
                fclose(file);
                return;
            }

            // "M:Large Box:Steel:200.00:50:0" gibi bir satırdan veriyi al
            int matched = sscanf(line, "M:%[^:]:%*[^:]:%f:%d:%d", material->name, &material->weight, &material->quantity, &material->loaded);

            if (matched != 4) // Eğer veriler eksikse hata mesajı ver
            {
                printf("Error: Invalid data format in line: %s", line);
                free(material);
                continue;
            }

            material->next = NULL;

            // Malzemeyi listeye ekle
            add_material_to_list(materials, material);
            printf("Loaded material: %s, Quantity: %d\n", material->name, material->quantity);
        }
    }

    fclose(file);
}

void add_sample_materials(MaterialList *materials)
{
    // Örnek malzemeler ekle
    Material *large_box = malloc(sizeof(Material));
    strcpy(large_box->name, "Large Box");
    large_box->weight = 200.0;
    large_box->quantity = 50;
    large_box->loaded = 0;
    large_box->next = NULL;

    Material *medium_box = malloc(sizeof(Material));
    strcpy(medium_box->name, "Medium Box");
    medium_box->weight = 150.0;
    medium_box->quantity = 50;
    medium_box->loaded = 0;
    medium_box->next = NULL;

    Material *small_box = malloc(sizeof(Material));
    strcpy(small_box->name, "Small Box");
    small_box->weight = 100.0;
    small_box->quantity = 50;
    small_box->loaded = 0;
    small_box->next = NULL;

    // Malzemeleri listeye ekle
    add_material_to_list(materials, large_box);
    add_material_to_list(materials, medium_box);
    add_material_to_list(materials, small_box);
}

int main()
{
    int choice;
    Train train;
    MaterialList materials;

    initialize_train(&train);
    initialize_material_list(&materials);

    // İlk başta malzemeleri dosyadan yükle
    load_material_from_file(&materials, "train_data.txt");

    do
    {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            load_train_from_file(&train, "train_data.txt", &materials);
            break;
        case 2:
            load_material_to_train(&train, &materials);
            break;
        case 3:
            load_material_to_specific_wagon(&train, &materials);
            break;
        case 4:
            unload_material_from_train(&train);
            break;
        case 5:
            unload_material_from_specific_wagon(&train);
            break;
        case 6:
            display_train_status(&train);
            break;
        case 7:
            display_material_status(&materials);
            break;
        case 8:
            empty_train(&train);
            break;
        case 9:
            save_train_to_file(&train, "train_status.txt");
            break;
        case 10:
            printf("Exiting program...\n");
            free_train(&train);
            free_material_list(&materials);
            return 0;
        default:
            printf("Invalid choice. Try again.\n");
        }

        printf("\n--- Train Status After Operation ---\n");
        display_train_status(&train);
        printf("-----------------------------------\n");

    } while (choice != 10);

    return 0;
}
