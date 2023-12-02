#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>

const int MAX_LINE_LENGTH = 512;
const int ID_BEGIN_INDEX = 5;

typedef struct {
    int red;
    int green;
    int blue;
} Draw;

typedef struct {
    int Id;
    int numOfDraws;
    Draw *draws;
} Game;

// Reads a number until character that's not a number
int readNum(char line[], int startPos) {
    int num = 0;

    for (int i = startPos; isdigit(line[i]); i++) {
        num *= 10;
        num += line[i] - '0';
    }

    return num;
}

int readGameID(char line[]) {
    return readNum(line, ID_BEGIN_INDEX);
}

void readGameDraw(int Id, char line[], int startPos, int *endPos, Draw *output) {
    int newStartPos = 0;
    // The index from which the `Draw` begins, if `startPos` is 0, gets the first `Draw`
    if (startPos == 0) { newStartPos = ID_BEGIN_INDEX + (int)log10(Id) + 1 + 2; } else { newStartPos = startPos; }

    int currentNum = 0;
    for (int x = 0; x < 3; x++) {

        // Get the number
        currentNum = readNum(line, newStartPos);

        // Assign to correct color and move to new position
        int numLength = (int)log10(currentNum) + 1;

        char color = line[newStartPos + numLength + 1];
        switch (color) {
            case 'r': { output->red = currentNum; newStartPos += numLength + 3 + 3; break; }
            case 'g': { output->green = currentNum; newStartPos += numLength + 5 + 3; break; }
            case 'b': { output->blue = currentNum; newStartPos += numLength + 4 + 3; break; }
            default: { printf("ERROR: Could not match color starting with '%c'.", color); break; }
        }
        
        // Check if done sooner
        if (line[newStartPos - 1] == '\0' || line[newStartPos - 2] == ';') { break; }
    }

    if (line[newStartPos - 1] == '\0') {
        *endPos = 0;
    } else {
        *endPos = newStartPos;
    }
}

bool isGamePossible(Game *game, int red, int green, int blue) {
    bool isPossible = true;
    for (int i = 0; i < game->numOfDraws; i++) {
        if (game->draws[i].red > red) { isPossible = 0; }
        if (game->draws[i].green > green) { isPossible = 0; }
        if (game->draws[i].blue > blue) { isPossible = 0; }
    }
    return isPossible;
}

int powerOfDraw(Game *game) {
    int highestRed = 0;
    int highestBlue = 0;
    int highestGreen = 0;

    for (int i = 0; i < game->numOfDraws; i++) {
        if (game->draws[i].red > highestRed) { highestRed = game->draws[i].red;}
        if (game->draws[i].blue > highestBlue) { highestBlue = game->draws[i].blue;}
        if (game->draws[i].green > highestGreen) { highestGreen = game->draws[i].green;}
    }

    return highestRed * highestBlue * highestGreen;
}



int main() {
    FILE *input_file;
    char line[MAX_LINE_LENGTH];

    // Open the file
    input_file = fopen("../input.txt", "r");

    int idSum = 0;
    int powerSum = 0;
    while (fgets(line, MAX_LINE_LENGTH, input_file)) {
        int newPos = 0;
        Game game = { readGameID(line), 0, (Draw*) calloc(0, sizeof(Draw)) };

        do {
            game.numOfDraws += 1;
            // TODO: Make more efficient
            game.draws = realloc(game.draws, game.numOfDraws * sizeof(Draw));
            if (game.draws == NULL) { printf("FAILED TO REALOCATE!"); break; }
            game.draws[game.numOfDraws-1].red = 0;
            game.draws[game.numOfDraws-1].blue = 0;
            game.draws[game.numOfDraws-1].green = 0;
            
            readGameDraw(game.Id, line, newPos, &newPos, &game.draws[game.numOfDraws-1]);
        } while (newPos != 0);

        if (isGamePossible(&game, 12, 13, 14)) { idSum += game.Id; }
        powerSum += powerOfDraw(&game);

        //printf("Game: %d\n", game.Id);
        //for (int i = 0; i < game.numOfDraws; i++) {
        //    printf("\tDraw: %d\n", i);
        //    printf("\t\tRed: %d\n", game.draws[i].red);
        //    printf("\t\tBlue: %d\n", game.draws[i].blue);
        //    printf("\t\tGreen: %d\n", game.draws[i].green);
        //}

        free(game.draws);
    }

    printf("The IDs of possible games sum up to: %d\n", idSum);
    printf("The sum of powers of each games is: %d", powerSum);

    fclose(input_file);
    return 0;
}
