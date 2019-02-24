Pentru taskul 1 am creat o structura in homeowrk.h pentru retinerea datelor citie din fisier ,
kernelului Gaussian, si a celor 2 imagini sub forma de matrice cea alb negru, si cea colora cu cele 3 culori.
Am creat o structura globala in homeowrk.c pentru a retine inceputul si finalul fiecarui thread si nr de threaduri
In functia readInput am citit datele din fisier si le am retinut in structura. Am retinut imaginea intr o matrice alocata dinamic.
In functia de resize retin in matricea de output valorile din matricea de input pt a le putea avea in functia de writeData
apoi aloc nr de indici pt threaduri in functie de valoarea num-threads si apelez cele 2 functii care fac resize pentru fiecare thread.
De asemenea aloc valoarea de inceput si final pt fiecare thread pt ambele tipuri de imagini.
In functia albnegru fac resizeul imaginii incolore.Parcurg matricea pe lnii si coloane paralelizand fiecare linie.
Daca resize are valoare para , parcurg un nr de resize linii si coloane si fac suma a resize_factor*resize_factor pixeli.
Dupa aceea suma o impart la resize_factor*resize_factor si o adaug in matricea de out
Pentru resize 3 inmultesc matricea de 3*3 cu matricea kernel Gaussiana si dupa impart rezultatul la 16.
In functia culoareim fac acelasi operatii dar pt toate cele 3 campuri de culori.
In functia de out scriu la fisierul de out valorile calculate.

Pentru taskul 2 in functia initilize initlizez valorile tipul ( tot timpul alb negru) rezolutia data prin argument si valoarea.
In functia de render initlizez ca la taskul 1 valorile de inceput si final ale threadurilor(i ul curent * rezolutia/ nr de threaduri)
si apelez functia de randare pt fiecare thread.
In functia dunga creez imaginea ceruta.Parcurg pe linii(paralelizate ) si coloane matricea si pentru fiecare pixel calculez 
distanta pana la dreapta data cu ecutia disantei de la un punct la o dreapta.
Adaug 0.5 la fiecare coordonata a pixelului deoarece distanta se calculeaza din centrul pixelului.
Scalez dimensiunea fiecarui pixel la rezolutia data imaprtind fiecare coordonata la rezolutie si inmultind cu marimea tablei.
De asemenea scad din rezolutie coloana pentru coordonata 0 este in stanga jos si pt linii si coloane.
In functia de write scriu in fisier datele obtinute. 



