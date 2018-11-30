#ifndef MESSAGE_H
#define MESSAGE_H

#define MESSAGE_ENCODE_TABLE_SIZE ('z'-'a')+1
#define MESSAGE_DECODE_TABLE_SIZE LAST_DIGIT-FIRST_DIGIT+1

#define MESSAGE_MAX_SIZE 100 // lungimea maxima a unui mesaj
#define MESSAGE_CODE_MAX_SIZE 300 // lungimea maxima a unui mesaj codificat

// separatorul folosit atunci cand in cod exista doua litere de pe aceeasi tasta succesiv
#define MESSAGE_CODE_CONFLICT_SEPARATOR '#'

#include "letter.h"
#include "dict.h"

extern const char MESSAGE_CODE_CONFLICT_SEPARATOR_STR[2]; // sirul de caractere corespunzator lui MESSAGE_CODE_CONFLICT_SEPARATOR

/**
 * Initializeaza tabela de codificare in variabila specificata ca argument.
 */
void messageEncodeTableInit(char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]);

/**
 * Initializeaza tabela de decodificare in variabila specificata ca argument.
 */
void messageDecodeTableInit(char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX]);

/**
 * Codifica mesajul message cu ajutorul tabelei de codificare messageEncodeTable
 * si stocheaza rezultatul in messageCode.
 * 
 * Returneaza un pointer catre mesajul codificat stocat in messageCode.
 **/
char* messageEncode(
    const char message[MESSAGE_MAX_SIZE+1], char messageCode[MESSAGE_CODE_MAX_SIZE+1],
    const char messageEncodeTable[MESSAGE_ENCODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX+1]
);

/**
 * Decodifica mesajul codificat messageCode cu ajutorul tabelei de decodificare messageDecodeTable
 * si stocheaza rezultatul in message.
 * 
 * Returneaza un pointer catre mesajul decodificat stocat in message.
 **/
char* messageDecode(
    const char messageCode[MESSAGE_CODE_MAX_SIZE+1], char message[MESSAGE_MAX_SIZE+1],
    const char messageDecodeTable[MESSAGE_DECODE_TABLE_SIZE][LETTERS_PER_DIGIT_MAX]
);

/**
 * Corecteaza greselile din mesajul message cu ajutorul dictionarului dict.
 * Modifica direct mesajul dat ca parametru, necreand o copie a acestuia.
 * 
 * Returneaza un pointer catre message.
 **/
char* messageCorrect(char message[MESSAGE_MAX_SIZE+1], const struct Dict *const dict);

/**
 * Inlatura toate separatoarele MESSAGE_CODE_CONFLICT_SEPARATOR din mesajul codificat messageCode.
 * Modifica direct mesajul dat ca parametru, necreand o copie a acestuia.
 * 
 * Returneaza un pointer catre messageCode.
 **/
char* messageCodeStrip(char messageCode[MESSAGE_CODE_MAX_SIZE+1]);

/**
 * Imparte mesajul codificat messageCode in componentele sale corespunzatoare si stocheaza
 * numarul de componente in messageCodeComponentCount.
 * 
 * Returneaza un pointer catre zona de memorie ce contine toate componentele.
 **/
unsigned long long* messageCodeSplit(const char messageCode[MESSAGE_CODE_MAX_SIZE+1], unsigned *messageCodeComponentCount);

/**
 * Imparte mesajul codificat messageCode in componentele sale corespunzatoare
 * dupa regula numarului magic si stocheaza numarul de componente in messageCodeComponentCount.
 * 
 * Returneaza un pointer catre zona de memorie ce contine toate componentele
 * sau pointer catre NULL daca mesajul nu este perfect.
 **/
unsigned long long* messageCodeSplitMagic(const char messageCode[MESSAGE_CODE_MAX_SIZE+1], unsigned *messageCodeComponentCount);

/**
 * Verifica daca mesajul este prim, adica daca toate componentele sale
 * sunt prime intre ele, doua cate doua.
 **/
bool isMessageCodePrime(unsigned long long *messageCodeComponents, unsigned messageCodeComponentCount);

#endif
