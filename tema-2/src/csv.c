#include "csv.h"

#define CSV_READ_BUF_SIZE 1024

#define CSV_INIT_ROW_COUNT_ALLOC 4
#define CSV_INIT_FIELD_COUNT_ALLOC 4

const char CSV_FIELD_ENCLOSE_QUOTE_STR[2] = {CSV_FIELD_ENCLOSE_QUOTE, '\0'};
const char CSV_FIELD_ESCAPED_QUOTE_STR[3] = {CSV_FIELD_ENCLOSE_QUOTE, CSV_FIELD_ENCLOSE_QUOTE, '\0'};

#include "err.h"
#include "str.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static_assert(CSV_READ_BUF_SIZE >= 2, "invalid size");

static_assert(CSV_INIT_ROW_COUNT_ALLOC >= 1, "invalid value");
static_assert(CSV_INIT_FIELD_COUNT_ALLOC >= 1, "invalid value");

static_assert(sizeof(CSV_FIELD_ENCLOSE_QUOTE_STR) <= sizeof(CSV_FIELD_ESCAPED_QUOTE_STR), "invalid sizes");


struct CSVRow* csvRowCreate(void);
void csvRowDestroy(struct CSVRow *row);


struct CSV* csvCreate(void) {
    struct CSV *csv = malloc(sizeof(*csv));
    if (NULL == csv) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_CREATE));

    csv->rowCount = 0;
    csv->rowCountAlloc = CSV_INIT_ROW_COUNT_ALLOC;
    csv->rows = malloc(csv->rowCountAlloc*sizeof(*csv->rows));
    if (NULL == csv->rows) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_CREATE));

    for (size_t i = 0; i < csv->rowCountAlloc; ++i) {
        struct CSVRow *row = csvRowCreate();
        // if (NULL == row) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_ROW_CREATE));

        csv->rows[i] = row;
    }

    csv->fieldSeparator = CSV_DEFAULT_FIELD_SEPARATOR;

    return csv;
}

void csvDestroy(struct CSV *csv) {
    assert(NULL != csv);

    for (size_t i = 0; i < csv->rowCountAlloc; ++i) csvRowDestroy(csv->rows[i]);
    free(csv->rows);

    free(csv);
}


struct CSV* csvRead(const char *filePath) {
    assert(NULL != filePath);

    struct CSV *csv = csvCreate();
    // if (NULL == csv) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_CREATE));

