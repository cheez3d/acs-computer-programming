#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "letter.h"
#include "message.h"
#include "helpers.h"

const char MESSAGE_CODE_CONFLICT_SEPARATOR_STR[2] = {MESSAGE_CODE_CONFLICT_SEPARATOR, '\0'};

/**
 * Tabela de codificare contine MESSAGE_ENCODE_TABLE_SIZE de siruri corespunzatoare codului fiecarei litere.
 **/
void messageEncodeTableInit(char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]) {
    unsigned digit = FIRST_DIGIT;
    unsigned lettersPerDigit = LETTERS_PER_DIGIT_MIN;

    for (char letter = 'a'; letter <= 'z' && digit <= LAST_DIGIT; letter += lettersPerDigit) {
        if (digit == 7 || digit == 9) { // hardcoded :(
            lettersPerDigit = LETTERS_PER_DIGIT_MAX;
        } else if (lettersPerDigit != LETTERS_PER_DIGIT_MIN) {
            lettersPerDigit = LETTERS_PER_DIGIT_MIN;
        }

        for (unsigned digitCount = 1; digitCount <= lettersPerDigit; ++digitCount) {
            unsigned pos = (letter-'a')+(digitCount-1);

            unsigned i;
            for (i = 0; i < digitCount; ++i) {
                messageEncodeTable[pos][i] = '0'+digit;
            }
            messageEncodeTable[pos][i] = '\0';
        }

        ++digit;
    }
}

/**
 * Tabela de decodificare contine MESSAGE_DECODE_TABLE_SIZE vectori de cate maxim LETTERS_PER_DIGIT_MAX
 * elemente, vectori pentru care pozitia elementului corespunde cu un numar de repetari al unei cifre din cod.
 **/
void messageDecodeTableInit(char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX]) {
    char letter = 'a';

    for (unsigned digit = FIRST_DIGIT; digit <= LAST_DIGIT && letter <= 'z'; ++digit) {
        unsigned lettersPerDigit = (digit == 7 || digit == 9) ? LETTERS_PER_DIGIT_MAX : LETTERS_PER_DIGIT_MIN; // hardcoded :(
        
        for (unsigned i = 0; i < lettersPerDigit; ++i) {
            messageDecodeTable[digit-FIRST_DIGIT][i] = letter;

            ++letter;
        }
    }
}


char* messageEncode(
    const char message[MESSAGE_MAX_SIZE+1], char messageCode[MESSAGE_CODE_MAX_SIZE+1],
    const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]
) {
    messageCode[0] = '\0';

    char letterCode[LETTER_CODE_MAX_SIZE+1];
    
    strcat(messageCode, letterEncode(message[0], letterCode, messageEncodeTable));
    for (unsigned i = 1; i < strlen(message); ++i) {
        // introduce separatorul de conflict intre doua coduri ce folosesc aceeasi cifra
        // nu face acest lucru daca litera din urma este mare, pentru ca nu se creeaza nicio confuzie
        if (areLettersOnSameDigit(message[i-1], message[i], messageEncodeTable) && !isLetterUpper(message[i])) {
            strcat(messageCode, MESSAGE_CODE_CONFLICT_SEPARATOR_STR);
        }

        // codifica fiecare litera in parte
        letterEncode(message[i], letterCode, messageEncodeTable);

        strcat(messageCode, letterCode);
    }

    return messageCode;
}

char* messageDecode(
    const char messageCode[MESSAGE_CODE_MAX_SIZE+1], char message[MESSAGE_MAX_SIZE+1],
    const char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX]
) {
    const char *ptr = messageCode;
    unsigned i = 0;

    while (*ptr != '\0') {
        if (*ptr == MESSAGE_CODE_CONFLICT_SEPARATOR) {
            ++ptr;
        }

        char letterCode[LETTER_CODE_MAX_SIZE+1];
        letterCode[0] = '\0';
        
        // TODO: add periodicity logic (not required by checker, but it's a plus)
        
        const char *startPtr = ptr;
        while (*(ptr+1) == *ptr || *ptr == LETTER_CODE_UPPERCASE_PREFIX) {
            ++ptr;
        }

        strncat(letterCode, startPtr, ptr-startPtr+1);

        // decodifica fiecare litera in parte
        message[i++] = letterDecode(letterCode, messageDecodeTable);
        
        ++ptr;
    }

    message[i] = '\0';

    return message;
}

