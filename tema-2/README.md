[ncurses]: https://en.wikipedia.org/wiki/Ncurses

[include]: include/
[src]: src/

[trivia-c]: trivia.c

[csv]: https://en.wikipedia.org/wiki/Comma-separated_values
[csv-h]: include/csv.h
[csv-c]: src/csv.c

[lang-h]: include/lang.h
[lang-c]: src/lang.c
[data-lang]: data/lang/

[game-h]: include/game.h
[game-c]: src/game.c

[question-h]: include/question.h

[str-h]: include/str.h
[str-c]: src/str.c

[err-h]: include/err.h
[err-c]: src/err.c

# Trivia
Acest subdirector conține **Tema 2** din cadrul cursului _Programarea calculatoarelor_ ce presupune implementarea unui joc de tip quiz în linia de comandă, utilizând biblioteca [ncurses][ncurses]. Programul este împărțit în mai multe componente/fișiere, fiecare din acestea rezolvând o anumită problemă.

## Structura codului sursă
În fișierele antet, aflate în [directorul include][include], se află declarațiile/definițiile de constante, structuri, funcții ce sunt folosite în cadrul fișierelor sursă. Unde este cazul, există și comentarii ce însoțesc declarațiile/definițiile menite să ușureze înțelegerea scopului acestora.

În fișierele sursă, aflate în [directorul src][src], se află implementările, fiind comentate părțile mai importante din logica programului.

## Bucla principală ([trivia.c][trivia-c])
Este partea principală ce se ocupă de inițializarea [ncurses][ncurses] și încărcarea fișierelor necesare jocului, cât și de începerea buclei de joc.

## [CSV][csv] ([csv.h][csv-h], [csv.c][csv-c])
Reprezintă o componentă ce înglobează un _parser_ și _writer_ de fișiere `.csv` scris de la 0 și poate fi considerat un _highlight_ al temei, deoarece acesta ușurează extrem de mult lucrul cu fișerele ce conțin întrebările, cât și salvarea persistentă a datelor jocului.

## Suportul pentru mai multe limbi ([lang.h][lang-h], [lang.c][lang-c])
Aceste fișiere implementează logica necesară pentru ca jocul să suporte mai multe limbi de afișare. Se folosește de componenta [CSV][csv] pentru a citi fișierele din [directorul data/lang][data-lang].

## Logica jocului ([game.h][game-h], [game.c][game-c])
Aceste fișere conțin implementarea logicii de joc.

## Întrebare ([question.h][question-h])
Aceste fișere conțin o structură ce stochează o întrebare cu variantele de răspuns și răpunsul corect.

## Funcții auxiliare pentru șiruri de caractere ([str.h][str-h], [str.c][str-c])
Aceste fișiere conțin funcții ajutătoare pentru manipularea mai facilă a șirurilor de caractere.

## Sistemul de gestionare a erorilor ([err.h][err-h], [err.c][err-c])
Reprezintă un sistem rudimentar de raportare și gestionare a erorilor ce pot apărea în cadrul rulării aplicației.

## Notă
Spre deosebire de cerință, programul nu ia o listă de fișiere cu întrebări ca argumente, ci le caută automat folosind funcții de traversare a directoarelor în [directorul data/lang][data-lang].
