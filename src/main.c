#include <stdio.h>
#include <stdlib.h>
#include "train.h"
#include "wagon.h"
#include "material.h"
#include "file_ops.h"
#include "utils.h"

void display_menu()
{
    printf("=== Train Loading Management System ===\n");
    printf("1. Load train status from file\n");
    printf("2. Load material starting from first suitable wagon\n");
    printf("3. Load material to specific wagon\n");
    printf("4. Unload material starting from first suitable wagon\n");
    printf("5. Unload material from specific wagon\n");
    printf("6. Display train status\n");
    printf("7. Display materials status\n");
    printf("8. Empty train\n");
    printf("9. Save train status to file\n");
    printf("10. Exit\n");
}

int main()
{
    int choice;
    Train train;
    MaterialList materials;

    initialize_train(&train);
    initialize_material_list(&materials);

    do
    {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            load_train_from_file(&train, "train_data.txt");
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
            save_train_to_file(&train, "train_data.txt");
            break;
        case 10:
            printf("Exiting program...\n");
            break;
        default:
            printf("Invalid choice. Try again.\n");
        }
    } while (choice != 10);

    free_train(&train);
    free_material_list(&materials);

    return 0;
}