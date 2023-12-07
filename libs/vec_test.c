#include <corecrt.h>
#include <stdbool.h>
#include <stdio.h>
#include "vec.h"

const char TEST_DATA[] = "Gami is a furry!";

void vec_init_test(Vector *vec, bool add_vec_info) {
    vec_init(vec, sizeof(char), (void*)&TEST_DATA, sizeof(TEST_DATA) / sizeof(char));

    if (add_vec_info) {
        p_vec_info(vec);
        printf("\n");
    }

    for (int i = 0; i < vec->len; i++) {
        printf("%c", *(char*)vec_get(vec, i));
    }
}

VBS_COMP comp(void *character) {
    switch (*(char*)character) {
        case 0: { return VBS_COMP_LEFT; }
        case '\n': { return VBS_COMP_FOUND; }
        default: { return VBS_COMP_RIGHT; }
    }
}

void vec_binary_search_test() {
    Vector vec;
    char data[] = "Gami to furras\n";
    vec_init(&vec, sizeof(char), &data, (sizeof(data) + (5 * sizeof(data[0]))) / sizeof(data[0]));

    size_t index = 0;
    int bin_ret = vec_binary_search(&vec, &comp, 0, vec.len, &index);

    switch (bin_ret) {
        case VBS_OK: { printf("Found `\\n` at index = %d", (int)index); break; }
        case VBS_OUT_OF_BOUNDS: { printf("Boundry is bigger than the vector length!"); break; }
        case VBS_COMP_INVALID_OUTPUT: { printf("Function `comp` gives invalid output!"); break; }
        case VBS_NOT_FOUND: { printf("The searched for value doesn't exist in the current boundry."); break; }
        case VBS_INVALID_INPUT: { printf("`beg` is bigger than `end`"); break; }
    }
}

void fgets_test() {
    FILE *file;
    Vector vec;

    vec_init(&vec, sizeof(char), NULL, 6);
    p_vec_info(&vec);

    printf("%d\n", fopen_s(&file, "vec_test.txt", "rt"));
    fgets(vec.data, vec.cap, file);
    p_vec_info(&vec);

    for (int i = 0; i < vec.cap; i++) {
        printf("'%c' ", *(char*)vec_get_unchecked(&vec, i));
    }

    _vec_alloc(&vec, ((vec.cap - 2) * 2) + 2);
    fgets(vec.data + (vec.cap/2) * vec.size, vec.cap/2, file);
    p_vec_info(&vec);

    for (int i = 0; i < vec.cap; i++) {
        printf("'%c' ", *(char*)vec_get_unchecked(&vec, i));
    }
}
VBS_COMP vec_rf_comp_test(void *character) {
    switch (*(char*)character) {
        case '\n': { return VBS_COMP_FOUND; }
        case 0: { return VBS_COMP_LEFT; }
        default: { return VBS_COMP_RIGHT; }
    }
}

void vec_read_file_test(Vector *vec, char file_name[], bool add_vec_info) {
    vec_read_file(vec, file_name, NULL, false);
    //FILE *file;
    //int err = 0;
    //void *data;

    //err = fopen_s(&file, file_name, "r");
    //if (err != 0) { printf("ERROR: %d\n", err); }
    //printf("File opened!\n");

    //vec_init(vec, sizeof(Vector), NULL, 0);
    //for (int j = 0; j < 3; j++) {
    //    printf("Line: ");
    //    Vector line;
    //    size_t index = 0;
    //    vec_init(&line, sizeof(char), NULL, 4);

    //    for (int i = 0; i < 10; i++) {
    //        char* fgets_ret;

    //        size_t shift = line.cap - 1;
    //        _vec_alloc(&line, ((line.cap - 2) * 2) + 2);
    //        size_t cap = shift;
    //        if (i == 0) { shift = 0; cap = line.cap; }
    //        printf("Shift: %d\n", (int)shift);
    //        printf("Cap: %d\n\n", (int)cap);
    //        fgets_ret = fgets(line.data + shift * line.size, cap, file);

    //        // Read failed
    //        if (fgets_ret == NULL) {
    //            // Check for errors
    //            err = ferror(file);
    //            if (err != 0) { printf("ERROR: %d", err); }
    //        }

    //        // Set the lenght
    //        line.len = line.cap;

    //        switch (vec_binary_search(&line, &vec_rf_comp_test, line.cap/2, line.cap, &index)) {
    //            case VBS_OK: { _vec_alloc(&line, line.cap - 2); goto line_read; }
    //            case VBS_NOT_FOUND: { break; }
    //            case VBS_INVALID_INPUT: { printf("ERROR: Invalid input."); break; }
    //            case VBS_OUT_OF_BOUNDS: { printf("ERROR: Out of bounds."); break; }
    //            case VBS_COMP_INVALID_OUTPUT: { printf("ERROR: Comp invalid output."); break; }
    //            default: { printf("ERROR: In binary search"); break; }
    //        }
    //    }

    //line_read:
    //    line.len = index;
    //    vec_push(vec, &line);
    //}

    //fclose(file);

    //printf("\n\n\n");
    //for (int y = 0; y < vec->len; y++) {
    //    Vector *row = vec_get_unchecked(vec, y);
    //    printf("\n");
    //    p_vec_info(row);
    //    printf("\nLine:");
    //    for (int x = 0; x < row->len; x++) {
    //        Vector *c = vec_get_unchecked(row, x);
    //        printf("'%c' ", *(char*)c);
    //    }
    //}
}

int main() {
    Vector vec;

    //vec_init_test(&vec, true);
    vec_read_file_test(&vec, "vec_test.txt", true);
    //vec_binary_search_test();
    //fgets_test();

    return 0;
}
