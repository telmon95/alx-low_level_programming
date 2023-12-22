#include <stdlib.h>
#include <string.h>

char *my_strdup(const char *src) {
    size_t len = strlen(src) + 1; // +1 for the null terminator
    char *dest = malloc(len);
    if (dest != NULL) {
        memcpy(dest, src, len);
    }
    return dest;
}
