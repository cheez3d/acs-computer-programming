#include "str.h"

#include "err.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <error.h>
#include <stdlib.h>
#include <string.h>


char* stringDelete(char *str, const char *pat) {
    char *strPtr = NULL;
    while (NULL != (strPtr = strstr(str, pat))) {
        memmove(strPtr, strPtr+strlen(pat), strlen(strPtr+strlen(pat))+1);
    }

    return str;
}


char* stringReplace(char *str, const char *pat, const char *rep) {
    size_t patLen = strlen(pat);
    size_t repLen = strlen(rep);

    assert(repLen <= patLen);

    char *strPtr = str;
    while (NULL != (strPtr = strstr(strPtr, pat))) {
        memcpy(strPtr, rep, repLen);
        strPtr += repLen;
        if (patLen > repLen) memmove(strPtr, strPtr+(patLen-repLen), strlen(strPtr+(patLen-repLen))+1);
    }

    return str;
}

char* stringReplaceNew(const char *str, const char *pat, const char *rep) {
    const char *strPtr = str;
    
    size_t hits = 0;
    while (NULL != (strPtr = strstr(strPtr, pat))) {
        ++hits;
        strPtr += strlen(pat);
    }

    char *res = calloc(strlen(str)-hits*strlen(pat)+hits*strlen(rep)+1, sizeof(*res));
    if (NULL == res) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_STRING_REPLACE_NEW));

    const char *strPrevPtr;
    strPtr = str;
    char *resPtr = res;
    while (NULL != (strPrevPtr = strPtr, strPtr = strstr(strPtr, pat))) {
        strncpy(resPtr, strPrevPtr, strPtr-strPrevPtr);
        resPtr += strPtr-strPrevPtr;

        strcpy(resPtr, rep);

        resPtr += strlen(rep);
        strPtr += strlen(pat);
    }
    strcpy(resPtr, strPrevPtr);

    return res;
}


char* stringTrim(char *str) {
    char *strPtr = str;
    while ('\0' != *strPtr) {
        if (isspace(*strPtr)) memmove(strPtr, strPtr+1, strlen(strPtr+1)+1);
        else ++strPtr;
    }

    return str;
}
