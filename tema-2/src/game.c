#include "game.h"

#include "csv.h"
#include "err.h"
#include "lang.h"
#include "question.h"

#include <curses.h>
#include <menu.h>

#include <assert.h>
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct Game* gameCreate(void) {
    struct Game *game = malloc(sizeof(*game));
    if (NULL == game) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_GAME_CREATE));

    game->lang = NULL;

    game->questionCount = 0;
    game->questions = NULL;


    game->questionIndex = 0;
    game->score = 0;
    game->hasSkip = true;
    game->hasFiftyFifty = true;
    game->usingFiftyFifty = false;

    game->right = 0;
    game->wrong = 0;

    return game;
}

void gameDestroy(struct Game *game) {
    assert(NULL != game);

    for (size_t i = 0; i < game->questionCount; ++i) free(game->questions[i]);
    free(game->questions);

    free(game);
}

void game_start(struct Game *game) {
    WINDOW *questionWin = newwin(10, 70, (LINES-10)/2, (COLS-70)/2);
    keypad(questionWin, TRUE);

    WINDOW *questionSub = derwin(questionWin, 7, 70, 2, 0);

    bool isQuitRequested = false;

    const struct Question *question;
    while (!isQuitRequested && NULL != (question = gameQuestionGet(game))) {
        mvwprintw(questionWin, 0, 0, "%s", question->text);
        mvwprintw(questionWin, 7, 0, "scor %u | corecte %u | gresite %u\nq - meniu\na, b, c, d, enter - alege răspuns  | %s | %s",
            game->score, game->right, game->wrong,
            game->hasSkip ? "z - sări întrebarea" : "folosit",
            game->hasFiftyFifty ? "x - 50/50" : "folosit");

        size_t fiftyFiftyLeft = 2;
        ITEM *answerItems[QUESTION_ANSWER_COUNT];
        for (size_t i = 0; i < QUESTION_ANSWER_COUNT; ++i) {
            // const char answerName[3] = {'a'+i, ')', '\0'};
            bool fiftyFiftyStriked = game->usingFiftyFifty && i != question->correctAnswer && fiftyFiftyLeft-- > 0;

            const char *answerText = fiftyFiftyStriked ? "-" : question->answers[i];
            ITEM *answerItem = new_item(answerText, "");
            if (!fiftyFiftyStriked) {
                size_t *answerIndexPtr = malloc(sizeof(*answerIndexPtr));
                *answerIndexPtr = i;
                set_item_userptr(answerItem, answerIndexPtr);
            } else {
                set_item_userptr(answerItem, NULL);
            }

            answerItems[i] = answerItem;
        }

        if (game->usingFiftyFifty) game->usingFiftyFifty = false;

        MENU *answerMenu = new_menu(answerItems);
        set_menu_mark(answerMenu, NULL);
        set_menu_win(answerMenu, questionWin);
        set_menu_sub(answerMenu, questionSub);

        post_menu(answerMenu);
        wrefresh(questionWin);

        while (!isQuitRequested) {
            int key = wgetch(questionWin);

            size_t selectedAnswer;
            switch (key) {
                case KEY_UP:
                    menu_driver(answerMenu, REQ_UP_ITEM);
                    break;

                case KEY_DOWN:
                    menu_driver(answerMenu, REQ_DOWN_ITEM);
                    break;

                case 'q':
                    // delwin(questionWin);
                    isQuitRequested = true;
                    continue;

                case 'z':
                    if (game->hasSkip) {
                        game->hasSkip = false;
                        goto next_question_skip;
                    }
                    break;

                case 'x':
                    if (game->hasFiftyFifty) {
                        game->hasFiftyFifty = false;
                        game->usingFiftyFifty = true;
                        
                        goto for_fifty_fifty;
                    }
                    break;

                case '\n':
                case KEY_ENTER: {
                    size_t *selectedAnswerPtr = (size_t*)item_userptr(current_item(answerMenu));

                    if (NULL == selectedAnswerPtr) continue;
                    selectedAnswer = *selectedAnswerPtr;

                    goto next_question;
                }

                default: {
                    if (key >= 'a' && key-'a' < QUESTION_ANSWER_COUNT) {
                        selectedAnswer = key-'a';

                        if (NULL == item_userptr(answerItems[selectedAnswer])) {
                            continue;
                        }

                    next_question:
                        if (selectedAnswer == question->correctAnswer) {
                            game->score += 10;
                            ++game->right;
                        } else {
                            game->score -= game->score < 5 ? game->score : 5;
                            ++game->wrong;
                        }

                    next_question_skip:
                        ++game->questionIndex;
                    for_fifty_fifty:
                        unpost_menu(answerMenu);
                        wclear(questionWin);
                        goto end;
                    }
                }
            }

            wrefresh(questionWin);
        }
    end:;
        for (size_t i = 0; i < QUESTION_ANSWER_COUNT; ++i) {
            if (NULL != item_userptr(answerItems[i])) free(item_userptr(answerItems[i]));

            free_item(answerItems[i]);
        }

        free_menu(answerMenu);

        delwin(questionWin);
        clear();
    }
}

void game_quit(const struct Game *game) {
    // TODO: find a way to break loop and continue execution
    //       from there instead of doing this
    endwin();

    exit(EXIT_SUCCESS);
}

