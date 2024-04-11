#include <stdio.h>
#include <stdlib.h>

/**
 * linear_search - searches for a value in an array using linear search algorithm
 * @array: pointer to the first element of the array to search
 * @size: number of elements in the array
 * @value: value to search for
 *
 * Return: index of the first occurrence of value in array, or -1 if not found
 */
int linear_search(int *array, size_t size, int value)
{
    size_t i; /* Variable to iterate over the array */

    if (array == NULL) /* Check if array is NULL */
        return (-1); /* If so, return -1 as value not found */

    for (i = 0; i < size; i++) /* Iterate over each element of the array */
    {
        printf("Value checked array[%lu] = [%d]\n", i, array[i]); /* Print the value being checked */

        if (array[i] == value) /* If the value is found */
            return (i); /* Return the index of the value */
    }

    return (-1); /* Value not found, return -1 */
}

