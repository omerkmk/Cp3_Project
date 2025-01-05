#include "train.h"
#include <stdlib.h>
#include <stdio.h>
#define MAX_WAGONS 24

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

void load_material_to_train(Train *train, MaterialList *materials)
{
    Material *current_material = materials->head;
    Wagon *current_wagon = NULL;

    if (!current_material)
    {
        printf("Error: No materials to load.\n");
        return;
    }

    // Sadece 1 adet malzemeyi yükle
    printf("Processing material: %s, Quantity: %d, Weight: %.2f\n",
           current_material->name, current_material->quantity, current_material->weight);

    // Eğer 1 adet malzeme yüklenecekse
    if (current_material->quantity > 0)
    {
        // Önce mevcut vagonda boşluk var mı diye kontrol et
        current_wagon = train->first_wagon;

        // Vagonda boşluk var mı kontrol et
        while (current_wagon)
        {
            if (current_wagon->current_weight + current_material->weight <= current_wagon->max_weight)
            {
                // Vagon dolu değilse, malzeme yükle
                current_wagon->current_weight += current_material->weight;
                current_material->quantity--;
                current_material->loaded++;

                // Yeni bir LoadedMaterial oluşturup vagonun loaded_materials listesine ekleyin
                LoadedMaterial *new_loaded_material = (LoadedMaterial *)malloc(sizeof(LoadedMaterial));
                new_loaded_material->type = current_material;
                new_loaded_material->next = NULL;

                // Eğer vagonun loaded_materials listesi boşsa, yeni malzemeyi başa ekleyin
                if (current_wagon->loaded_materials == NULL)
                {
                    current_wagon->loaded_materials = new_loaded_material;
                    new_loaded_material->prev = NULL;
                }
                else
                {
                    // Eğer doluysa, listenin sonuna ekleyin
                    LoadedMaterial *last = current_wagon->loaded_materials;
                    while (last->next != NULL)
                    {
                        last = last->next;
                    }
                    last->next = new_loaded_material;
                    new_loaded_material->prev = last;
                }

                printf("Loaded %s into Wagon %d. Current weight: %.2f/%.2f\n",
                       current_material->name, current_wagon->wagon_id,
                       current_wagon->current_weight, current_wagon->max_weight);
                return; // Yalnızca 1 malzeme yüklendikten sonra işlemi sonlandır
            }
            current_wagon = current_wagon->next; // Sonraki vagona geç
        }

        // Eğer mevcut vagonlarda boşluk yoksa yeni vagon ekle
        if (train->wagon_count >= MAX_WAGONS)
        {
            printf("Warning: Train is full. Remaining materials cannot be loaded.\n");
            return;
        }

        // Yeni bir vagon ekle
        int new_wagon_id = train->wagon_count + 1;
        current_wagon = create_wagon(new_wagon_id);
        add_wagon_to_train(train, current_wagon);
        printf("Added Wagon %d to train.\n", current_wagon->wagon_id);

        // Yeni vagona malzeme yükle
        if (current_wagon->current_weight + current_material->weight <= current_wagon->max_weight)
        {
            current_wagon->current_weight += current_material->weight;
            current_material->quantity--;
            current_material->loaded++;

            // Yeni bir LoadedMaterial oluşturup vagonun loaded_materials listesine ekleyin
            LoadedMaterial *new_loaded_material = (LoadedMaterial *)malloc(sizeof(LoadedMaterial));
            new_loaded_material->type = current_material;
            new_loaded_material->next = NULL;

            if (current_wagon->loaded_materials == NULL)
            {
                current_wagon->loaded_materials = new_loaded_material;
                new_loaded_material->prev = NULL;
            }
            else
            {
                LoadedMaterial *last = current_wagon->loaded_materials;
                while (last->next != NULL)
                {
                    last = last->next;
                }
                last->next = new_loaded_material;
                new_loaded_material->prev = last;
            }

            printf("Loaded %s into Wagon %d. Current weight: %.2f/%.2f\n",
                   current_material->name, current_wagon->wagon_id,
                   current_wagon->current_weight, current_wagon->max_weight);
        }
    }

    printf("No materials left to load.\n");
}

