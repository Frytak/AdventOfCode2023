#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

const int MAX_LINE_LENGTH = 512;

typedef struct {
    size_t len;
    size_t cap;
    size_t size;
    void *data;
} Vector;

int _vec_double(Vector *vec) {
    // Allocate double the capacity
    vec->cap *= 2;
    vec->data = realloc(vec->data, vec->cap * vec->size);

    if (vec->data == NULL) { return 1; }

    // Zero out the new memory
    size_t new_memory_size = (vec->cap / 2) * vec->size;
    memset(vec->data + new_memory_size, 0, new_memory_size);

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

void *vec_get(Vector *vec, size_t index) {
    if (vec->len-1 < index) { return NULL; }

    return vec->data + (vec->size * index);
}

int vec_init(Vector *vec, size_t size, void *data, size_t amount) {
    vec->size = size;

    // Get a power of two that is closest to the required amount
    for (vec->cap = 2; vec->cap < amount; vec->cap *= 2);

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

void p_vec_info(Vector *vec) {
    printf("Len: %d\n", (int)vec->len);
    printf("Cap: %d\n", (int)vec->cap);
    printf("Size: %d\n", (int)vec->size);
}



typedef struct {
    int num;
    bool included;
    int y;
    int beg;
    int end;
} Part;

typedef struct {
    int x;
    int y;
} Gear;


// WARNING!!! This solution is horrible...

int main() {
    FILE *input;
    char line[MAX_LINE_LENGTH];

    input = fopen("input.txt", "r");

    Vector engine;
    vec_init(&engine, sizeof(Vector), NULL, 0);

    // Get the input as a vector
    while(fgets(line, MAX_LINE_LENGTH, input)) {
        Vector row;
        vec_init(&row, sizeof(char), NULL, 0);

        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
            vec_push(&row, &(line[i]));
        }

        vec_push(&engine, &row);
    }



    int transformations[] = { -1, 0, 1 };
    
    Vector numbers;
    vec_init(&numbers, sizeof(Part), NULL, 0);

    Vector gears;
    vec_init(&gears, sizeof(Gear), NULL, 0);

    for (int y = 0; y < engine.len; y++) {
        Vector *row = vec_get(&engine, y);

        Part part = { 0, false, -1, -1, -1 };
        Gear gear = { -1, -1 };
        char prev_char = '.';

        for (int x = 0; x < engine.len; x++) {
            char *current_char = vec_get(row, x);

            if (isdigit(*current_char)) {
                if (part.num == 0) {
                    part.y = y;
                    part.beg = x;
                }
                part.num *= 10;
                part.num += *current_char - '0';

                int ay = 0;
                int max_ay = 2;

                if (y == 0) { ay = 1; }
                if (y == engine.len -1) { max_ay = 1; }

                // Check if there are special chars around the digit
                for (; ay <= max_ay; ay++) {
                    int ax = 0;
                    int max_ax = 2;

                    if (x == 0) { ax = 1; }
                    if (x == row->len - 1) { max_ax = 1; }

                    Vector *a_row = vec_get(&engine, y + transformations[ay]);

                    for (; ax <= max_ax; ax++) {
                        char *a_char = vec_get(a_row, x + transformations[ax]);

                        if (*a_char != '.' && !isdigit(*a_char)) { part.included = true; }
                    }
                }
            } else if (*current_char == '*') {
                gear.x = x;
                gear.y = y;
                vec_push(&gears, &gear);
                gear.x = -1;
                gear.y = -1;
                // Add the part when the digits end
                if (isdigit(prev_char)) {
                    part.end = x-1;
                    vec_push(&numbers, &part);
                    part.num = 0;
                    part.included = false;
                    part.y = -1;
                    part.beg = -1;
                    part.end = -1;
                }
            } else {
                // Add the part when the digits end
                if (isdigit(prev_char)) {
                    part.end = x-1;
                    vec_push(&numbers, &part);
                    part.num = 0;
                    part.included = false;
                    part.y = -1;
                    part.beg = -1;
                    part.end = -1;
                }
            }

            prev_char = *current_char;
        }

        // Add the part when there is no next char other than a number
        if (isdigit(prev_char)) {
            part.end = engine.len - 1;
            vec_push(&numbers, &part);
            part.num = 0;
            part.included = false;
            part.y = -1;
            part.beg = -1;
            part.end = -1;
        }
    }

    int sum = 0;
    for (int i = 0; i < numbers.len; i++) {
        Part *part = vec_get(&numbers, i);
        if (part->included) {
            sum += part->num;
        }
    }

    int gear_ratio_sum = 0;
    // NOTICE: It could be done more efficiently by using a hashmap but I'm not going to implement one currently
    for (int i = 0; i < gears.len; i++) {
        Gear *gear = vec_get(&gears, i);
        int part1 = 0;
        int part2 = 0;

        for (int j = 0; j < numbers.len; j++) {
            Part *part = vec_get(&numbers, j);

            for (int ai = -1; ai <= 1; ai++) {
                for (int aj = -1; aj <= 1; aj++) {
                    if (gear->y + ai == part->y && gear->x + aj >= part->beg && gear->x + aj <= part->end) {
                        if (part1 == 0 || part1 == part->num) {
                            part1 = part->num;
                        } else if (part2 == 0 || part2 == part->num) {
                            part2 = part->num;
                        } else {
                            part1 = 0;
                            part2 = 0;
                        }
                    }
                }
            }
        }

        gear_ratio_sum += part1 * part2;
    }

    printf("Sum of all the parts is equal to: %d\n", sum);
    printf("Sum of all the gear ratios is equal to: %d", gear_ratio_sum);

    fclose(input);
    return 0;
}
