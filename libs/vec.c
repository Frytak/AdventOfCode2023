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
        if (vec->cap == 0) {
            _vec_alloc(vec, 2);
        } else {
            int result = _vec_double(vec);
            if (result != 0) { return result; }
        }
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

    if (amount != 0) { vec->cap = amount; }

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

// TODO: Implement errors
void vec_find_first(Vector *vec, bool (*comp)(void *vec_item), size_t beg, size_t end, size_t *index) {
    if (beg > end) { return; }
    if (end > vec->len-1) { return; }

    for (size_t x = beg; x < end; x++) {
        if (comp(vec_get_unchecked(vec, x))) {
            *index = x;
            return;
        }
    }
}

int vec_read_file(Vector *vec, char file_name[], size_t *bytes_written, bool minimize) {
    FILE *file;
    int err = 0;
    void *data;

    err = fopen_s(&file, file_name, "r");
    if (err != 0) { printf("ERROR: %d\n", err); }

    vec_init(vec, sizeof(Vector), NULL, 0);
    while (true) {
        Vector line;
        size_t index = 0;
        vec_init(&line, sizeof(char), NULL, 4);

        for (int i = 0;; i++) {
            char* fgets_ret;

            size_t shift = line.cap - 1;
            _vec_alloc(&line, ((line.cap - 2) * 2) + 2);
            size_t cap = shift;
            if (i == 0) { shift = 0; cap = line.cap; }
            fgets_ret = fgets(line.data + shift * line.size, cap, file);

            // Read failed
            if (fgets_ret == NULL) {
                // Check for errors
                err = ferror(file);
                if (err != 0) { printf("ERROR: %d", err); }

                // Check for EOF 
                if (feof(file) != 0) {
                    goto file_read;
                }
            }

            // Set the lenght
            line.len = line.cap;

            switch (vec_binary_search(&line, &vec_rf_comp, line.cap/2, line.cap, &index)) {
                case VBS_OK: { _vec_alloc(&line, line.cap - 2); goto line_read; }
                case VBS_NOT_FOUND: { break; }
                case VBS_INVALID_INPUT: { printf("ERROR: Invalid input."); break; }
                case VBS_OUT_OF_BOUNDS: { printf("ERROR: Out of bounds."); break; }
                case VBS_COMP_INVALID_OUTPUT: { printf("ERROR: Comp invalid output."); break; }
                default: { printf("ERROR: In binary search"); break; }
            }
        }

    line_read:
        line.len = index;
        vec_push(vec, &line);
    }

file_read:
    fclose(file);
    return err;
}

void p_vec_info(Vector *vec) {
    printf("Len: %d\n", (int)vec->len);
    printf("Cap: %d\n", (int)vec->cap);
    printf("Size: %d\n", (int)vec->size);
}
