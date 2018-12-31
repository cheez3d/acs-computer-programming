#ifndef LANG_H
#define LANG_H

#include "question.h"

#include <stddef.h>

#define LANG_DIR_PATH "./data/lang"

#define LANG_INTERFACE_FILE_NAME "interface.csv"
#define LANG_QUESTIONS_FILE_NAME "questions.csv"

#define LANG_INTERFACE_DATA_CSV_FIELD_CATEGORY 0
#define LANG_INTERFACE_DATA_CSV_FIELD_KEY 1
#define LANG_INTERFACE_DATA_CSV_FIELD_TEXT 2

#define LANG_INTERFACE_DATA_CSV_HEADER_CATEGORY "category"
#define LANG_INTERFACE_DATA_CSV_HEADER_KEY "key"
#define LANG_INTERFACE_DATA_CSV_HEADER_TEXT "text"

#define LANG_QUESTION_DATA_CSV_FIELD_QUESTION 0
#define LANG_QUESTION_DATA_CSV_FIELD_CORRECT_ANSWER LANG_QUESTION_DATA_CSV_FIELD_QUESTION+QUESTION_ANSWER_COUNT+1

#define LANG_QUESTION_DATA_CSV_HEADER_QUESTION "question"
#define LANG_QUESTION_DATA_CSV_HEADER_ANSWER "answer"
#define LANG_QUESTION_DATA_CSV_HEADER_CORRECT_ANSWER "correctAnswer"

struct Lang {
    char *name;

    struct CSV *interfaceData;
    struct CSV *questionData;
};

struct Lang* langCreate(const char *langName);
void langDestroy(struct Lang *lang);

const char* langInterfaceTextGet(const struct Lang *lang, const char *category, const char *key);

#endif // LANG_H