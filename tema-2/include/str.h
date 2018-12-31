#ifndef STR_H
#define STR_H

char* stringDelete(char *str, const char *pat);
char* stringReplace(char *str, const char *pat, const char *rep);
char* stringReplaceNew(const char *str, const char *pat, const char *rep);
char* stringTrim(char *str);

#endif // STR_H
