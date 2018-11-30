
	Programarea calculatoarelor - Seria CC
		Tema 1 - Telefon


	Pentru a putea utiliza checkerul pus la dispozitie, dupa dezarhivarea
checker.zip copiati in acelasi director sursa telefon.c, fisierul Makefile si
READMEul corespunzator temei. Ca sa va puteti testa tema rulati scriptul
checker.sh. Acesta va v-a afisa in consola rezultatele pentru fiecare cerinta
de la fiecare test in parte.

	unzip checker.zip
	cd checker
	cp ../your/path/telefon.c .
	cp ../your/path/Makefile .
	cp ../your/path/README .

	./checker.sh

Pentru ca checkerul sa functioneze:
	- Makefile-ul inclus trebuie sa contina cele trei reguli: build, run, clean
	- trebuie sa cititi inputul corespunzator pentru toate cerintele, chiar daca
decideti sa nu rezolvati unele cerinte intermediare
	- Atunci cand nu stiti sa rezolvati o cerinta, dar vreti sa rezolvati
cerintele urmatoare, afisati '\n'.
