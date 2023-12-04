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

void vec_read_file_test(Vector *vec, char file_name[], bool add_vec_info) {
    size_t bytes_written;
    errno_t ret;

    vec_init(vec, sizeof(char), NULL, 20);
    ret = vec_read_file(vec, file_name, &bytes_written, 0);
    p_vec_info(vec);
    for (int i = 0; i < vec->len; i++) {
        printf("'%d' ", *(char*)vec_get(vec, i));
    }

    printf("\nRETURN: %d", ret);
}

int main() {
    Vector vec;

    //vec_init_test(&vec, true);
    vec_read_file_test(&vec, "vec_test.txt", true);

    return 0;
}
