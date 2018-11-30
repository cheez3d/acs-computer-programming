#ifndef LETTER_H
#define LETTER_H

#include <stdbool.h>

#define LETTERS_PER_DIGIT_MIN 3
#define LETTERS_PER_DIGIT_MAX 4 // pentru tastele 7 si 9
#define FIRST_DIGIT 2 // prima cifra de pe tastatura ce contine litere
#define LAST_DIGIT 9 // ultima cifra de pe tastatura ce contine litere

#define LETTER_CODE_MAX_SIZE 1+LETTERS_PER_DIGIT_MAX // se adauga 1 fiindca litera poate fi majuscula si atunci
                                                     // mai e nevoie de memorie si pentru prefixul LETTER_CODE_UPPERCASE_PREFIX

#define LETTER_CODE_SPACE '0' // codul pentru reprezentarea spatiilor in mesajele codificate
#define LETTER_CODE_UPPERCASE_PREFIX '1' // prefixul care semnaleaza faptul ca litera o litera e majuscula in mesajul codificat

#include "message.h"

extern const char LETTER_CODE_SPACE_STR[2]; // sirul de caractere corespunzator lui LETTER_CODE_SPACE
extern const char LETTER_CODE_UPPERCASE_PREFIX_STR[2]; // sirul de caractere corespunzator lui LETTER_CODE_UPPERCASE_PREFIX

/**
 * Verifica daca litera este majuscula.
 **/
bool isLetterLower(char letter);

/**
 * Verifica daca litera este mica.
 **/
bool isLetterUpper(char letter);

/**
 * Verifica daca doua litere impart aceeasi cifra/tasta.
 **/
bool areLettersOnSameDigit(
    char letter1, char letter2,
    const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]
);

/**
 * Codifica o singura litera cu ajutorul tabelei de codificare messageEncodeTable,
 * stocand codificarea acesteia in letterCode.
 * 
 * Returneaza un pointer catre letterCode.
 **/
char* letterEncode(
    char letter, char letterCode[LETTER_CODE_MAX_SIZE+1],
    const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]
);

/**
 * Decodifica o singura litera cu ajutorul tabelei de decodificare messageDecodeTable.
 * 
 * Returneaza litera decodificata.
 **/
char letterDecode(
    const char letterCode[LETTER_CODE_MAX_SIZE+1], 
    const char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX]
);

#endif