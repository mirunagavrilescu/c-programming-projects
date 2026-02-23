1.Window join:
Programul citește mai întâi o valoare window, iar apoi, in mod repetat, perechi cu timestamp si valoare.Verific sa nu fie ultima pereche (0,0)
si salvez intr-o variabila (k) numarul de intari.Folosind doua structuri repetitive parcurc toate perechile doua cate doua si verific ca diferanta
de timp dintre cele doua sa fie mai mica sau egala decat valoarea window, caz in care calculez folosindu ma de functii cel mai mic multiplu comun
si cel mai mare divizor comun pentru cele doua valori si le afisez in ordinea ceruta.

2.Akari:
Pentru rezolvare, am folosit mai multe funcții care lucrează cu grid-ul în funcție de opțiunea aleasă.
Initial citesc optiunea, dimensiunile grid-ului, iar apoi cu doua structuri repetitive elementele acestuia.După citirea datelor, apelez 
funcția propagare_lumina, care caută toate casutele ce conțin becuri (L).Pentru fiecare bec, funcția merge în cele patru direcții 
(sus, jos, stânga, dreapta) și înlocuiește toate casutele albe (-) cu caracterul x, pentru a marca faptul că acele casute sunt iluminate.
Lumina se propagă până la întâlnirea unei casute negre sau până la marginea grd-ului.
Astfel, pentru optiunea 1 imi mai ramane doar sa  apelez functia care cauta casutele cu cifre.Pentru fiecare astfel de casuta, se numără câte becuri 
există în cele patru casute adiacente.Dacă numărul de becuri este egal cu cifra din casuta, atunci toate celelalte casute adiacente libere sunt 
marcate cu x, pentru că acolo nu se mai pot pune becuri.Dupa ce am apelat si functia asta am marcat in grid toate casutele care sunt iluminate 
si pe care nu se mai poate pune bec ramane doar sa il afisez apeland functia afisare_grid.

Pentru optiunea 2 am folosit 3 functii:becuri_luminate_reciproc -verifică dacă există două becuri care se luminează unul pe celălalt.
Funcția parcurge grid-ul, iar pentru fiecare bec caută pe linie și pe coloană un alt bec până întâlnește o casutsa neagră sau marginea grilei.
Dacă găsește un alt bec pe același drum, returnează 0,nr_corect_becuri_adiacente - verifică pentru fiecare celulă numerotată dacă numărul de becuri
adiacente este exact egal cu cifra din celulă.Dacă sunt mai multe becuri decât trebuie, returnează 0.Dacă sunt mai puține, funcția apelează o altă funcție,
poate_pune_bec, care verifică dacă se mai pot plasa becuri în celulele adiacente (adică dacă acelea nu sunt deja iluminate sau ocupate).
Dacă nu se pot pune suficiente becuri, se returnează tot 0.Daca ambele functii intorc in final valoarea 1 inseamna ca grid-ul 
respecta regulile si se afiseaza 'ichi' daca cel putin una este 0 atunci se afiseaza 'zero'.

3.Helicopters
Pentru rezolvare, am folosit o funcție care verifică poziționarea și efectul fiecărui elicopter asupra terenului, iar în main am realizat 
citirea datelor, verificarea poziționării și afișarea rezultatelor finale.Inițial citesc dimensiunile terenului (n și m), apoi elementele matricei,
reprezentând pătrățelele terenului (cu valori 0 sau 1).După, citesc numărul de elicoptere (k), iar pentru fiecare elicopter se citesc coordonatele celor două capete 
ale ipotenuzei (l1, c1, l2, c2) și sensul varfului (s). Pentru fiecare elicopter, verific mai întâi dacă ipotenuza este paralelă cu una dintre pseudodiagonalele matricei.
Dacă elicopterul are ipotenuza orizontală, verticală sau nu respectă forma unui triunghi dreptunghic isoscel, programul afișează:Elicopterul <indice> este pozitionat 
necorespunzator!Pentru elicopterele valide, apelez funcția verificare_elicopter, care determină modul în care elicopterul afectează terenul.Functia calculeaza numarul 
total de patratele din triunghiul format de elicopter si numarul de patratele afectate de umbra din acesta.In final dacă mai mult de jumătate dintre pătrățele sunt 
afectate, elicopterul a aterizat greșit si funcția returnează -1,dacă niciun pătrățel nu este afectat, elicopterul a aterizat bine si returneaza 1,dacă afectează 
doar o parte din pătrățele elicopterul este valid dar face umbra deci returneaza 0.
In main folosesc doua variabile, x(numărul de elicoptere care nu afectează deloc terenul) si y(numărul de elicoptere care au aterizat greșit).Dacă funcția 
verificare_elicopter returnează -1, se adaugă în lista de elicoptere greșite dacă returnează 1, creste x.
In final se afiseaza mesajele pentru elicopterele poziționate greșit, numarul de elicoptere care nu fac umbra deloc,numarul de elicoptere care au aterizat 
gresit si indicii elicopterelor care au aterizat gresit in ordine crescatoare.