char* messageCorrect(char message[MESSAGE_MAX_SIZE+1], const struct Dict *const dict) {
    char *messageCopy = strdup(message);

    const char *target = strtok(messageCopy, " ");
    int correctionOffset = 0;
    
    while (target != NULL) {
        // cauta cuvantul curent in dictionar
        const struct DictEntry *const dictEntry = dictGetEntryByTarget(dict, target);

        if (dictEntry != NULL) {
            size_t targetSize = strlen(dictEntry->target);
            size_t substSize = strlen(dictEntry->subst);
            size_t min = targetSize < substSize ? targetSize : substSize;

            char *dest = message+(target-messageCopy)+correctionOffset;
            memmove(dest, dictEntry->subst, min); // muta numarul de caractere comune atat pentru target cat si subst
            
            // daca lungimile sirurilor nu difera am terminat de corectat cuvantul
            // daca lungimile sirurilor target si subst difera
            if (substSize > targetSize) { // daca subst e mai mare
                memmove( // deplasam partea din dreapta a mesajului spre dreapta, pentru a mai face loc
                    dest+min+(substSize-targetSize),
                    dest+min,
                    strlen(dest+min)+1 // +1 pentru a muta si terminatorul de sir
                );

                correctionOffset += substSize-targetSize;

                memmove(dest+min, dictEntry->subst+min, substSize-targetSize);
            } else if (substSize < targetSize) { // daca subst e mai mic
                memmove( // deplasam partea din dreapta a mesajului spre stanga, pentru a umple spatiul gol
                    dest+min,
                    dest+min+(targetSize-substSize),
                    strlen(dest+min+(targetSize-substSize))+1 // +1 pentru a muta si terminatorul de sir
                );

                correctionOffset -= targetSize-substSize;
            }
        }

        target = strtok(NULL, " ");
    }

    free(messageCopy);

    return message;
}

char* messageCodeStrip(char messageCode[MESSAGE_CODE_MAX_SIZE+1]) {
    char *ptr = messageCode;

    while ((ptr = strchr(ptr, MESSAGE_CODE_CONFLICT_SEPARATOR)) != NULL) { // cautam toate caracterele MESSAGE_CODE_CONFLICT_SEPARATOR
        // deplasam mesajul codificat spre stanga cu 1 pozitie pentru a umple spatiul gol
        memmove(ptr, ptr+1, strlen(ptr+1)+1); // +1 pentru a muta si terminatorul de sir
        ++ptr;
    }

    return messageCode;
}

unsigned long long* messageCodeSplit(const char messageCode[MESSAGE_CODE_MAX_SIZE+1], unsigned *messageCodeComponentCountPtr) {
    *messageCodeComponentCountPtr = 0;

    // calculeaza numarul de componente ale mesajului codificat
    const char *ptr = messageCode;
    do {
        ++(*messageCodeComponentCountPtr);

        ++ptr;
    } while ((ptr = strchr(ptr, LETTER_CODE_SPACE)) != NULL);

    unsigned long long *messageCodeComponents = (unsigned long long*)malloc(*messageCodeComponentCountPtr*sizeof(unsigned long long));

    char *messageCodeCopy = strdup(messageCode);
    messageCodeStrip(messageCodeCopy); // inlatura simbolurile MESSAGE_CODE_CONFLICT_SEPARATOR

    unsigned i = 0;
    while ((ptr = strtok(i == 0 ? messageCodeCopy : NULL, LETTER_CODE_SPACE_STR)) != NULL) {
        messageCodeComponents[i++] = strtoull(ptr, NULL, 10); // transforma sirul de caractere in numar

        ptr += strlen(ptr)+1;
    }

    free(messageCodeCopy);

    return messageCodeComponents;
}

unsigned long long* messageCodeSplitMagic(const char messageCode[MESSAGE_CODE_MAX_SIZE+1], unsigned *messageCodeComponentCountPtr) {
    char *messageCodeCopy = strdup(messageCode);
    messageCodeStrip(messageCodeCopy);
    
    // calculeaza suma cifrelor mesajului codificat
    unsigned sum = 0;
    for (unsigned i = 0; i < strlen(messageCodeCopy); ++i) {
        sum += messageCodeCopy[i]-'0';
    }

    unsigned magicNumber = sum%9; // calculeaza numarul magic
    if (magicNumber == 0 || strlen(messageCodeCopy)%magicNumber != 0) { // daca mesajul nu este perfect
        return NULL;
    }

    *messageCodeComponentCountPtr = strlen(messageCodeCopy)/magicNumber;

    // introducem messageCodeComponentCount-1 terminatori de sir intre componentele mesajului codificat
    // pentru ca functia de conversie de la sir de caractere la numar sa functioneze corespunzator
    messageCodeCopy = (char*)realloc(messageCodeCopy, (strlen(messageCodeCopy)+1+(*messageCodeComponentCountPtr-1))*sizeof(char*));

    char *ptr = messageCodeCopy;
    while (*ptr != '\0') {
        ptr += magicNumber;
        size_t len = strlen(ptr)+1;
        memmove(ptr+1, ptr, len);
        *ptr = '\0';
        ++ptr;
    }
    
    unsigned long long *messageCodeComponents = (unsigned long long*)malloc(*messageCodeComponentCountPtr*sizeof(unsigned long long));

    ptr = messageCodeCopy;
    unsigned i = 0;
    while (*ptr != '\0') {
        messageCodeComponents[i++] = strtoull(ptr, NULL, 10);

        ptr += magicNumber+1; // +1 pentru a trece si de terminatorul de sir
    }
    
    free(messageCodeCopy);

    return messageCodeComponents;
}

bool isMessageCodePrime(unsigned long long *messageCodeComponents, unsigned messageCodeComponentCount) {
    if (messageCodeComponentCount == 1) {
        return true;
    }

    // verificam daca toate componentele sunt prime intre ele doua cate doua
    for (unsigned i = 0; i <= messageCodeComponentCount-2; ++i) {
        for (unsigned j = i+1; j <= messageCodeComponentCount-1; ++j) {
            if (gcd(messageCodeComponents[i], messageCodeComponents[j]) != 1) {
                return false;
            }
        }
    }

    return true;
}