void load_material_to_specific_wagon(Train *train, MaterialList *materials)
{
    int wagon_id;
    printf("Enter Wagon ID to load materials: ");
    scanf("%d", &wagon_id);

    // Vagon ID'si, maksimum 25 olmalı
    if (wagon_id < 1 || wagon_id > MAX_WAGONS)
    {
        printf("Invalid wagon ID. Please enter a number between 1 and 25.\n");
        return;
    }

    // Eğer trenin vagonu yoksa, belirtilen vagon ID'ye kadar yeni vagolar ekle
    while (train->wagon_count < wagon_id)
    {
        int new_wagon_id = train->wagon_count + 1;
        Wagon *new_wagon = create_wagon(new_wagon_id);
        add_wagon_to_train(train, new_wagon);
        printf("Added Wagon %d to train.\n", new_wagon->wagon_id);
    }

    // Belirtilen vagona ulaşmak
    Wagon *wagon = train->first_wagon;
    while (wagon && wagon->wagon_id != wagon_id)
    {
        wagon = wagon->next;
    }

    if (!wagon)
    {
        printf("No wagon with ID %d found.\n", wagon_id);
        return;
    }

    // Vagon bulundu, malzeme yükleme işlemi başlat
    Material *current_material = materials->head;

    // Malzeme sırasıyla yükleme işlemi
    while (current_material)
    {
        if (current_material->quantity > 0 &&
            wagon->current_weight + current_material->weight <= wagon->max_weight)
        {
            // Vagonun kapasitesine göre malzeme yükle
            wagon->current_weight += current_material->weight;
            current_material->quantity--;

            // Yeni bir LoadedMaterial oluştur ve bunu vagonun loaded_materials listesine ekle
            LoadedMaterial *new_loaded_material = (LoadedMaterial *)malloc(sizeof(LoadedMaterial));
            new_loaded_material->type = current_material;
            new_loaded_material->next = NULL;

            // Eğer vagonun loaded_materials listesi boşsa, yeni malzemeyi başa ekleyin
            if (wagon->loaded_materials == NULL)
            {
                wagon->loaded_materials = new_loaded_material;
                new_loaded_material->prev = NULL;
            }
            else
            {
                // Eğer doluysa, listenin sonuna ekleyin
                LoadedMaterial *last = wagon->loaded_materials;
                while (last->next != NULL)
                {
                    last = last->next;
                }
                last->next = new_loaded_material;
                new_loaded_material->prev = last;
            }

            printf("Loaded %s into Wagon %d. Current weight: %.2f/%.2f\n",
                   current_material->name, wagon->wagon_id,
                   wagon->current_weight, wagon->max_weight);
            return; // Bir malzeme yüklendikten sonra işlemi sonlandır
        }
        current_material = current_material->next; // Sonraki malzemeye geç
    }

    // Vagon kapasitesine uygun malzeme yoksa
    printf("No suitable materials to load into Wagon %d.\n", wagon_id);
}

void unload_material_from_train(Train *train)
{
    // Eğer trenin hiç vagonu yoksa
    if (train->first_wagon == NULL)
    {
        printf("Train is already empty.\n");
        return;
    }

    // Son vagondan başla
    Wagon *wagon = train->first_wagon;

    // Son vagona git
    while (wagon && wagon->next)
    {
        wagon = wagon->next;
    }

    // Son vagonda malzeme boşaltma işlemi
    while (wagon)
    {
        printf("Checking wagon %d\n", wagon->wagon_id); // Debug mesajı
        printf("Current weight: %.2f / %.2f\n", wagon->current_weight, wagon->max_weight);

        // Eğer vagonun mevcut yükü varsa (yani vagon dolmuşsa) bir malzeme boşalt
        if (wagon->current_weight > 0)
        {
            printf("Vagon yükü var, malzeme boşaltılabilir.\n");

            // Son eklenen malzemeyi boşaltıyoruz
            LoadedMaterial *last_loaded_material = wagon->loaded_materials;

            // Eğer vagonun malzeme listesi boşsa
            if (last_loaded_material == NULL)
            {
                printf("Loaded materials list is empty in Wagon %d.\n", wagon->wagon_id);
                break; // Malzeme yoksa, işlem yapmadan çık
            }

            // Son malzemeye kadar git
            while (last_loaded_material && last_loaded_material->next != NULL)
            {
                last_loaded_material = last_loaded_material->next;
            }

            // Eğer son malzeme varsa
            if (last_loaded_material && last_loaded_material->type)
            {
                printf("Son malzeme bulundu: %s\n", last_loaded_material->type->name);

                // Vagonun ağırlığını güncelliyoruz
                wagon->current_weight -= last_loaded_material->type->weight; // Malzemenin ağırlığını düşürüyoruz

                // Malzemenin miktarını artırıyoruz
                last_loaded_material->type->quantity++;

                printf("Unloaded 1 %s from Wagon %d. Current weight: %.2f/%.2f\n",
                       last_loaded_material->type->name, wagon->wagon_id,
                       wagon->current_weight, wagon->max_weight);

                // Son malzemeyi listeden çıkarıyoruz (boşaltıldı)
                if (last_loaded_material->prev)
                {
                    last_loaded_material->prev->next = NULL; // Listeyi sonlandır
                }
                else
                {
                    // Eğer yalnızca 1 malzeme varsa
                    wagon->loaded_materials = NULL;
                }

                // Hafızayı serbest bırak
                free(last_loaded_material);

                // Eğer vagon boşsa, vagonu kaldırmak isteyebilirsiniz
                if (wagon->loaded_materials == NULL)
                {
                    printf("No materials left in Wagon %d. Removing wagon.\n", wagon->wagon_id);
                    // Vagonu trenin başından kaldırıyoruz
                    if (wagon->prev)
                        wagon->prev->next = wagon->next;
                    if (wagon->next)
                        wagon->next->prev = wagon->prev;

                    // Eğer ilk vagonsa
                    if (train->first_wagon == wagon)
                        train->first_wagon = wagon->next;

                    free(wagon);          // Vagonu serbest bırak
                    train->wagon_count--; // Vagon sayısını azalt
                    break;                // İşlem tamamlandı
                }

                break; // Yalnızca bir malzeme boşaltılacak
            }
            else
            {
                printf("No valid materials to unload in Wagon %d.\n", wagon->wagon_id);
            }
        }
        else
        {
            printf("No materials to unload in Wagon %d.\n", wagon->wagon_id);
        }

        // Bir önceki vagona geç
        wagon = wagon->prev;
    }

    // Tren ve vagonların durumunu yazdır
    printf("\n--- Train Status After Unloading ---\n");
    display_train_status(train);
    printf("-----------------------------------\n");

    printf("One material has been unloaded from the train.\n");
}

