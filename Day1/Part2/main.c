#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

const int LINE_LENGTH = 128;
const char dictionary[][2][6] = {
    { "1", "one" },
    { "2", "two" },
    { "3", "three" },
    { "4", "four" },
    { "5", "five" },
    { "6", "six" },
    { "7", "seven" },
    { "8", "eight" },
    { "9", "nine" }
};

void scavengeNumbers(char *input, char *output, int *last_index) {
    int outputIndex = 0;

    // For each character of input...
    for (int i = 0; input[i] != '\0'; i++) {

        if (isdigit(input[i])) {
            output[outputIndex++] = input[i];
            continue;
        }

        // Look at each word in a dictionary...
        for (int j = 0; j < 9; j++) {

            // Compare characters from the dictionray word...
            for (int k = 0; true; k++) {
                // If the word ends first it matches
                if (dictionary[j][1][k] == '\0') {
                    output[outputIndex++] = dictionary[j][0][0];
                    break;
                }

                // If the input ends first...
                if (input[i+k] == '\0') { break; }

                // If the characters don't match
                if (input[i+k] != dictionary[j][1][k]) { break; }
            }
        }
    }

    *last_index = outputIndex - 1;
    output[outputIndex] = '\0';
}

int main() {
    FILE *pinput;
    char line[LINE_LENGTH];
    int calibrationValueSum = 0;
    char output[LINE_LENGTH];
    int last_index = 0;

    pinput = fopen("../input.txt", "r");
    while (fgets(line, LINE_LENGTH, pinput)) {
        scavengeNumbers(&line, &output, &last_index);
        char numberStr[3] = { output[0], output[last_index], '\0' };
        memset(output, 0, sizeof(output));
        calibrationValueSum += strtol(numberStr, NULL, 10);
    }
    printf("%d\n", calibrationValueSum);

    fclose(pinput);
    return 0;
}