    FILE *file = fopen(filePath, "r");
    if (NULL == file) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ_FILE_OPEN));
    
    char buf[CSV_READ_BUF_SIZE] = {};
    size_t lineSizeAlloc = sizeof(buf);
    char *line = calloc(lineSizeAlloc, sizeof(*line));
    if (NULL == line) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ_FILE_OPEN));

    bool isFieldSeparatorDefinitionChecked = false;

    size_t rawRowCountAlloc = CSV_INIT_ROW_COUNT_ALLOC;
    char **rawRows = malloc(rawRowCountAlloc*sizeof(*rawRows));
    if (NULL == rawRows) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ));

    size_t rawRowIndex = 0;
    while (NULL != fgets(buf, sizeof(buf), file)) {
        if (strlen(line)+strlen(buf)+1 > lineSizeAlloc) { // daca marimea liniei este prea mica pentru noul continut ...
            line = realloc(line, (lineSizeAlloc *= 2)*sizeof(*line));
            if (NULL == line) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ));
        }
        
        strcat(line, buf); // adauga continutul citit la linia curenta
        if (NULL == strchr(buf, '\n')) { // daca nu am citit inca o linie intreaga ...
            continue; // mai citim pana am citit o line intreaga
        }
        
        // daca am ajuns aici inseamna ca am citit o line intreaga
        line[strcspn(line, "\n")] = '\0'; // inlatura caracterul '\n' de la sfarsitul liniei

        bool isLineEmpty = true;
        bool isLineComment = false;
        for (size_t i = 0; i < strlen(line); ++i) {
            if (!isspace(line[i])) {
                isLineEmpty = false;
                if (CSV_COMMENT_MARKER == line[i]) isLineComment = true;
                
                break;
            }
        }

        if (isLineEmpty || isLineComment) goto line_clear;

        // daca am ajuns aici inseamna ca linia nu este goala si nu este un comentariu

        // daca se doreste folosirea unui alt separator in loc de cel implicit
        // acesta trebuie specificat pe prima linie din fisier care nu e goala si nu e un comentariu,
        // inainte de toate definitiile field-urilor
        if (!isFieldSeparatorDefinitionChecked) {
            stringTrim(line); // sterge toate spatiile

            int res = sscanf(line, CSV_FIELD_SEPARATOR_DEFINITION, &csv->fieldSeparator);
            if (EOF == res || 0 == res) { // daca un separator nu a fost specificat pe prima linie ...
                rewind(file); // ne intoarcem la inceputul fisierului
            }
            // daca sscanf nu a putut captura separatorul in variabila specificata, aceasta ramane nemodificata

            isFieldSeparatorDefinitionChecked = true;

            goto line_clear;
        }

        if (rawRowIndex+1 > rawRowCountAlloc) {
            rawRows = realloc(rawRows, (rawRowCountAlloc *= 2)*sizeof(*rawRows));
            if (NULL == rawRows) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ));
        }

        rawRows[rawRowIndex++] = strdup(line);

    line_clear:
        memset(line, 0, lineSizeAlloc);
    }
    free(line);

    // daca nu am putut inchide fisierul ...
    if (EOF == fclose(file)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_READ_FILE_CLOSE));

    // daca nu exista randuri de procesat ...
    if (0 == rawRowIndex) return csv;

    // csv->rowCount = rawRowIndex;
    size_t rawRowCount = rawRowIndex;

    // spre deosebire de randuri unde putem avea fisiere fara niciun rand in cazul field-urilor
    // avem garantia ca exista cel putin 2 fielduri deoarece, daca un rand este gol, nu se ajunge aici

    rawRowIndex = 0;
    while (rawRowIndex < rawRowCount) {
        char *rawRowPtr = rawRows[rawRowIndex]; // pointer-ul de traversare a randului

        bool isRowLastFieldEmpty = false;
        
        size_t fieldIndex = 0;
        while ('\0' != *rawRowPtr) {
            bool isFieldQuoted = false;

            char *fieldStartPtr = rawRowPtr;

            if (CSV_FIELD_ENCLOSE_QUOTE == *rawRowPtr) { // daca am gasit un quote de deschidere ...
                ++rawRowPtr; // trecem de el
                
                // acest loop trece pointer-ul de traversare de field-uri cu quote-uri, daca ele exista
                while (true) {
                    char *ptr = strchr(rawRowPtr, CSV_FIELD_ENCLOSE_QUOTE); // cautam urmatorul quote

                    // daca nu mai exista quote-uri candidate pentru verificare ...
                    if (NULL == ptr) break;

                    ++ptr; // avansam pentru a ne uita la urmatorul caracter dupa quote-ul gasit

                    // intr-un fieiser CSV quote-urile dinauntrul unui field pus intre quote-uri
                    // se dubleaza (" devine "") pentru a nu se interpreta ca sfarsitul field-ului
                    if (CSV_FIELD_ENCLOSE_QUOTE == *ptr) { // daca quote-ul este dublu ...
                        rawRowPtr = ptr+1; // trecem de el
                        continue; // continuam cautarea quote-ului de inchidere
                    }

                    // aici ne putem uita dupa urmatorul separator de field-uri
                    if (*ptr == csv->fieldSeparator || '\0' == *ptr) { // daca urmatorul caracter este separatorul de field-uri
                                                                          // sau daca acesta e ultimul field  ...
                        // daca am ajuns aici inseamna ca am gasit qoute-ul de inchidere
                        rawRowPtr = ptr; // avansam pointer-ul de traversare

                        // daca am ajuns aici inseamna ca field-ul are quote-uri
                        isFieldQuoted = true;
                    }
                    
                    break;
                }
            }

            // aici ne putem uita dupa urmatorul separator de field-uri
            char *fieldEndPtr;

            char *ptr = strchrnul(rawRowPtr, csv->fieldSeparator);
            if ('\0' == *ptr) { // daca nu mai exista field-uri ...
                rawRowPtr = ptr; // sarim la sfarsitul randului (si implicit al field-ului)

                fieldEndPtr = rawRowPtr;
            } else {
                fieldEndPtr = ptr;

                rawRowPtr = ptr+1; // sarim peste separator

                // daca am ajuns la sfarsitul randului aici inseamna ca ultimul field este gol
                if ('\0' == *rawRowPtr) isRowLastFieldEmpty = true;
            }

            // aici putem extrage continutul field-ului din rand
            if (isFieldQuoted) {
                *(fieldEndPtr-1) = '\0';
                stringReplace(++fieldStartPtr, CSV_FIELD_ESCAPED_QUOTE_STR, CSV_FIELD_ENCLOSE_QUOTE_STR);
            } else {
                *fieldEndPtr = '\0';
            }
            if (!csvFieldSet(csv, rawRowIndex, fieldIndex++, fieldStartPtr)) {
                // error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_SET));
            }


            
            // daca am ajuns la sfarsitul randului si ultimul field este gol ...
            if ('\0' == *rawRowPtr && isRowLastFieldEmpty) {
                if (!csvFieldSet(csv, rawRowIndex, fieldIndex++, "")) {
                    // error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_SET));
                }
            }
        }
        free(rawRows[rawRowIndex++]);
    }
    free(rawRows);

    return csv;
}

