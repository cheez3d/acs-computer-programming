#include <stdlib.h>
#include <string.h>

#include "message.h"
#include "dict.h"

void dictInit(struct Dict *const dict, unsigned entryCount) {
    dict->entries = (struct DictEntry*)malloc(entryCount*sizeof(struct DictEntry));

    dict->size = 0;
    dict->maxSize = entryCount;
}

void dictAddEntry(struct Dict *const dict, struct DictEntry dictEntry) {
    if (dict->size+1 <= dict->maxSize) {
        dict->entries[dict->size++] = dictEntry;
    }
}

const struct DictEntry *const dictGetEntry(const struct Dict *const dict, unsigned entryIndex) {
    return entryIndex < dict->size ? dict->entries+entryIndex : NULL;
}

const struct DictEntry *const dictGetEntryByTarget(const struct Dict *const dict, const char *target) {
    for (unsigned i = 0; i < dict->size; ++i) {
        if (strcmp(dict->entries[i].target, target) == 0) { // daca cuvantul a fost gasit in dictionar
            return dict->entries+i;
        }
    }

    return NULL;
}

void dictFree(const struct Dict *const dict) {
    for (unsigned i = 0; i < dict->size; ++i) {
        dictEntryFree(dict->entries+i);
    }

    free(dict->entries);
}

void dictEntryInit(struct DictEntry *const dictEntry, char target[MESSAGE_MAX_SIZE+1], char subst[MESSAGE_MAX_SIZE+1]) {
    dictEntry->target = strdup(target);
    dictEntry->subst = strdup(subst);
}

void dictEntryFree(const struct DictEntry *const dictEntry) {
    free(dictEntry->target);
    free(dictEntry->subst);
}
