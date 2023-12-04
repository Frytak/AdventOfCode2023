#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vec.h"

int _vec_double(Vector *vec) {
    // Allocate double the capacity
    vec->cap *= 2;
    vec->data = realloc(vec->data, vec->cap * vec->size);

    if (vec->data == NULL) { return 1; }

    return 0;
}

// TODO: Pass a comparing function
int _vec_binary_search(Vector *vec, void *comp, size_t beg, size_t end) {
    for (int i = beg; i < vec->len && i < end; i = (end - i) / 2) {
        if (*(char*)vec->data + i * vec->size == *comp) {
        }
    }
}

int _vec_double_until(Vector *vec, size_t amount) {
    // Allocate double the capacity
    for (; (vec->cap - vec->len) < amount; vec->cap *= 2);
    vec->data = realloc(vec->data, vec->cap * vec->size);

    if (vec->data == NULL) { return 1; }

    return 0;
}

int vec_push(Vector *vec, void *data) {
    ++vec->len;

    // Double the capacity if there is not enough space
    if (vec->len > vec->cap) {
        int result = _vec_double(vec);
        if (result != 0) { return result; }
    }

    // Copy the data
    memcpy(vec->data + (vec->size * (vec->len - 1)), data, vec->size);

    return 0;
}

int vec_push_multi(Vector *vec, void *data, size_t amount) {
    vec->len += amount;

    // Double the capacity if there is not enough space (until there is)
    if (vec->len > vec->cap) {
        int result = _vec_double(vec);
        if (result != 0) { return result; }
    }

    // Copy the data
    memcpy(vec->data + (vec->size * (vec->len - 1)), data, vec->size);

    return 0;
}

void *vec_get(Vector *vec, size_t index) {
    if (vec->len-1 < index) { return NULL; }

    return vec->data + (vec->size * index);
}

int vec_init(Vector *vec, size_t size, void *data, size_t amount) {
    vec->size = size;
    vec->len = 0;
    vec->cap = 0;

    // Get a power of two that is closest to the required amount
    if (amount != 0) {
        for (vec->cap = 2; vec->cap < amount; vec->cap *= 2);
    }

    p_vec_info(vec);

    // Allocate the memory
    vec->data = calloc(vec->cap, vec->size);
    if (vec->data == NULL) { return 1; }

    // Assign the data
    if (amount > 0 && data != NULL) {
        memcpy(vec->data, data, vec->size * amount);
        vec->len = amount;
    } else {
        vec->len = 0;
    }

    return 0;
}

errno_t vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize) {
    FILE *file;
    errno_t err = 0;
    void *data;

    err = fopen_s(&file, file_name, "r");
    if (err != 0) { return err; }
    printf("File opened!\n");

    if (vec->cap == 0) {
        vec->cap = 4;
        vec->data = realloc(vec->data, vec->cap * vec->size);
        printf("No capacity... setting to 4!\n");
    }

    if (vec->cap > vec->len) {
        char* fgets_ret;
        fgets_ret = fgets(vec->data + (vec->len * vec->size), (vec->cap - vec->len) * vec->size, file);
        printf("File hopefully read correctly...\n");

        if (fgets_ret == NULL) {
            // Check for errors
            err = ferror(file);
            if (err != 0) { return err; }
            printf("No errors!\n");

            // Check for EOF
            int feof_ret = 0;
            feof_ret = feof(file);
            if (feof_ret == 0) {
                printf("It's EOF! %d\n", feof_ret);
                goto finalize;
            }
        }

        printf("First char: '%c'\n", *fgets_ret);
        vec->len = vec->cap;
        printf("No issues while reading the file!\n");
    }
    

    printf("\n\n\nPROCEEDING...\n\n\n");

    // Allocate double the amount +2 (to get rid of `\n` and `\0`)
    // If the vector is full and the last character is not a `\n` or '\0', repeat
    // Do the first read without +2 in hope that it fits the first time
    //for (; vec->cap == vec->len;) {
    //    char* fgets_ret;

    //    _vec_double(vec);
    //    fgets(vec->data + (vec->len * vec->size), vec->cap - vec->len, file);

    //    // Full read failed, error or EOF
    //    if (fgets_ret == NULL) {
    //        // Check for errors
    //        err = ferror(file);
    //        if (err != 0) { return err; }

    //        // Check for EOF
    //        int feof_ret = 0;
    //        feof_ret = feof(file);
    //        if (feof_ret == 0) {
    //            goto finalize;
    //        }
    //    }

    //    
    //}

finalize:
    printf("\n\n\nFINALIZING...\n\n\n");

    if (minimize && vec->len != vec->cap) {
        printf("Minimizing the capacity to match the length. Cap: %d, Len: %d", (int)vec->cap, (int)vec->len);
        vec->data = realloc(vec->data, vec->cap * vec->size);
    }

    printf("Done!\n");

    return err;
}

void p_vec_info(Vector *vec) {
    printf("Len: %d\n", (int)vec->len);
    printf("Cap: %d\n", (int)vec->cap);
    printf("Size: %d\n", (int)vec->size);
}
