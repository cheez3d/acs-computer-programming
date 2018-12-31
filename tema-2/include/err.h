#ifndef ERR_H
#define ERR_H

#include <assert.h>

enum Error {
    ERROR_CSV_CREATE,

    ERROR_CSV_READ, 
    ERROR_CSV_READ_FILE_OPEN,
    ERROR_CSV_READ_FILE_CLOSE,

    ERROR_CSV_WRITE,
    ERROR_CSV_WRITE_FILE_OPEN,
    ERROR_CSV_WRITE_FILE_CLOSE,
    ERROR_CSV_WRITE_FILE_WRITE,

    ERROR_CSV_ROW_CREATE,

    ERROR_CSV_FIELD_GET,
    ERROR_CSV_FIELD_SET,


    ERROR_LANG_CREATE,
    ERROR_LANG_CREATE_TOO_FEW_ROWS,
    ERROR_LANG_CREATE_TOO_FEW_FIELDS,
    ERROR_LANG_CREATE_INVALID_HEADER,
    ERROR_LANG_CREATE_INVALID_CORRECT_ANSWER,

    ERROR_LANG_INTERFACE_TEXT_GET,


    ERROR_GAME_CREATE,
    ERROR_GAME_LANG_SET,


    ERROR_STRING_REPLACE_NEW,
};

const char* errorGetMessage(enum Error error);

#endif // ERR_H

// TODO: make a globally accessible error code stack and a function for printing it and terminating the program (errorPrintStack)
