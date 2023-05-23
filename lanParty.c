#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Player
{
    char* firstName;
    char* secondName;
    int points;
};

// folosesc o lista simplu inlantuita pentru a retine jucatorii
struct PlayerList
{
    struct Player player;
    struct PlayerList* next;
};

struct Team
{
    char* name;
    int numberOfPlayers;
    double score;
    struct PlayerList* players;
};

// folosesc o lista simplu inlantuita pentru a retine echipele
struct TeamList
{
    struct Team team;
    struct TeamList* next;
};

struct LanParty
{
    int numberOfTeams;
    struct TeamList* teams;
};

// Citeste din fisierul c.in si returneaza un vector de 5 elemente, 0 sau 1, care indica daca se doreste executarea cerintei respective sau nu.
int* readC(FILE* c) {
    int* v = (int*)malloc(5 * sizeof(int));
    for(int i = 0; i < 5; i++) {
        fscanf(c, "%d", &v[i]);
    }
    return v;
}

// Cerinta 1: Citeste din fisierul d.in si returneaza o structura de tip LanParty.
struct LanParty cerinta1(FILE* d) {
    struct LanParty lanParty;
    fscanf(d, "%d", &lanParty.numberOfTeams);
    lanParty.teams = NULL;
    for(int i = 0; i < lanParty.numberOfTeams; i++) {
        struct Team team;
        team.players = NULL;
        team.numberOfPlayers = 0;
        team.name = (char*)malloc(100 * sizeof(char));

        char *line = (char*)malloc(100 * sizeof(char));
        fgets(line, 100, d);
        if (line[0] == '\n') {
            i--;
            continue;
        }
        sscanf(line, "%d", &team.numberOfPlayers);

        // Find the start position of the team name
        char* teamNameStart = strchr(line, ' ') + 1;
        // Remove the trailing newline character, if present
        char* newlineChar = strchr(teamNameStart, '\n');
        if (newlineChar != NULL) {
            *newlineChar = '\0';
        }
        strcpy(team.name, teamNameStart);
        if (team.name[strlen(team.name) - 1] == ' ') {
            team.name[strlen(team.name) - 1] = '\0';
        }

        for(int j = 0; j < team.numberOfPlayers; j++) {
            struct Player player;
            player.firstName = (char*)malloc(100 * sizeof(char));
            player.secondName = (char*)malloc(100 * sizeof(char));
            fscanf(d, "%s %s %d", player.firstName, player.secondName, &player.points);
            struct PlayerList* playerList = (struct PlayerList*)malloc(sizeof(struct PlayerList));
            playerList->player = player;
            playerList->next = team.players;
            team.players = playerList;
        }
        struct TeamList* teamList = (struct TeamList*)malloc(sizeof(struct TeamList));
        teamList->team = team;
        teamList->next = lanParty.teams;
        lanParty.teams = teamList;
    }
    return lanParty;
}

void printTeams(FILE* r, struct LanParty lanParty) {
    struct TeamList* teamList = lanParty.teams;
    while(teamList != NULL) {
        fprintf(r, "%s\n", teamList->team.name);
        teamList = teamList->next;
    }
}

void cerinta2(FILE* r, struct LanParty *lanParty) {
    struct TeamList* teamList = lanParty->teams;
    while(teamList != NULL) {
        double score = 0;
        struct PlayerList* playerList = teamList->team.players;
        while(playerList != NULL) {
            score += playerList->player.points;
            playerList = playerList->next;
        }
        score /= teamList->team.numberOfPlayers;
        teamList->team.score = score;
        teamList = teamList->next;
    }
    // find the highest power of 2 that is smaller than the number of teams
    int rem = 1;
    while(rem * 2 <= lanParty->numberOfTeams) {
        rem *= 2;
    }
    int n = lanParty->numberOfTeams - rem;
    // Repeat the elimination process until the desired number of teams is reached
    for (int i = 0; i < n; i++) {
        struct TeamList* teamList1 = lanParty->teams;
        struct TeamList* teamList2 = lanParty->teams->next;
        struct TeamList* prevTeamList2 = NULL;
        struct TeamList* minTeamList = teamList1;

        while (teamList2 != NULL) {
            if (teamList2->team.score < minTeamList->team.score) {
                minTeamList = teamList2;
                prevTeamList2 = teamList1;
            }
            teamList1 = teamList1->next;
            teamList2 = teamList2->next;
        }
        // Remove the team with the lowest score from the list
        if (prevTeamList2 != NULL) {
            prevTeamList2->next = minTeamList->next;
        } else {
            lanParty->teams = minTeamList->next;
        }
    }
}

