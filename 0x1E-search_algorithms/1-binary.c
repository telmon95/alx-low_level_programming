#include <stdio.h>
#include <stdlib.h>

/**
 * binary_search - searches for a value in a sorted array using binary search
 * @array: pointer to the first element of the array to search
 * @size: number of elements in the array
 * @value: value to search for
 *
 * Return: index of the first occurrence of value in array, or -1 if not found
 */
int binary_search(int *array, size_t size, int value)
{
    size_t left = 0; /* Variable to hold the leftmost index of the array */
    size_t right = size - 1; /* Variable to hold the rightmost index of the array */
    size_t i; /* Variable to iterate over the subarray */
    size_t mid; /* Variable to hold the middle index of the array */

    if (array == NULL) /* Check if array is NULL */
        return (-1); /* If so, return -1 as value not found */

    while (left <= right) /* Perform binary search */
    {
        mid = (left + right) / 2; /* Calculate the middle index */

        printf("Searching in array: "); /* Print the array being searched */

        /* Print the current subarray being searched */
        for (i = left; i <= right; i++)
        {
            printf("%d", array[i]);
            if (i < right)
                printf(", ");
        }
        printf("\n");

        if (array[mid] == value) /* If the value is found */
            return (mid); /* Return the index of the value */
        else if (array[mid] < value) /* If value is greater, search the right subarray */
            left = mid + 1;
        else /* If value is smaller, search the left subarray */
            right = mid - 1;
    }

    return (-1); /* Value not found, return -1 */
}

