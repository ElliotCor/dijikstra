#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define max 50

typedef struct structure1 {
    char name;
    char prev;
} structure1;

typedef struct structure2 {
    struct structure1 *nextPoints;
    int dist;
} structure2;

typedef struct Point {
    struct structure1 *Data;
    struct structure2 list[max];
} Point;

// Pour Stocker Deja Visiter (liste statique)
typedef struct {
    Point points[max];
    int count;
} VisitedList;

// Pour Stocker Recontrer
typedef struct Cell {
    Point Contenu;
    struct Cell *next;
} Cell;

typedef struct Queue {
    Cell* head;
    Cell* tail;
    int length;
} Queue;

typedef struct Stack {
    Point Contenu;
    struct Stack *next;
} Stack;

void ajouter_a_queue(Queue* Q, Point C);
void empiler(Stack** sommet, Point C, Stack **head);
void supprimer_de_queue(Queue* Q, Point C);
void affichage_point_vist(Stack *stack);
void affichage_point_non_visit(Queue *Q);

Queue* Initia() {
    Queue* A = (Queue*)malloc(sizeof(Queue));
    if (A == NULL) {
        printf("\nError : Allocation De Memoire\n");
        return NULL;
    }
    A->head = NULL;
    A->tail = NULL;
    A->length = 0;
    return A;
}

Point* InitiaP(char c, Queue* Q) {
    Point* B = (Point*)malloc(sizeof(Point));
    if (B == NULL) {
        printf("\nError : Allocation De Memoire pour Point\n");
        return NULL;
    }
    B->Data = (structure1*)malloc(sizeof(structure1));
    if (B->Data == NULL) {
        printf("\nError : Allocation De Memoire pour structure1\n");
        free(B);
        return NULL;
    }
    B->Data->name = c;
    B->Data->prev = '\0';
    for (int i = 0; i < max; i++) {
        B->list[i].nextPoints = NULL;
        B->list[i].dist = 0;
    }
    ajouter_a_queue(Q, *B);
    return B;
}

void InitiaPile(Stack **head, Stack **sommet) {
    *head = *sommet = NULL;
}

// Liaison Entre Pointe
void Distance(Point* A, Point* B, int D) {
    int i = 0;
    while (A->list[i].nextPoints != NULL && i < max) {
        i++;
    }
    if (i < max) {
        A->list[i].nextPoints = B->Data;
        A->list[i].dist = D;
    }

    i = 0;
    while (B->list[i].nextPoints != NULL && i < max) {
        i++;
    }
    if (i < max) {
        B->list[i].nextPoints = A->Data;
        B->list[i].dist = D;
    }
}

void ajouter_a_queue(Queue* Q, Point C) {
    Cell* B = (Cell*)malloc(sizeof(Cell));
    if (B == NULL) {
        printf("\nError : Allocation De Memoire pour Cell\n");
        return;
    }
    B->next = NULL;
    B->Contenu = C;
    if (Q->length == 0) {
        Q->head = Q->tail = B;
    } else {
        Q->tail->next = B;
        Q->tail = B;
    }
    Q->length++;
}

void empiler(Stack** sommet, Point C, Stack **head) {
    Stack* nouveau_point = (Stack*)malloc(sizeof(Stack));
    if (nouveau_point == NULL) {
        printf("\nError : Allocation De Memoire pour Stack\n");
        return;
    }
    nouveau_point->Contenu = C;
    nouveau_point->next = *sommet;
    *sommet = nouveau_point;
    if (*head == NULL) {
        *head = nouveau_point; 
    }
}

void supprimer_de_queue(Queue* Q, Point C) {
    Cell *temp = Q->head, *prev = NULL;
    while (temp != NULL && temp->Contenu.Data->name != C.Data->name) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        return; // Point N'existe Pas Dans La File
    }
    if (prev == NULL) {
        Q->head = temp->next;
    } else {
        prev->next = temp->next;
    }
    if (temp == Q->tail) {
        Q->tail = prev;
    }
    free(temp);
    Q->length--;
}

Point extraire_min(Point* A, Queue* Q, Stack** pile, Stack **head) {
    // Chercher le point
    Cell* temp;
    Cell *p = NULL;
    int i = 0, dist_min = INT_MAX;
    while (A->list[i].nextPoints != NULL && i < max) {
        temp = Q->head;
        while (temp != NULL) {
            int x = A->list[i].dist;
            if (A->list[i].nextPoints->name == temp->Contenu.Data->name && x < dist_min) {
                dist_min = x;
                p = temp;
            }
            temp = temp->next;
        }
        i++;
    }
    if (p != NULL) {
        p->Contenu.Data->prev = A->Data->name;
        // temp pointe sur point qui doit supprimer de la "File" et ajouter a la "Pile"
        empiler(pile, p->Contenu, head);
        supprimer_de_queue(Q, p->Contenu);
    }
    return p->Contenu;
}

void affichage_point_vist(Stack *stack) {
    Stack *temp = stack;
    if (temp == NULL) {
        printf("\nAucun Point\n");
    } else {
        while (temp != NULL) {
            printf("%c --> ", temp->Contenu.Data->name);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

void affichage_point_non_visit(Queue *Q) {
    Cell *temp = Q->head;
    if (temp == NULL) {
        printf("\nAucun Point\n");
    } else {
        while (temp != NULL) {
            printf("%c --> ", temp->Contenu.Data->name);
            temp = temp->next;
        }
        printf("NULL\n");
    } 
}

int main() {
    // Initialisation des structures
    Queue* queue = Initia();
    Stack* stack_head = NULL;
    Stack* stack_sommet = NULL;
    InitiaPile(&stack_head, &stack_sommet);

    // Création des points
    Point* pointA = InitiaP('A', queue);
    Point* pointB = InitiaP('B', queue);
    Point* pointC = InitiaP('C', queue);
    Point* pointD = InitiaP('D', queue);

    // Définir les distances entre les points
    Distance(pointA, pointB, 1);
    Distance(pointA, pointC, 4);
    Distance(pointB, pointC, 2);
    Distance(pointB, pointD, 5);
    Distance(pointC, pointD, 1);

    // Extraction et affichage des points selon Dijkstra
    printf("Points non visités initialement :\n");
    affichage_point_non_visit(queue);

    Point point_actuel = *pointA; // On commence par le point A
    while (queue->length > 0) {
        point_actuel = extraire_min(&point_actuel, queue, &stack_sommet, &stack_head);
        if (point_actuel.Data == NULL) {
            printf("\nAucun point extrait.\n");
            break;
        }
        printf("\nPoint extrait : %c\n", point_actuel.Data->name);
        printf("Points non visités :\n");
        affichage_point_non_visit(queue);
        printf("Points visités :\n");
        affichage_point_vist(stack_sommet);
    }

    // Libération de la mémoire allouée
    Cell* temp = queue->head;
    while (temp != NULL) {
        Cell* next = temp->next;
        free(temp->Contenu.Data);
        free(temp);
        temp = next;
    }
    stack_head = stack_sommet = NULL; // Remise à zéro des piles

    return 0;
}


