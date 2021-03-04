[t9]: https://en.wikipedia.org/wiki/T9_(predictive_text)

[include]: include/
[src]: src/

[telefon-c]: telefon.c

[letter-h]: include/letter.h
[letter-c]: src/letter.c

[dict-h]: include/dict.h
[dict-c]: src/dict.c

[message-h]: include/message.h
[message-c]: src/message.c

[helpers-h]: include/helpers.h
[helpers-c]: src/helpers.c

# Telefon
Acest subdirector conține **Tema 1** din cadrul cursului _Programarea calculatoarelor_ ce presupune operații pe mesaje și mesaje codificate într-un format similar celui folosit de [tehnologia T9][t9]. Programul este împărțit în mai multe componente/fișiere, fiecare din acestea rezolvând o anumită problemă.

## Structura codului sursă
În fișierele antet, aflate în [directorul include][include], se află declarațiile/definițiile de constante, structuri, funcții ce sunt folosite în cadrul fișierelor sursă. Unde este cazul, există și comentarii ce însoțesc declarațiile/definițiile menite să ușureze înțelegerea scopului acestora.

În fișierele sursă, aflate în [directorul src][src], se află implementările, fiind comentate părțile mai importante din logica programului.

## Main ([telefon.c][telefon-c])
Este partea principală ce se ocupă de citirea datelor de intrare și rezolvarea fiecărei cerințe în parte cu ajutorul componentelor enumerate mai jos.

## Literă ([letter.h][letter-h], [letter.c][letter-c])
Aceasta stă la baza mesajelor. Pe o literă se poate aplica funcția de codificare `letterEncode` pentru codificarea acesteia, iar pe codificarea unei litere se poate aplica funcția `letterDecode` pentru decodificare. De asemenea, aici există și funcția `areLettersOnSameDigit` prin care se verifică dacă 2 litere se află pe aceeași tastă a unei tastaturi de tip [T9][t9].

## Dicționar ([dict.h][dict-h], [dict.c][dict-c])
Este folosit pentru a stoca cuvintele ce trebuie corectate în cadrul mesajelor.

## Mesaj ([message.h][message-h], [message.c][message-c])
Un mesaj este compus din mai multe litere, iar acesta poate fi codificat cu ajutorul funcției de codificare `messageEncode` și decodificat cu ajutorul funcției de decodificare `messageDecode`. De asemenea, un mesaj poate fi corectat cu ajutorul cuvintelor dintr-un dicționar oferit ca argument funcției `messageCorrect`.

## Helpers ([helpers.h][helpers-h], [helpers.c][helpers-c])
Aici se află funcțiile ajutătoare ce facilitează rezolvarea temei.
