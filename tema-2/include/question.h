#ifndef QUESTION_H
#define QUESTION_H

#include <assert.h>
#include <stddef.h>

#define QUESTION_ANSWER_COUNT 4

static_assert(QUESTION_ANSWER_COUNT >= 0 && QUESTION_ANSWER_COUNT <= 9, "invalid value");

struct Question {
    const char *text;

    const char *answers[QUESTION_ANSWER_COUNT];
    size_t correctAnswer;
};

#endif // QUESTION_H