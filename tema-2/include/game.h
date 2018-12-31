#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stddef.h>

#define GAME_DEFAULT_LANG "romanian"

struct Game {
    const struct Lang *lang;

    size_t questionCount;
    struct Question **questions;


    size_t questionIndex;
    unsigned score;
    bool hasSkip;
    bool hasFiftyFifty;
    bool usingFiftyFifty;

    size_t right;
    size_t wrong;
};

struct Game* gameCreate(void);
void gameDestroy(struct Game *game);

void gameStart(struct Game *game);

bool gameLangSet(struct Game *game, const struct Lang *lang);

const char* gameTextGet(const struct Game *game, const char *category, const char *key);

const struct Question* gameQuestionGet(struct Game *game);

#endif // GAME_H
