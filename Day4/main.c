#include <ctype.h>
#include <stdio.h>
#include "../libs/vec.h"

typedef struct {
    int id;
    Vector winning_numbers;
    Vector numbers_you_have;
} Card;

int get_number(Vector *vec, size_t beg, size_t *end) {
    int num = 0;
    int num_len = 0;

    for (int x = beg; x < vec->len; x++) {
        char *num_char = vec_get_unchecked(vec, x);
        if (!isdigit(*num_char)) { break; };

        ++num_len;
        num *= 10;
        num += (*num_char - '0');
    }

    if (end != NULL) { *end = beg + num_len - 1; }
    return num;
}

bool is_colon(void *vec_item) { return *(char*)vec_item == ':'; }
bool until_digit(void *vec_item) { return isdigit(*(char*)vec_item); }

const size_t ID_BEGIN_INDEX = 5;

int main() {
    Vector file;
    Vector cards;

    // Read file to a Vector
    vec_read_file(&file, "input.txt", NULL, false);

    // Initialize cards vector
    vec_init(&cards, sizeof(Vector), NULL, file.len);

    // Find semicolon position
    size_t semicolon_index = 0;
    Vector *row = vec_get_unchecked(&file, 0);
    vec_find_first(row, &is_colon, ID_BEGIN_INDEX, row->len-1, &semicolon_index);

    printf("Semicolon index: %d\n", (int)semicolon_index);

    // Get cards
    for (size_t y = 0; y < file.len; y++) {
        Vector *row = vec_get_unchecked(&file, y);

        // Get card ID
        size_t id_beg_index = 0;
        vec_find_first(row, &until_digit, ID_BEGIN_INDEX, row->len-1, &id_beg_index);
        int card_id = get_number(row, id_beg_index, NULL);

        printf("Index: %d\n", card_id);

        // Get numbers you have
        Vector winning_numbers;
        vec_init(&winning_numbers, sizeof(int), NULL, 0);

        for (size_t x = semicolon_index + 2;;) {
            char *c = vec_get_unchecked(row, x);
            if (*c == '|') { break; } 

            size_t end = 0;
            int winning_number = get_number(row, x, &end);
            vec_push(&winning_numbers, &winning_number);
            x = end + 2;
        }

        // Get winning numbers
        Vector winning_numbers;
        vec_init(&winning_numbers, sizeof(int), NULL, 0);

        for (size_t x = semicolon_index + 2;;) {
            char *c = vec_get_unchecked(row, x);
            if (*c == '|') { break; } 

            size_t end = 0;
            int winning_number = get_number(row, x, &end);
            vec_push(&winning_numbers, &winning_number);
            x = end + 2;
        }

        for (size_t x = 0; x < winning_numbers.len; x++) {
            printf("%d ", *(char*)vec_get_unchecked(&winning_numbers, x));
        }
    }

    return 0;
}
