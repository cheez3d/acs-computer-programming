#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "letter.h"
#include "message.h"

const char LETTER_CODE_SPACE_STR[2] = {LETTER_CODE_SPACE, '\0'};
const char LETTER_CODE_UPPERCASE_PREFIX_STR[2] = {LETTER_CODE_UPPERCASE_PREFIX, '\0'};


bool isLetterLower(char letter) {
    return isalpha(letter) && letter == tolower(letter);
}

bool isLetterUpper(char letter) {
    return isalpha(letter) && letter == toupper(letter);
}


bool areLettersOnSameDigit(
    char letter1, char letter2,
    const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]
) {
    if (!isalpha(letter1) || !isalpha(letter2)) {
        return false;
    }

    if (isLetterUpper(letter1)) {
        letter1 = tolower(letter1);
    }

    if (isLetterUpper(letter2)) {
        letter2 = tolower(letter2);
    }

    return messageEncodeTable[letter1-'a'][0] == messageEncodeTable[letter2-'a'][0]; // daca codul literelor incepe cu aceeasi cifra,
                                                                                     // inseamna ca impartasesc aceeasi cifra/tasta
}


char* letterEncode(
    char letter, char letterCode[LETTER_CODE_MAX_SIZE+1],
    const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]
) {
    letterCode[0] = '\0';

    if (letter == ' ') {
        return strcat(letterCode, LETTER_CODE_SPACE_STR);
    } else if (isLetterUpper(letter)) {
        strcat(letterCode, LETTER_CODE_UPPERCASE_PREFIX_STR);
        letter = tolower(letter);
    }
    
    return strcat(letterCode, messageEncodeTable[letter-'a']);
}

char letterDecode(
    const char letterCode[LETTER_CODE_MAX_SIZE+1],
    const char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX]
) {
    if (letterCode[0] == LETTER_CODE_SPACE) {
        return ' ';
    }

    size_t letterCodeSize = strlen(letterCode);
    bool isLetterUpper = (letterCodeSize == 1) ? false : (letterCode[0] == LETTER_CODE_UPPERCASE_PREFIX); // daca codul are doar o cifra, aceasta nu poate fi un prefix
    unsigned digit = letterCode[isLetterUpper]-'0'; // foloseste primul caracter daca nu exista prefix
                                                    // sau al doilea caracter daca exista prefix,
                                                    // in functie de valoarea lui isLetterUpper (0 sau 1)

    letterCodeSize -= isLetterUpper;
    
    char letter = messageDecodeTable[digit-FIRST_DIGIT][letterCodeSize-1];
    return isLetterUpper ? toupper(letter) : letter;
}
