Programul contine 3 fisiere si anume main.c(aici e doar bucla principala while 
care citeste comenzile de la tastatura si apeleaza functiile necesare) 
functii.c(contine toate structurile si functiile) si functii.h(l-am folosit
ca sa leg cele doua fisiere, punand aici definitiile structurilor si antetele
functiilor).Pentru a gestiona toata aplicatia (imaginea curenta, L-System-ul, 
istoricul, fonturile), am folosit o singura structura mare numita game care 
le cuprinde pe toate. Astfel trimit un singur pointer &gs prin functii, 
iar orice modificare se pastreaza automat.

Am integrat si un sistem de Undo/Redo gestionat prin functiile do_undo si 
do_redo. Am folosit o structura history (un vector de actiuni) care retine 
starea anterioara relevanta. Pentru a economisi memorie, nu salvez totul 
mereu: la desenare salvez o copie a imaginii (img_state), iar la incarcarea 
de fisiere salvez doar parametrii necesari.

Grafeme fractale: Pentru rezolvarea acestui task am definit structura lsystem 
care stocheaza axioma si regulile de productie necesare generarii fractalilor.
Incep cu citirea fisierului, unde extrag axioma Si apoi parcurg fiecare linie 
pentru a salva regulile.Am facut stocarea regulilor folosind un vector de 
pointeri (rules) dimensionat pentru toate caracterele ASCII,ceea ce imi 
permite sa accesez regula pentru un anumit simbol, folosind codul sau ASCII 
pe post de index.Functia derive este cea care face efectiv generarea.
La fiecare derivare, mai intai parcurg tot sirul si calculez exact cati 
bytes va ocupa rezultatul (adunand lungimea fiecarei reguli care se aplica 
iar pentru caracterele fara regula contorizez doar 1 byte).
Cand stiu dimensiunea finala, aloc memoria si construiesc noul sir copiind 
caracterele neschimbate sau inlocuindu-le pe cele care au reguli.
Functia cleanup curata tot ce a fost alocat anterior (axiome, reguli) inainte 
sa incarc un fisier nou, ca sa nu ramana memorie neeliberata.

Turtle graphics: Pentru transformarea sirului in imagine am facut functia 
principala interpret_turtle care primeste sirul generat si il parcurge 
caracter cu caracter.Cand intalneste F (inaintare), calculez noua poziyie 
(x1, y1) folosind functii trigonometrice (cos pentru X, sin pentru Y) bazate 
pe unghiul si lungimea pasului.Dupa calcul apelez functia draw_line pentru 
a trasa segmentul pe imagine.Tot aici gestionez rotatiile la + si - doar 
actualizez variabila dir (unghiul), avand grija sa o pastrez in intervalul 
(0, 360) folosind fmod.Functia draw_line implementeaza algoritmul lui Bresenham
care decide optim ce pixeli trebuie aprinsi pentru a uni doua puncte.
Pentru a desena fractali complecsi (crengi), am avut nevoie sa retin pozitii 
anterioare.Am implementat o structura numita stack, care retine starea 
testoasei (pozitia X, Y si directia).La simbolul [ salvez starea curenta 
in vectorul stivei. Daca stiva se umple, o redimensionez dinamic ca sa nu am 
limite fixe.La simbolul ] extrag ultima pozitie salvata si trimit testoasa 
inapoi, fara sa desenez nimic.

Font:Pentru a gestiona fonturile, am definit structura font care contine un 
vector de pointeri catre structuri glyph, indexat direct prin codul ASCII.
Functia principala de incarcare, load_font, proceseaza fisierul BDF linie cu 
linie. Cand intalneste cuvantul STARTCHAR, aloc dinamic memorie pentru o noua 
glifa. Apoi, citesc dimensiunile din BBX pentru a sti cat spatiu ocupa 
caracterul si unde trebuie pozitionat la cursor.Am implementat functia 
hex_to_bin pentru a transforma pixelii din fisierul BDF din forma hexazecimala
in binar si completeaza matricea glifei bit cu bit.Pentru afisare am scris 
functia draw_text care parcurge sirul primit si apeleazs draw_glyph pentru 
fiecare litera.Pixelii sunt desenati tinand cont de offset-urile x_off si 
y_off, iar dupa fiecare litera, cursorul avanseaza pe axa X cu valoarea dx 
specificata in font, pentru ca literele sa nu se suprapuna.

Gravura: Pentru acest task am implementat mai intai functia ajutatoare get_bit.
Aceasta primeste indexul absolut al unui bit din tot sirul de date si 
returneaza valoarea lui, ocupandu-se de calculele matematice necesare.
Functia principala, bitcheck, parcurge toti bitii imaginii si verifica ferestre
de cate 4 biti consecutivi.Cand detectez tiparele problematice 
(0010 sau 1101) transform acel index inapoi in coordonate vizuale.Am folosit o 
logica inversa: din indexul bitului am aflat octetul, din octet am dedus 
indexul pixelului si canalul de culoare (R, G sau B), iar la final am 
calculat coordonatele X si Y.Am simulat eroarea folosind operatorul XOR pe 
bitul instabil.
Astfel, pot afisa valoarea fara sa modific imaginea din memorie.
