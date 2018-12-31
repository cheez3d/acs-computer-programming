#include "err.h"

const char* errorGetMessage(enum Error error) {
    switch (error) {
        // CSV
        case ERROR_CSV_CREATE: return "could not create CSV";

        case ERROR_CSV_READ: return "could not read CSV";
        case ERROR_CSV_READ_FILE_OPEN: return "could not open CSV file for reading";
        case ERROR_CSV_READ_FILE_CLOSE: return "could not close CSV file";

        case ERROR_CSV_WRITE: return "could not write CSV";
        case ERROR_CSV_WRITE_FILE_OPEN: return "could not open CSV file for writing";
        case ERROR_CSV_WRITE_FILE_CLOSE: return "could not close CSV file";
        case ERROR_CSV_WRITE_FILE_WRITE: return "could not write to CSV file";

        case ERROR_CSV_ROW_CREATE: return "could not create CSV row";

        case ERROR_CSV_FIELD_GET: return "could not get CSV field";
        case ERROR_CSV_FIELD_SET: return "could not set CSV field";


        // LANG
        case ERROR_LANG_CREATE: return "could not create Lang";
        case ERROR_LANG_CREATE_TOO_FEW_ROWS: return "Lang file contains too few rows";
        case ERROR_LANG_CREATE_TOO_FEW_FIELDS: return "Lang file contains too few fields";
        case ERROR_LANG_CREATE_INVALID_HEADER: return "Lang file contains invalid headers";
        case ERROR_LANG_CREATE_INVALID_CORRECT_ANSWER: return "Lang file contains question invalid correct answer";

        case ERROR_LANG_INTERFACE_TEXT_GET: return "could not get Lang interface text";


        // GAME
        case ERROR_GAME_CREATE: return "could not create Game";
        case ERROR_GAME_LANG_SET: return "could not set Game lang";


        // STRING
        case ERROR_STRING_REPLACE_NEW: return "could not replace pattern";


        default: return "failure";
    }
}
