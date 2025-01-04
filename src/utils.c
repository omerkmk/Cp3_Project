#include "utils.h"
#include <stdio.h>

void print_error(const char *message)
{
    printf("Error: %s\n", message);
}

void validate_input(int input, int min, int max)
{
    if (input < min || input > max)
    {
        printf("Invalid choice. Please enter a number between %d and %d.\n", min, max);
    }
}
