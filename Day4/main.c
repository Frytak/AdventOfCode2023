#include <stdio.h>
#include "../libs/vec.h"

int main() {
    FILE *input;
    char line[512];
    Vector line_vec;

    vec_init(&line_vec, sizeof(char), NULL, 0);

    input = fopen("input.txt", "r");
    while (fgets(line, 512, input)) {
        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
            vec_push(&line_vec, &line[i]);
        }
    }

    for (int i = 0; i < line_vec.len; i++) {
        printf("%c", *(char*)vec_get(&line_vec, i));
    }

    fclose(input);
    return 0;
}
