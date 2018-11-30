#ifndef DICT_H
#define DICT_H

#include "message.h"

struct Dict {
    struct DictEntry *entries; // intrarile dictionarului
    unsigned size; // dimensiunea curenta a dictionarului
    unsigned maxSize; // dimensiunea maxima pe care o poate avea dictionarul
};

struct DictEntry {
    char *target; // cuvantul de inlocuit
    char *subst; // cuvantul cu care se inlocuieste
};

/**
 * Initializeaza dimensiunea, dimensiunea maxima si aloca memorie pentru intrarile dicionarului.
 **/
void dictInit(struct Dict *const dict, unsigned entryCount);

/**
 * Adauga o intrare in dictionar.
 **/
void dictAddEntry(struct Dict *const dict, struct DictEntry dictEntry);

/**
 * Extrage intrarea din dictionar aflata la pozitia entryIndex.
 * 
 * Returneaza intrarea din dictionar de la pozitia entryIndex sau NULL daca pozitia specificata e eronata.
 **/
const struct DictEntry *const dictGetEntry(const struct Dict *const dict, unsigned entryIndex);

/**
 * Cauta in dictionar cuvantul target.
 * 
 * Returneaza intrarea gasita sau NULL daca cuvantul nu a fost gasit in dictionar.
 **/
const struct DictEntry *const dictGetEntryByTarget(const struct Dict *const dict, const char *target);

/**
 * Elibereaza memoria ocupata de dicionar.
 **/
void dictFree(const struct Dict *const dict);

/**
 * Initializeaza campurile intrarii de dictionar cu valorile specificate.
 **/
void dictEntryInit(struct DictEntry *const dictEntry, char target[MESSAGE_MAX_SIZE+1], char subst[MESSAGE_MAX_SIZE+1]);

/**
 * Elibereaza memoria ocupata de intrarea de dictionar.
 **/
void dictEntryFree(const struct DictEntry *const dictEntry);

#endif
