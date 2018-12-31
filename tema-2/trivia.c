#include "err.h"
#include "game.h"
#include "lang.h"

#include <dirent.h>
#include <errno.h>
#include <error.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main(int argc, char **argv) {
    setlocale(LC_ALL, ""); // pentru ca curses sa poata afisa caractere UTF-8 multi-byte
    srandom((unsigned)time(NULL)); 

    size_t langCount = 0;
    size_t langCountAlloc = 0;
    struct Lang **langs = NULL; // TODO: make like this in csv, lang and only use realloc calls, no malloc calls

    DIR *langDir = opendir(LANG_DIR_PATH);
    if (NULL == langDir) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(-1));
    
    struct dirent *entry;
    while (NULL != (entry = readdir(langDir))) {
        if (entry->d_type != DT_DIR) continue;
        if (0 == strcmp(entry->d_name, ".")) continue;
        if (0 == strcmp(entry->d_name, "..")) continue;
        
        struct Lang *lang = langCreate(entry->d_name);
        // if (NULL == lang) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

        if (langCount+1 > langCountAlloc) {
            langCountAlloc = 0 == langCountAlloc ? 1 : langCountAlloc*2;
            langs = realloc(langs, langCountAlloc*sizeof(*langs));
        }

        langs[langCount++] = lang;
    }

    if (-1 == closedir(langDir)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(-1));

    if (0 == langCount) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(-1));

    struct Game *game = gameCreate();

    for (size_t i = 0; i < langCount; ++i) {
        if (0 == strcmp(langs[i]->name, GAME_DEFAULT_LANG)) {
            if (!gameLangSet(game, langs[i])) {
                // error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_GAME_LANG_SET));
            }
        }
    }
    if (NULL == game->lang) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(-1));


    gameStart(game); // loop-ul jocului


    gameDestroy(game);    

    for (size_t i = 0; i < langCount; ++i) langDestroy(langs[i]);
    free(langs);

    exit(EXIT_SUCCESS);
}
