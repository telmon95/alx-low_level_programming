#include <stdio.h>
#include "search_algos.h"

/**
 * advanced_binary - Searches for a value in a sorted array of integers
 * @array: Pointer to the first element of the array to search in
 * @size: Number of elements in the array
 * @value: Value to search for
 *
 * Return: Index where value is located, or -1 if not found
 */
int advanced_binary(int *array, size_t size, int value)
{
    size_t i, mid = (size - 1) / 2;
    int res;

    if (array == NULL || size == 0)
        return (-1);

    printf("Searching in array: ");
    for (i = 0; i < size; i++)
    {
        printf("%d", array[i]);
        if (i < size - 1)
            printf(", ");
    }
    printf("\n");

    if (array[mid] == value)
    {
        if (size == 1)
            return (mid);
        else if (array[mid - 1] < value)
            return (mid);
    }
    else if (array[mid] < value)
    {
        res = advanced_binary(array + mid + 1, size - mid - 1, value);
        if (res == -1)
            return (-1);
        return (res + mid + 1);
    }
    return (advanced_binary(array, mid, value));
}