void gameStart(struct Game *game) {
    WINDOW *menuWin = initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    // start_color();
    // init_pair(1, COLOR_RED, COLOR_BLACK);

    keypad(menuWin, TRUE);

    WINDOW *menuSub = derwin(menuWin, 10, 70, (LINES-10)/2, (COLS-70)/2);

    size_t menuItemCount = 0;
    ITEM *menuItems[4];
    for (size_t i = 0; i < sizeof(menuItems); ++i) menuItems[i] = NULL;

    const char *newGameName = gameTextGet(game, "menu", "new_game");
    const char *newGameDesc = gameTextGet(game, "menu", "new_game_desc");
    ITEM *newGameItem = new_item(newGameName, newGameDesc);
    set_item_userptr(newGameItem, "start");
    menuItems[menuItemCount++] = newGameItem;

    const char *quitName = gameTextGet(game, "menu", "quit");
    const char *quitDesc = gameTextGet(game, "menu", "quit_desc");
    ITEM *quitItem = new_item(quitName, quitDesc);
    set_item_userptr(quitItem, "quit");
    menuItems[menuItemCount++] = quitItem;

    MENU *menu = new_menu(menuItems);
    set_menu_mark(menu, NULL);
    set_menu_win(menu, menuWin);
    set_menu_sub(menu, menuSub);

     // TODO: same code 0x0
    post_menu(menu);
    wrefresh(menuWin);
    refresh();
    
    while (true) {
        int key = getch();

        if (KEY_UP == key) {
            menu_driver(menu, REQ_UP_ITEM);
            continue;
        }

        if (KEY_DOWN == key) {
            menu_driver(menu, REQ_DOWN_ITEM);
            continue;
        }

        if ('\n' == key || KEY_ENTER == key) {
            ITEM *item = current_item(menu);
            void *userPtr = item_userptr(item);

            unpost_menu(menu);
            wrefresh(menuWin);
            refresh();

            const char *str = (const char*)userPtr;
            if (0 == strcmp(str, "start")) {
                if (0 == game->questionIndex) {
                    game->score = 0;
                    game->hasSkip = true;
                    game->hasFiftyFifty = true;

                    game->right = 0;
                    game->wrong = 0;
                }

                game_start(game);
            }
            else if (0 == strcmp(str, "quit")) game_quit(game);

            // TODO: same code 0x0
            clear(); // TODO: clear vs erase?
            post_menu(menu);
            mvwprintw(menuSub, 7, 0, "scor %u | corecte %u | gresite %u",
                game->score, game->right, game->wrong);
            wrefresh(menuWin);
            refresh();

            continue;
        }
    }

    for (size_t i = 0; i < menuItemCount; ++i) free_item(menuItems[i]);
    free_menu(menu);

    delwin(menuWin);

    endwin();
}


bool gameLangSet(struct Game *game, const struct Lang *lang) {
    assert(NULL != lang);

    game->lang = lang;

    size_t questionCount = lang->questionData->rowCount-1;
    for (size_t i = questionCount; i < game->questionCount; ++i) free(game->questions[i]);

    if (questionCount > game->questionCount) {
        struct Question **questions = realloc(game->questions, questionCount*sizeof(*questions));
        if (NULL == questions) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

        // TODO: when questionCount < game->questionCount free the struct Question-s that are malloc-ed below
        for (size_t i = game->questionCount; i < questionCount; ++i) {
            struct Question *question = malloc(sizeof(*question));
            if (NULL == question) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE));

            questions[i] = question;
        }

        game->questions = questions;
    }

    game->questionCount = questionCount;

    for (size_t rowIndex = 1; rowIndex < lang->questionData->rowCount; ++rowIndex) {
        struct Question *question = game->questions[rowIndex-1];

        const char *text = csvFieldGet(lang->questionData, rowIndex, LANG_QUESTION_DATA_CSV_FIELD_QUESTION);
        if (NULL == text) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

        question->text = text;

        for (size_t i = 0; i < QUESTION_ANSWER_COUNT; ++i) {
            const char *answer = csvFieldGet(lang->questionData, rowIndex, LANG_QUESTION_DATA_CSV_FIELD_QUESTION+1+i);
            if (NULL == answer) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

            question->answers[i] = answer;
        }

        const char *correctAnswerStr = csvFieldGet(lang->questionData, rowIndex, LANG_QUESTION_DATA_CSV_FIELD_CORRECT_ANSWER);
        if (NULL == correctAnswerStr) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

        size_t correctAnswer = strtoul(correctAnswerStr, NULL, 10);
        if (correctAnswer >= QUESTION_ANSWER_COUNT) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_LANG_CREATE_INVALID_CORRECT_ANSWER));

        question->correctAnswer = correctAnswer;
    }

    return true;
}


const char* gameTextGet(const struct Game *game, const char *category, const char *key) {
    // TODO: add error handling
    return langInterfaceTextGet(game->lang, category, key);
}


const struct Question* gameQuestionGet(struct Game *game) {
    // TODO: add error handling
    if (game->questionIndex < game->questionCount) {
        return game->questions[game->questionIndex];
    } else {
        game->questionIndex = 0;

        return NULL;
    }
}
