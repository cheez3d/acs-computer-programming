[telefon-c]: telefon.c

[letter-h]: include/letter.h
[message-h]: include/message.h
[dict-h]: include/dict.h
[helpers-h]: include/helpers.h

[letter-c]: src/letter.c
[message-c]: src/message.c
[dict-c]: src/dict.c
[helpers-c]: src/helpers.c

# Telefon
Tema 1 din cadrul cursului de Programarea Calculatoarelor ce presupune operații pe mesaje sau mesaje codificate.
Programul este împărțit în mai multe componente, fiecare din acestea rezolvând o anumită problemă.

## Main ([telefon.c][telefon-c])
Este partea principală ce se ocupă de citirea datelor de intrare și rezolvarea fiecărei cerințe în parte cu ajutorul componentelor enumerate mai jos

### Literă ([letter.h][letter-h], [letter.c][letter-c])
* stă la baza mesajelor
* pe ea se poate aplica funcția de codificare
* pe codul ei se poate aplica funcția de decodificare
* se poate verifica dacă două litere împărtășesc aceeași cifră/tastă

### Mesaj ([message.h][message-h], [message.c][message-c])
* compus din mai multe litere
* se poate codifica cu ajutorul funcției de codificare
* codificarea acestuia se poate decodifica cu ajutorul funcției de decodificare
* se poate corecta cu ajutorul dicționarului
* se poate împărți în componente

### Dicționar ([dict.h][dict-h], [dict.c][dict-c])
Folosit pentru a stoca cuvintele ce trebuie corectate.

### Helpers ([helpers.h][helpers-h], [helpers.c][helpers-c])
Aici se află funcțiile ajutătoare.

## Structura codului sursă
În fișierele header (aflate în directorul include) se află declarațiile și descrierea a ceea ce face fiecare funcție în parte.

În fișierele sursă (aflate în directorul src) corespunzătoare fișierelor header se află implementările, fiind comentate părțile mai importante din logica programului.
