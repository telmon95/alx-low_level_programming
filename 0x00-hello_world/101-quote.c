#include <stdio.h>
#include <unistd.h>
/**
 * main: Entry point
 *
 * Description: This program demonstrates the usage of the write system call to print a message to the standard error.
 * Return: Always 0 (Success)
 */
int main(void)
{
write(2, "and that piece of art is useful\" - Dora Korpar, 2015-10-19\n",
sizeof("and that piece of art is useful\" - Dora Korpar, 2015-10-19"));
return (1);
}
