#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

const int LINE_LENGTH = 128;

int main() {
    FILE *pinput;
    char line[LINE_LENGTH];
    int calibrationValueSum = 0;

    pinput = fopen("../input.txt", "r");
    while (fgets(line, LINE_LENGTH, pinput)) {
        char first_num = 0;
        char last_num = 0;

        for (int i = 0; line[i] != '\0'; i++) {
            if (!isdigit(line[i])) { continue; }
            if (first_num == 0) { first_num = line[i]; }
            last_num = line[i];
        }

        char numberStr[3] = { first_num, last_num, '\0' };
        long int number = strtol(numberStr, NULL, 10);

        calibrationValueSum += number;
    }
    printf("%d\n", calibrationValueSum);

    fclose(pinput);
    return 0;
}
