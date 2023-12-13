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
bool contains_num(void *vec_item, void* provided_item) { printf("\tNum compare: %d == %d\n", *(int*)provided_item, *(int*)vec_item); return *(int*)vec_item == *(int*)provided_item; }

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

        // Get winning numbers
        size_t pipe_index = 0;
        Vector winning_numbers;
        vec_init(&winning_numbers, sizeof(int), NULL, 0);

        for (size_t x = semicolon_index + 2; x < row->len;) {
            char *c = vec_get_unchecked(row, x);
            if (*c == '|') { pipe_index = x; break; } 

            size_t num_begin_index = 0;
            vec_find_first(row, &until_digit, x, row->len-1, &num_begin_index);

            size_t end = 0;
            int winning_number = get_number(row, num_begin_index, &end);
            vec_push(&winning_numbers, &winning_number);
            x = end + 2;
        }

        // Get numbers you have
        Vector numbers_you_have;
        vec_init(&numbers_you_have, sizeof(int), NULL, 0);

        for (size_t x = pipe_index + 2; x < row->len;) {
            char *c = vec_get_unchecked(row, x);

            size_t num_begin_index = 0;
            vec_find_first(row, &until_digit, x, row->len-1, &num_begin_index);

            size_t end = 0;
            int number_you_have = get_number(row, num_begin_index, &end);
            vec_push(&numbers_you_have, &number_you_have);
            x = end + 2;
        }

        // Push the card to vector of cards
        Card card = { card_id, winning_numbers, numbers_you_have };
        vec_push(&cards, &card);
    }

    p_vec_info(&cards);

    for (int c = 0; c < cards.len; c++) {
        Card *card = vec_get_unchecked(&cards, c);

        printf("Card index: %d\n", card->id);
        printf("Winning numbers: ");
        for (int win = 0; win < card->winning_numbers.len; win++) {
            printf("%d ", *(int*)vec_get_unchecked(&card->winning_numbers, win));
        }
        printf("\n");

        printf("Numbers you have: ");
        for (int have = 0; have < card->numbers_you_have.len; have++) {
            printf("%d ", *(int*)vec_get_unchecked(&card->numbers_you_have, have));
        }
        printf("\n\n");
    }

    // Sum up card worth
    int card_worth_sum = 0;
    for (int i = 0; i < cards.len; i++) {
        Card *card = vec_get_unchecked(&cards, i);
        printf("Card ID: %d\n", card->id);

        // Calculate worth of a card
        int card_worth = 0;
        for (int j = 0; j < card->numbers_you_have.len; j++) {
            int *num = vec_get_unchecked(&card->numbers_you_have, j);

            switch (vec_contains(&card->winning_numbers, &contains_num, num, 0, card->winning_numbers.len-1, NULL)) {
                case CF_OK: { printf("Found"); (card_worth == 0) ? (card_worth += 1) : (card_worth *= 2); break; }
                case CF_NOT_FOUND: { break; }
                default: { printf("ERROR!"); }
            }
        }
    }

    printf("Cards worth sums up to: %d", card_worth_sum);

    return 0;
}
