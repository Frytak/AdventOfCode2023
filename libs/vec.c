#include <corecrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "vec.h"

int _vec_alloc(Vector *vec, size_t cap) {
    vec->cap = cap;
    vec->data = realloc(vec->data, vec->cap * vec->size);

    if (vec->data == NULL) { return 1; }

    return 0;
}

int _vec_double(Vector *vec) {
    // Allocate double the capacity
    return _vec_alloc(vec, vec->cap * 2);
}

VBS_RET vec_binary_search(Vector *vec, VBS_COMP (*comp)(void *vec_item), size_t beg, size_t end, size_t *found) {
    if (vec->len < end || vec->len < beg) { return VBS_OUT_OF_BOUNDS; }
    if (beg > end) { return VBS_INVALID_INPUT; }
    size_t new_beg = beg;
    size_t new_end = end;

    while (true) {
        size_t middle = (new_end + new_beg) / 2;
        char* current_char = vec->data + (vec->size * middle);

        if (middle == new_end || middle == new_beg) { return VBS_NOT_FOUND; }

        switch (comp(current_char)) {
            case VBS_COMP_LEFT: { new_end = middle; break; }
            case VBS_COMP_FOUND: { *found = middle; return VBS_OK; }
            case VBS_COMP_RIGHT: { new_beg = middle; break; }
            default: { return VBS_COMP_INVALID_OUTPUT; }
        }
    }

    return VBS_NOT_FOUND;
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

void *vec_get_unchecked(Vector *vec, size_t index) {
    return vec->data + (vec->size * index);
}

void *vec_get(Vector *vec, size_t index) {
    if (vec->len-1 < index) { return NULL; }
    return vec_get_unchecked(vec, index);
}

int vec_init(Vector *vec, size_t size, void *data, size_t amount) {
    vec->size = size;
    vec->len = 0;
    vec->cap = 0;

    // Get a power of two that is closest to the required amount
    if (amount != 0) {
        vec->cap = amount;
    }

    // Allocate the memory
    vec->data = malloc(vec->cap * vec->size);
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

VBS_COMP vec_rf_comp(void *character) {
    switch (*(char*)character) {
        case '\n': { return VBS_COMP_FOUND; }
        case 0: { return VBS_COMP_LEFT; }
        default: { return VBS_COMP_RIGHT; }
    }
}

int vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize) {
    FILE *file;
    int err = 0;
    void *data;

    err = fopen_s(&file, file_name, "r");
    if (err != 0) { return err; }
    printf("File opened!\n");

    if (vec->cap == 0) {
        _vec_alloc(vec, 4);
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

    // Read each line as a seperate `Vector`
    while (true) {
        Vector line;
        vec_init(&line, sizeof(char), NULL, 4);

        // Allocate double the amount +2 (to get rid of `\n` and `\0`)
        // If the vector is full and the last character is not a `\n` or '\0', repeat
        // Do the first read without +2 in hope that it fits the first time
        while (true) {
            char* fgets_ret;

            // Minimal `cap` required is 3 (to fit `\n` and '\0', and so it's )
            _vec_alloc(vec, ((vec->cap - 2) * 2) + 2);
            fgets_ret = fgets(vec->data + (vec->cap/2) * vec->size, vec->cap/2, file);

            // Read failed
            if (fgets_ret == NULL) {
                // Check for errors
                err = ferror(file);
                if (err != 0) { return err; }
            }

            size_t index = 0;
            switch (vec_binary_search(vec, &vec_rf_comp, vec->cap/2, vec->cap, &index)) {
                case VBS_OK: { _vec_alloc(&line, line.cap - 2); goto line_read; }
                case VBS_NOT_FOUND: { break; }
                default: { return -1; }
            }
        }

    line_read:
        vec_push(vec, &line);
    }

finalize:
    printf("\n\n\nFINALIZING...\n\n\n");

    if (minimize && vec->len != vec->cap) {
        printf("Minimizing the capacity to match the length. Cap: %d, Len: %d", (int)vec->cap, (int)vec->len);
        vec->data = realloc(vec->data, vec->cap * vec->size);
    }

    fclose(file);
    printf("Done!\n");

    return err;
}

void p_vec_info(Vector *vec) {
    printf("Len: %d\n", (int)vec->len);
    printf("Cap: %d\n", (int)vec->cap);
    printf("Size: %d\n", (int)vec->size);
}
