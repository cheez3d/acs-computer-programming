#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "letter.h"
#include "message.h"
#include "dict.h"
#include "helpers.h"

// POSSIBLE REFACTORINGS
// - remove casts from malloc, calloc calls
// - change returns from numbers to EXIT defines
// - move constants first in compares (e.g. NULL == ...)
// - change count variable types to size_t
// - change strcspn \n comment to the one from tema-2
// - remove unnecessary *const-s
// - replace strtok while loops with for loops (initial statement with str argument, increment with NULL)
// - change makefile -g flag valgrind
// - change all allocs sizeof from type to *variable
// - nodejs also get compiler path from json and others
// - change #include directives "" to have main one at the top and rest underneath with one empty line in between
// - change header include order
// - add coments for indication to #endif-s
// - change asterisk location in type casts and function return types like this: int * test();

#ifdef TASK1
void read1(char message[MESSAGE_MAX_SIZE+1]) {
    fgets(message, MESSAGE_MAX_SIZE+1, stdin);
    message[strcspn(message, "\n")] = '\0'; // sterge caracterul newline adaugat de fgets de la sfarsitul sirului
}

void solve1(const char message[MESSAGE_MAX_SIZE+1], const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]) {
    char messageCode[MESSAGE_CODE_MAX_SIZE+1];
    puts(messageEncode(message, messageCode, messageEncodeTable)); // afiseaza mesajul codificat cu ajutorul tabelei messageEncodeTable
}
#endif

#ifdef TASK2
void read2(char messageCode[MESSAGE_CODE_MAX_SIZE+1]) {
    fgets(messageCode, MESSAGE_CODE_MAX_SIZE+1, stdin);
    messageCode[strcspn(messageCode, "\n")] = '\0';
}

void solve2(const char messageCode[MESSAGE_CODE_MAX_SIZE+1], const char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX]) {
    char message[MESSAGE_CODE_MAX_SIZE+1];
    puts(messageDecode(messageCode, message, messageDecodeTable)); // afiseaza mesajul decodificat cu ajutorul tabelei messageDecodeTable
}
#endif

#ifdef TASK3
void read3(char message[MESSAGE_MAX_SIZE+1], struct Dict *const dict) {
    fgets(message, MESSAGE_MAX_SIZE+1, stdin);
    message[strcspn(message, "\n")] = '\0';

    unsigned dictEntryCount;
    scanf("%u", &dictEntryCount); // citeste numarul de intrari din dictionar

    dictInit(dict, dictEntryCount);
    
    // goleste zona tampon de caractere reziduale
    // am incercat si fflush(stdin), dar nu functioneaza
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    // citeste intrarile din dictionar
    for (unsigned i = 0; i < dictEntryCount; ++i) {
        char line[MESSAGE_MAX_SIZE+1+MESSAGE_MAX_SIZE+1]; // +1 din mijloc pentru spatiu, +1 de la sfarsit pentru terminatorul de sir

        fgets(line, MESSAGE_MAX_SIZE+1+MESSAGE_MAX_SIZE+1, stdin);
        line[strcspn(line, "\n")] = '\0';

        struct DictEntry dictEntry;

        char *target = strtok(line, " ");
        char *subst = strtok(NULL, " ");

        dictEntryInit(&dictEntry, target, subst);

        dictAddEntry(dict, dictEntry);
    }
}

void solve3(char message[MESSAGE_MAX_SIZE+1], const struct Dict *const dict, const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]) {
    messageCorrect(message, dict); // corecteaza cuvintele gresite din mesajul message cu ajutorul dictionarului dict
    
    char messageCode[MESSAGE_CODE_MAX_SIZE];
    puts(messageEncode(message, messageCode, messageEncodeTable)); // afiseaza mesajul codificat
}
#endif

#ifdef TASK4
void read4(char message[MESSAGE_MAX_SIZE+1]) {
    fgets(message, MESSAGE_MAX_SIZE+1, stdin);
    message[strcspn(message, "\n")] = '\0';
}

void solve4(const char message[MESSAGE_MAX_SIZE+1], const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]) {
    char messageCode[MESSAGE_CODE_MAX_SIZE+1];
    messageEncode(message, messageCode, messageEncodeTable);
    
    unsigned messageCodeComponentCount;
    unsigned long long *messageCodeComponents = messageCodeSplit(messageCode, &messageCodeComponentCount); // imparte mesajul in componente


    printf("%u\n", messageCodeComponentCount); // afiseaza numarul de componente

    if (messageCodeComponentCount > 1) { // facem aceasta verificare pentru ca variabila e unsigned si cand scadem doi poate da underflow daca este egala cu 1
        for (unsigned i = 0; i <= messageCodeComponentCount-2; ++i) {
            printf("%llu ", messageCodeComponents[i]);
        }
    }
    printf("%llu\n", messageCodeComponents[messageCodeComponentCount-1]);


    unsigned maxIndex = 0;
    unsigned long long max = messageCodeComponents[0];

    for (unsigned i = 1; i < messageCodeComponentCount; ++i) {
        if (messageCodeComponents[i] > max) {
            maxIndex = i;
            max = messageCodeComponents[i];
        }
    }

    printf("%llu %u\n", max, maxIndex+1); // afiseaza elementul maxim si pozitia lui

    printf("%d\n", isMessageCodePrime(messageCodeComponents, messageCodeComponentCount)); // afiseaza daca mesajul este prim

    free(messageCodeComponents);


    // incearca impartirea mesajului in componente dupa regula numarului magic
    messageCodeComponents = messageCodeSplitMagic(messageCode, &messageCodeComponentCount);

    // daca mesajul nu este perfect
    if (messageCodeComponents == NULL) {
        puts("0");
        return;
    }

    // sorteaza mesajele in ordine descrescatoare
    qsort(messageCodeComponents, messageCodeComponentCount, sizeof(messageCodeComponents[0]), compareDescending);

    // afiseaza componentele
    if (messageCodeComponentCount > 1) {
        for (unsigned i = 0; i <= messageCodeComponentCount-2; ++i) {
            printf("%llu ", messageCodeComponents[i]);
        }
    }
    printf("%llu\n", messageCodeComponents[messageCodeComponentCount-1]);

    free(messageCodeComponents);
}
#endif


int main(void) {
#if defined(TASK1) || defined(TASK3) || defined(TASK4)
    char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1];
    messageEncodeTableInit(messageEncodeTable); // initializeaza tabela de codificare
#endif

#ifdef TASK1
    {
        char message[MESSAGE_MAX_SIZE+1];
        read1(message);

        solve1(message, (const char (*)[])messageEncodeTable);
    }
#endif

#ifdef TASK2
    char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX];
    messageDecodeTableInit(messageDecodeTable); // initializeaza tabela de decodificare

    {
        char messageCode[MESSAGE_CODE_MAX_SIZE+1];
        read2(messageCode);
        
        solve2(messageCode, (const char (*)[])messageDecodeTable);
    }
#endif

#ifdef TASK3
    {
        struct Dict dict;

        char message[MESSAGE_MAX_SIZE+1];
        read3(message, &dict);

        solve3(message, &dict, (const char (*)[])messageEncodeTable);

        dictFree(&dict);
    }
#endif

#ifdef TASK4
    {
        char message[MESSAGE_MAX_SIZE+1];
        read4(message);

        solve4(message, (const char (*)[])messageEncodeTable);
    }
#endif
    
    return 0;
}