void unload_material_from_specific_wagon(Train *train)
{
    if (!train->first_wagon)
    {
        printf("Train is empty. Cannot unload specific wagon.\n");
        return;
    }

    int wagon_id;
    printf("Enter Wagon ID to unload materials: ");
    scanf("%d", &wagon_id);

    Wagon *wagon = train->first_wagon;

    // Find the wagon with the given ID
    while (wagon && wagon->wagon_id != wagon_id)
    {
        wagon = wagon->next;
    }

    if (!wagon)
    {
        printf("No wagon with ID %d found.\n", wagon_id);
        return;
    }

    // If the wagon has loaded materials, unload the last one
    if (wagon->loaded_materials)
    {
        LoadedMaterial *last_loaded_material = wagon->loaded_materials;

        // Traverse to the last loaded material
        while (last_loaded_material->next)
        {
            last_loaded_material = last_loaded_material->next;
        }

        // Unload the last material
        wagon->current_weight -= last_loaded_material->type->weight; // Reduce the weight
        last_loaded_material->type->quantity++;                      // Increase the material quantity

        printf("Unloaded 1 %s from Wagon %d. Current weight: %.2f/%.2f\n",
               last_loaded_material->type->name, wagon->wagon_id,
               wagon->current_weight, wagon->max_weight);

        // Remove the last loaded material from the wagon
        if (last_loaded_material->prev)
        {
            last_loaded_material->prev->next = NULL; // Disconnect the last material
        }
        else
        {
            // If there is only one material, set the loaded materials list to NULL
            wagon->loaded_materials = NULL;
        }

        // Free the memory for the last loaded material
        free(last_loaded_material);

        // If the wagon is empty, remove it from the train
        if (wagon->loaded_materials == NULL)
        {
            printf("No materials left in Wagon %d. Removing wagon.\n", wagon->wagon_id);

            // Remove the wagon from the train list
            if (wagon->prev)
                wagon->prev->next = wagon->next;
            if (wagon->next)
                wagon->next->prev = wagon->prev;

            // If it's the first wagon, update the train's first wagon
            if (train->first_wagon == wagon)
                train->first_wagon = wagon->next;

            // Free the wagon memory and decrement the wagon count
            free(wagon);
            train->wagon_count--;
            printf("Removed Wagon %d from train.\n", wagon_id);
        }
    }
    else
    {
        printf("No materials to unload in Wagon %d.\n", wagon_id);
    }
}

void display_train_status(const Train *train)
{
    Wagon *wagon = train->first_wagon;

    if (!wagon)
    {
        printf("Train is empty.\n");
        return;
    }

    printf("Train Status:\n");
    while (wagon)
    {
        printf("Wagon ID: %d, Current Weight: %.2f/%.2f\n",
               wagon->wagon_id, wagon->current_weight, wagon->max_weight);
        wagon = wagon->next;
    }
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

void add_wagon_to_train(Train *train, Wagon *wagon)
{
    // Maksimum vagon sınırını kontrol et
    if (train->wagon_count >= MAX_WAGONS)
    {
        printf("Error: Cannot add more wagons. Maximum limit of %d reached.\n", MAX_WAGONS);
        free(wagon); // Yeni vagonu serbest bırak (eklenemedi)
        return;
    }

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
    printf("Added Wagon %d to train.\n", wagon->wagon_id);
}
