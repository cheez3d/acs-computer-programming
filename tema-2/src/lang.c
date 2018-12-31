#include "lang.h"

#include "csv.h"
#include "err.h"
#include "question.h"

#include <assert.h>
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Lang* langCreate(const char *langName) {
    assert(NULL != langName);

    struct Lang *lang = malloc(sizeof(*lang));
    if (NULL == lang) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

    lang->name = strdup(langName);
    if (NULL == lang->name) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

    lang->interfaceData = NULL;
    lang->questionData = NULL;


    char *langPath = calloc(strlen(LANG_DIR_PATH)+1+strlen(langName)+1, sizeof(*langPath));
    if (NULL == langPath) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

    strcat(langPath, LANG_DIR_PATH);
    strcat(langPath, "/");
    strcat(langPath, langName);

    char *interfacePath = calloc(strlen(langPath)+1+strlen(LANG_INTERFACE_FILE_NAME)+1, sizeof(*interfacePath));
    if (NULL == interfacePath) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

    strcat(interfacePath, langPath);
    strcat(interfacePath, "/");
    strcat(interfacePath, LANG_INTERFACE_FILE_NAME);

    char *questionsPath = calloc(strlen(langPath)+1+strlen(LANG_INTERFACE_FILE_NAME)+1, sizeof(*questionsPath));
    if (NULL == questionsPath) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

    strcat(questionsPath, langPath);
    strcat(questionsPath, "/");
    strcat(questionsPath, LANG_QUESTIONS_FILE_NAME);

    free(langPath);


    // CITIRE INTERFATA
    struct CSV *interfaceData = csvRead(interfacePath);
    // if (NULL == lang->interface) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ));
    free(interfacePath);

    if (0 == interfaceData->rowCount) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_TOO_FEW_ROWS));
    for (size_t i = 0; i < interfaceData->rowCount; ++i) {
        if (interfaceData->rows[i]->fieldCount < LANG_INTERFACE_DATA_CSV_FIELD_TEXT+1) {
            error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_TOO_FEW_FIELDS));
        }
    }

    const char *headerCategory = csvFieldGet(interfaceData, 0, LANG_INTERFACE_DATA_CSV_FIELD_CATEGORY);
    // if (NULL == headerCategory) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

    if (0 != strcmp(headerCategory, LANG_INTERFACE_DATA_CSV_HEADER_CATEGORY)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_INVALID_HEADER));

    const char *headerKey = csvFieldGet(interfaceData, 0, LANG_INTERFACE_DATA_CSV_FIELD_KEY);
    // if (NULL == headerKey) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

    if (0 != strcmp(headerKey, LANG_INTERFACE_DATA_CSV_HEADER_KEY)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_INVALID_HEADER));

    const char *headerText = csvFieldGet(interfaceData, 0, LANG_INTERFACE_DATA_CSV_FIELD_TEXT);
    // if (NULL == headerText) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

    if (0 != strcmp(headerText, LANG_INTERFACE_DATA_CSV_HEADER_TEXT)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_INVALID_HEADER));

    lang->interfaceData = interfaceData;


    // CITIRE INTREBARI
    struct CSV *questionData = csvRead(questionsPath);
    // if (NULL == questionData) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ));
    free(questionsPath);

    if (0 == questionData->rowCount) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_TOO_FEW_ROWS));
    for (size_t i = 0; i < questionData->rowCount; ++i) {
        if (questionData->rows[i]->fieldCount < LANG_QUESTION_DATA_CSV_FIELD_CORRECT_ANSWER+1) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_TOO_FEW_FIELDS));
    }
    
    const char *headerQuestion = csvFieldGet(questionData, 0, LANG_QUESTION_DATA_CSV_FIELD_QUESTION);
    // if (NULL == headerQuestion) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

    if (0 != strcmp(headerQuestion, LANG_QUESTION_DATA_CSV_HEADER_QUESTION)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_INVALID_HEADER));
    
    for (size_t i = 0; i < QUESTION_ANSWER_COUNT; ++i) {
        const char *headerAnswer = csvFieldGet(questionData, 0, LANG_QUESTION_DATA_CSV_FIELD_QUESTION+1+i);
        // if (NULL == headerAnswer) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

        if (0 != strcmp(headerAnswer, LANG_QUESTION_DATA_CSV_HEADER_ANSWER)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_INVALID_HEADER));
    }

    const char *headerCorrectAnswer = csvFieldGet(questionData, 0, LANG_QUESTION_DATA_CSV_FIELD_CORRECT_ANSWER);
    // if (NULL == headerQuestion) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

    if (0 != strcmp(headerCorrectAnswer, LANG_QUESTION_DATA_CSV_HEADER_CORRECT_ANSWER)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_INVALID_HEADER));

    lang->questionData = questionData;


    return lang;
}


void langDestroy(struct Lang *lang) {
    assert(NULL != lang);

    free(lang->name);

    csvDestroy(lang->interfaceData);
    csvDestroy(lang->questionData);

    free(lang);
}


const char* langInterfaceTextGet(const struct Lang *lang, const char *category, const char *key) {
    assert(NULL != lang);

    for (size_t i = 1; i < lang->interfaceData->rowCount; ++i) {
        if (0 != strcmp(csvFieldGet(lang->interfaceData, i, LANG_INTERFACE_DATA_CSV_FIELD_CATEGORY), category)) continue;
        if (0 != strcmp(csvFieldGet(lang->interfaceData, i, LANG_INTERFACE_DATA_CSV_FIELD_KEY), key)) continue;

        return csvFieldGet(lang->interfaceData, i, LANG_INTERFACE_DATA_CSV_FIELD_TEXT);
    }

    error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_INTERFACE_TEXT_GET));

    return NULL;
}