// create queue
struct Queue {
    struct Team team;
    struct Queue* next;
    int size;
};

struct Queue* createQueue(struct Team team) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->team = team;
    queue->next = NULL;
    return queue;
}

// add to queue
void enqueue(struct Queue** queue, struct Team team) {
    struct Queue* queueNode = createQueue(team);
    if (*queue == NULL) {
        *queue = queueNode;
        return;
    }
    struct Queue* last = *queue;
    while(last->next != NULL) {
        last = last->next;
    }
    last->next = queueNode;

    (*queue)->size++;
}

// remove from queue
struct Team *dequeue(struct Queue** queue) {
    if (*queue == NULL) {
        struct Team *team;
        return team;
    }
    struct Queue* temp = *queue;
    *queue = (*queue)->next;
    if (*queue == NULL) {
        temp->size = 0;
    } else {
        (*queue)->size--;
    }
    struct Team *team = &temp->team;

    return team;
}

// create stack
struct Stack {
    struct Team team;
    struct Stack* next;
};

struct Stack* createStack(struct Team team) {
    struct Stack* stack = (struct Stack*)malloc(sizeof(struct Stack));
    stack->team = team;
    stack->next = NULL;
    return stack;
}

// add to stack
void push(struct Stack** stack, struct Team team) {
    struct Stack* stackNode = createStack(team);
    stackNode->next = *stack;
    *stack = stackNode;
}

// remove from stack
struct Team pop(struct Stack** stack) {
    if (*stack == NULL) {
        struct Team team;
        return team;
    }
    struct Stack* temp = *stack;
    *stack = (*stack)->next;
    struct Team team = temp->team;
    return team;
}

void initGame(struct LanParty lanParty, struct TeamList** teamList, struct Queue** gamesQueue, struct Stack** winnersStack, struct Stack** losersStack, struct TeamList** top8) {
    *teamList = lanParty.teams;
    *gamesQueue = malloc(sizeof(struct Queue));
    (*gamesQueue)->size = 0;
    *winnersStack = malloc(sizeof(struct Stack));
    *losersStack = malloc(sizeof(struct Stack));
    *top8 = malloc(sizeof(struct TeamList));
}

void enqueueTeams(struct TeamList* teamList, struct Queue** gamesQueue) {
    while(teamList != NULL) {
        enqueue(gamesQueue, teamList->team);
        teamList = teamList->next;
    }
}

void checkWinner(struct Team *team1, struct Team *team2, struct Stack** winnersStack, struct Stack** losersStack) {
    if (team1->score > team2->score) {
        struct PlayerList* playerList = team1->players;
        while (playerList != NULL) {
            playerList->player.points++;
            playerList = playerList->next;
        }
        team1->score++;
        push(winnersStack, *team1);
        push(losersStack, *team2);
    } else {
        struct PlayerList* playerList = team2->players;
        while (playerList != NULL) {
            playerList->player.points++;
            playerList = playerList->next;
        }
        team2->score++;
        push(winnersStack, *team2);
        push(losersStack, *team1);
    }
}

void processFirstRounds(FILE* r, struct Queue** gamesQueue, struct Stack** winnersStack, struct Stack** losersStack, int* round) {
    while ((*gamesQueue)->size > 8) {
        fprintf(r, "--- ROUND NO:%d\n", *round);
        while (*gamesQueue != NULL) {
            // extrag primele doua echipe din gamesQueue
            struct Team *team1 = dequeue(gamesQueue);
            if (team1->name == NULL) {
                team1 = dequeue(gamesQueue);
            }
            struct Team *team2 = dequeue(gamesQueue);
            
            int padding = 32 - strlen(team2->name);
            fprintf(r, "%-32s - %*s%s\n", team1->name, padding, " ", team2->name);

            checkWinner(team1, team2, winnersStack, losersStack);
        }

        // sterg echipele din stiva de invinsi
        while (*losersStack != NULL) {
            pop(losersStack);
        }

        fprintf(r, "\nWINNERS OF ROUND NO:%d\n", *round);
        // cat timp stiva de castigatori nu e goala
        while (*winnersStack != NULL) {
            // extrag primele doua echipe din stiva de castigatori
            struct Team team1 = pop(winnersStack);
            struct Team team2 = pop(winnersStack);
            if (team1.name == NULL || team2.name == NULL) {
                break;
            }
            fprintf(r, "%-33s -  %.2f\n", team1.name, team1.score);
            fprintf(r, "%-33s -  %.2f\n", team2.name, team2.score);
            // adaug meciul in gamesQueue
            enqueue(gamesQueue, team1);
            enqueue(gamesQueue, team2);
        }
        fprintf(r, "\n");
        (*round)++;
    }
}

