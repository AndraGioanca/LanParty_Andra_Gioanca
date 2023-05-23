# Tema 1 - Lan Party

## Descriere scurta
Am folosit o lista simplu inlantuita pentru a retine echipele si o lista simplu inlantuita pentru a retine jucatorii. Am folosit o coada pentru a retine echipele care urmeaza sa joace si o stiva pentru a retine echipele castigatoare si echipele invinse. Am folosit o lista simplu inlantuita pentru a retine echipele din top8.

## Cerinta 1
Am citit din fisierul d.in si am returnat o structura de tip LanParty.

## Cerinta 2
Am calculat scorul fiecarei echipe si am eliminat echipele cu scorul cel mai mic pana cand am ajuns la 8 echipe.

## Cerinta 3
Am initializat jocul si am adaugat echipele in coada. Am procesat rundele pana cand am ajuns la 8 echipe. Am creat top8 si l-am afisat. Am procesat rundele finale.

## Functii folosite
- `readC(FILE* c)` - citeste din fisierul c.in si returneaza un vector de 5 elemente, 0 sau 1, care indica daca se doreste executarea cerintei respective sau nu.
- `cerinta1(FILE* d)` - citeste din fisierul d.in si returneaza o structura de tip LanParty.
- `printTeams(FILE* r, struct LanParty lanParty)` - afiseaza echipele din lanParty.
- `cerinta2(FILE* r, struct LanParty *lanParty)` - calculeaza scorul fiecarei echipe si elimina echipele cu scorul cel mai mic pana cand ajunge la 8 echipe.
- `createQueue(struct Team team)` - creaza o coada.
- `enqueue(struct Queue** queue, struct Team team)` - adauga o echipa in coada.
- `dequeue(struct Queue** queue)` - extrage o echipa din coada.
- `createStack(struct Team team)` - creaza o stiva.
- `push(struct Stack** stack, struct Team team)` - adauga o echipa in stiva.
- `pop(struct Stack** stack)` - extrage o echipa din stiva.
- `initGame(struct LanParty lanParty, struct TeamList** teamList, struct Queue** gamesQueue, struct Stack** winnersStack, struct Stack** losersStack, struct TeamList** top8)` - initializeaza jocul.
- `enqueueTeams(struct TeamList* teamList, struct Queue** gamesQueue)` - adauga echipele in coada.
- `checkWinner(struct Team *team1, struct Team *team2, struct Stack** winnersStack, struct Stack** losersStack)` - verifica care echipa castiga.
- `processFirstRounds(FILE* r, struct Queue** gamesQueue, struct Stack** winnersStack, struct Stack** losersStack, int* round)` - proceseaza rundele pana cand ajunge la 8 echipe.
- `createTop8(struct Queue** gamesQueue, struct TeamList** top8)` - creaza top8.
- `displayTop8(FILE* r, struct TeamList* top8)` - afiseaza top8.
- `processFinalRounds(FILE* r, struct Queue** gamesQueue, struct Stack** winnersStack, struct Stack** losersStack, int* round)` - proceseaza rundele finale.
- `cerinta3(FILE* r, struct LanParty lanParty)` - initializeaza jocul, adauga echipele in coada, proceseaza rundele pana cand ajunge la 8 echipe, creaza top8, afiseaza top8, proceseaza rundele finale.

## Structuri folosite
- `struct Player` - contine numele jucatorului si scorul acestuia.
- `struct PlayerList` - contine o lista de jucatori.
- `struct Team` - contine numele echipei, scorul echipei, numarul de jucatori si o lista de jucatori.
- `struct TeamList` - contine o lista de echipe.
- `struct Queue` - contine o echipa si un pointer catre urmatoarea echipa.
- `struct Stack` - contine o echipa si un pointer catre urmatoarea echipa.
- `struct LanParty` - contine numarul de echipe si o lista de echipe.

## Detaliere folosire structuri de date
- lista: am folosit trei liste simplu inlantuite: una pentru a retine echipele, una pentru a retine jucatorii si una pentru a retine ehipele din top 8.
- coada: am folosit o coada pentru a retine echipele care urmeaza sa joace.
- stiva: am folosit o stiva pentru a retine echipele castigatoare si una pentru a retine echipele invinse.

## Concluzie
Am aprofundat cunostintele despre liste, cozi si stive, prin implementarea unui program care simuleaza o situatie reala.