int csvWrite(const struct CSV *csv, const char *fileName) {
    assert(NULL != csv);
    assert(NULL != fileName);

    FILE *file = fopen(fileName, "w");
    if (NULL == file) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE_FILE_OPEN));

    if (CSV_DEFAULT_FIELD_SEPARATOR != csv->fieldSeparator) {
        int res = fprintf(file, CSV_FIELD_SEPARATOR_DEFINITION, csv->fieldSeparator);
        if (res < 0) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE_FILE_WRITE));

        res = fputc('\n', file);
        if (EOF == res) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE_FILE_WRITE));
    }

    for (size_t rowIndex = 0; rowIndex < csv->rowCount; ++rowIndex) {
        const struct CSVRow *row = csv->rows[rowIndex];

        for (size_t fieldIndex = 0; fieldIndex < row->fieldCount; ++fieldIndex) {
            const char *field = csvFieldGet(csv, rowIndex, fieldIndex);
            if (NULL == field) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

            char *quotedField = NULL;
            if (NULL != strchr(field, csv->fieldSeparator)) { // daca exista cel putin un separator in field ...
                // punem quote-uri field-ului

                // daca field-ul are quote-uri in interior
                if (NULL != strchr(field, CSV_FIELD_ENCLOSE_QUOTE)) {
                    quotedField = stringReplaceNew(field, CSV_FIELD_ENCLOSE_QUOTE_STR, CSV_FIELD_ESCAPED_QUOTE_STR);
                } else {
                    quotedField = strdup(field);
                }
                if (NULL == quotedField) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE));

                size_t quotedFieldLen = strlen(quotedField);
                quotedField = realloc(quotedField, 1+quotedFieldLen+1+1);
                if (NULL == quotedField) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE));

                memmove(quotedField+1, quotedField, quotedFieldLen+1);
                quotedField[0] = quotedField[1+quotedFieldLen] = CSV_FIELD_ENCLOSE_QUOTE;
                quotedField[1+quotedFieldLen+1] = '\0';
            }

            int res = fputs(NULL != quotedField ? quotedField : field, file);
            if (EOF == res) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE_FILE_WRITE));

            if (NULL != quotedField) free(quotedField);

            if (fieldIndex == row->fieldCount-1) continue;

            res = fputc(csv->fieldSeparator, file);
            if (EOF == res) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE_FILE_WRITE));
        }

        int res = fputc('\n', file);
        if (EOF == res) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE_FILE_WRITE));
    }

    // daca nu am putut inchide fisierul ...
    if (EOF == fclose(file)) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_WRITE_FILE_CLOSE));

    return 1;
}


struct CSVRow* csvRowCreate(void) {
    struct CSVRow *row = malloc(sizeof(*row));
    if (NULL == row) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_ROW_CREATE));

    row->fieldCount = 0;
    row->fieldCountAlloc = CSV_INIT_FIELD_COUNT_ALLOC;
    row->fields = malloc(row->fieldCountAlloc*sizeof(*row->fields));
    if (NULL == row->fields) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_ROW_CREATE));

    for (size_t i = 0; i < row->fieldCountAlloc; ++i) row->fields[i] = NULL;

    return row;
}

void csvRowDestroy(struct CSVRow *row) {
    assert(NULL != row);

    for (size_t i = 0; i < row->fieldCount; ++i) {
        free(row->fields[i]);
    }
    free(row->fields);

    free(row);
}


const char* csvFieldGet(const struct CSV *csv, size_t rowIndex, size_t fieldIndex) {
    assert(NULL != csv);
    assert(rowIndex >= 0 && rowIndex < csv->rowCount);

    const struct CSVRow *row = csv->rows[rowIndex];
    assert(fieldIndex >= 0 && fieldIndex < row->fieldCount);

    if (NULL == row->fields[fieldIndex]) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_GET));

    return row->fields[fieldIndex];
}

bool csvFieldSet(struct CSV *csv, size_t rowIndex, size_t fieldIndex, const char *field) {
    assert(NULL != csv);
    assert(rowIndex >= 0 && rowIndex <= csv->rowCount);

    if (rowIndex == csv->rowCount) ++csv->rowCount;

    if (csv->rowCount > csv->rowCountAlloc) {
        csv->rows = realloc(csv->rows, (csv->rowCountAlloc *= 2)*sizeof(*csv->rows));
        if (NULL == csv->rows) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_SET));

        for (size_t i = rowIndex; i < csv->rowCountAlloc; ++i) {
            struct CSVRow *row = csvRowCreate();
            // if (NULL == row) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_ROW_CREATE));

            csv->rows[i] = row;
        }
    }

    struct CSVRow *row = csv->rows[rowIndex];
    assert(fieldIndex >= 0 && fieldIndex <= row->fieldCount);
    
    if (fieldIndex == row->fieldCount) ++row->fieldCount;

    if (row->fieldCount > row->fieldCountAlloc) {
        row->fields = realloc(row->fields, (row->fieldCountAlloc *= 2)*sizeof(*row->fields));
        if (NULL == row->fields) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_SET));

        for (size_t i = fieldIndex; i < row->fieldCountAlloc; ++i) row->fields[i] = NULL;
    }

    if (NULL != row->fields[fieldIndex]) free(row->fields[fieldIndex]);

    row->fields[fieldIndex] = strdup(field);
    if (NULL == row->fields[fieldIndex]) error_at_line(EXIT_FAILURE, errno, __FILE__, __LINE__, "%s", errorGetMessage(ERROR_CSV_FIELD_SET));

    return true;
}