void createTop8(struct Queue** gamesQueue, struct TeamList** top8) {
    struct Queue* aux = *gamesQueue;
    // adaug in top8 echipele ramase in gamesQueue
    while (aux != NULL) {
        struct Team *team = dequeue(&aux);
        (*top8)->team = *team;
        (*top8)->next = NULL;
    }
}

void displayTop8(FILE* r, struct TeamList* top8) {
    // afisez echipele din top8
    while (top8->next != NULL) {
        fprintf(r, "%s", top8->team.name);
        top8 = top8->next;
    }
}

void processFinalRounds(FILE* r, struct Queue** gamesQueue, struct Stack** winnersStack, struct Stack** losersStack, int* round) {
    for (int i = 0; i < 3; i++) {
        fprintf(r, "--- ROUND NO:%d\n", *round);
        while (*gamesQueue != NULL) {
            // extrag primele doua echipe din gamesQueue, dar tine cont ca team1 este struct Team *, deci pointer
            struct Team *team1 = dequeue(gamesQueue);
            if (team1->name == NULL) {
                team1 = dequeue(gamesQueue);
            }
            struct Team *team2 = dequeue(gamesQueue);
            int padding = 32 - strlen(team2->name);
            fprintf(r, "%-32s - %*s%s\n", team1->name, padding, " ", team2->name);
            checkWinner(team1, team2, winnersStack, losersStack);
        }
        // sterg echipele din stiva de invinsi
        while (*losersStack != NULL) {
            pop(losersStack);
        }
        fprintf(r, "\nWINNERS OF ROUND NO:%d\n", *round);
        // cat timp stiva de castigatori nu e goala
        while (*winnersStack != NULL) {
            // extrag primele doua echipe din stiva de castigatori
            struct Team team1 = pop(winnersStack);
            struct Team team2 = pop(winnersStack);
            if (team1.name == NULL) {
                break;
            } else if (strcmp(team1.name, team2.name) == 0) {
                fprintf(r, "%-33s -  %.2f\n", team1.name, team1.score);
                return;
            }
            fprintf(r, "%-33s -  %.2f\n", team1.name, team1.score);
            fprintf(r, "%-33s -  %.2f\n", team2.name, team2.score);
            // adaug meciul in gamesQueue
            enqueue(gamesQueue, team1);
            enqueue(gamesQueue, team2);
        }
        fprintf(r, "\n");
        (*round)++;
    }
}



void cerinta3(FILE* r, struct LanParty lanParty) {
    struct TeamList* teamList;
    struct Queue* gamesQueue;
    struct Stack* winnersStack;
    struct Stack* losersStack;
    struct TeamList* top8;
    int round = 1;
    
    initGame(lanParty, &teamList, &gamesQueue, &winnersStack, &losersStack, &top8);
    enqueueTeams(teamList, &gamesQueue);
    processFirstRounds(r, &gamesQueue, &winnersStack, &losersStack, &round);
    createTop8(&gamesQueue, &top8);
    displayTop8(r, top8);
    processFinalRounds(r, &gamesQueue, &winnersStack, &losersStack, &round);
}

int main(int argc, char *argv[]) {
    // usage: ./lanParty c.in d.in r.out
    FILE *c = fopen(argv[1], "r");
    FILE *d = fopen(argv[2], "r");
    FILE *r = fopen(argv[3], "w");

    int* v = readC(c);
    struct LanParty lanParty = cerinta1(d);
    for (int i = 0; i < 5; i++) {
        if (v[i] == 1) {
            switch(i) {
                case 0:
                    break;
                case 1:
                    cerinta2(r, &lanParty);
                    break;
                case 2:
                    printTeams(r, lanParty);
                    fprintf(r, "\n");
                    cerinta3(r, lanParty);
                    break;
                case 3:
                    // cerinta4(r, lanParty);
                    break;
                case 4:
                    // cerinta5(r, lanParty);
                    break;
            }
        }
    }
    if (v[2] == 0) {
        printTeams(r, lanParty);
        return 0;
    } else {
        return 0;
    }

    fclose(c);
    fclose(d);
    fclose(r);
}