#ifndef CSV_H
#define CSV_H

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>

#define CSV_COMMENT_MARKER '#'
#define CSV_FIELD_ENCLOSE_QUOTE '"'
#define CSV_DEFAULT_FIELD_SEPARATOR ','
#define CSV_FIELD_SEPARATOR_DEFINITION "sep=%c"

extern const char CSV_FIELD_ENCLOSE_QUOTE_STR[2];
extern const char CSV_FIELD_ESCAPED_QUOTE_STR[3];

struct CSV {
    size_t rowCount;
    size_t rowCountAlloc;
    struct CSVRow **rows;

    char fieldSeparator;
};

struct CSVRow {
    size_t fieldCount;
    size_t fieldCountAlloc;
    char **fields;
};

struct CSV* csvCreate(void);
void csvDestroy(struct CSV *csv);

struct CSV* csvRead(const char *filePath);
int csvWrite(const struct CSV *csv, const char *fileName);

const char* csvFieldGet(const struct CSV *csv, size_t rowIndex, size_t fieldIndex);
bool csvFieldSet(struct CSV *csv, size_t rowIndex, size_t fieldIndex, const char *field);

#endif // CSV_H