#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 50

typedef struct structure1 {
    char name;
    char prev;
} structure1;

typedef struct structure2 {
    structure1 *nextPoints;
    int dist;
} structure2;

typedef struct {
    structure1 *Data;
    structure2 *list[MAX];
} Point;

typedef struct cell {
    Point *Contentu;
    struct cell *next;
} Cell;

// Pour Stocker Recontrer
typedef struct {
    Cell *head;
    Cell *tail;
    int length;
} Queue;

Queue* Init() {
    Queue *A = (Queue*)malloc(sizeof(Queue));
    if (A == NULL) {
        printf("\nError: Allocation De Memoire\n");
        return NULL; 
    }
    A->head = NULL;
    A->tail = NULL;
    A->length = 0;
    return A;
}
void ajouter_a_queue(Queue *Q, Point *C);
Point* InitP(char c,Queue *Q) {
    Point *B = (Point*)malloc(sizeof(Point));
    if (B == NULL) {
        printf("\nError: Allocation De Memoire\n");
        return NULL;
    }
    B->Data = (structure1*)malloc(sizeof(structure1));
    if (B->Data == NULL) {
        printf("\nError: Allocation De Memoire\n");
        free(B);
        return NULL;
    }
    B->Data->name = c;
    B->Data->prev = '\0'; // '\0' pour indiquer les points precedents
    for (int i = 0; i < MAX; i++) {
        B->list[i] = NULL;
    }
    ajouter_a_queue(Q,B);
    return B;
}

// Liaison Entre Pointe
void Distance(Point *A, Point *B, int D) {
    int i = 0;
    while (A->list[i] != NULL && i < MAX) {
        i++;
    }
    if (i == MAX) {
        printf("Error: Maximum de point atteint %c\n", A->Data->name);
        return;
    }
    A->list[i] = (structure2*)malloc(sizeof(structure2));
    if (A->list[i] == NULL) {
        printf("Error: Allocation Memoire\n");
        return;
    }
    A->list[i]->nextPoints = B->Data;
    A->list[i]->dist = D;
    
    i = 0;
    while (B->list[i] != NULL && i < MAX) {
        i++;
    }
    if (i == MAX) {
        printf("Error: Maximum de point atteint %c\n", B->Data->name);
        return;
    }
    B->list[i] = (structure2*)malloc(sizeof(structure2));
    if (B->list[i] == NULL) {
        printf("Error: Allocation Memoire\n");
        return;
    }
    B->list[i]->nextPoints = A->Data;
    B->list[i]->dist = D;
}

// pour ajouter une nouvelle cellule
void ajouter_a_queue(Queue *Q, Point *C) {
    Cell *B = (Cell*)malloc(sizeof(Cell));
    if (B == NULL) {
        printf("Error: Allocation Memoire\n");
        return;
    }
    B->Contentu = C;
    B->next = NULL;
    if (Q->length == 0) {
        Q->head = Q->tail = B;
    } else {
        Q->tail->next = B;
        Q->tail = B;
    }
    Q->length++;
}

// pour supprimer une cellule a tete
void supprimer_de_queue(Queue *Q) {
    if (Q->length == 0) return;
    Cell *temp = Q->head;
    Q->head = Q->head->next;
    free(temp);
    Q->length--;
    if (Q->length == 0) {
        Q->tail = NULL;
    }
}

// trouver la distance minimale
Point* extraire_min(Point* A,Queue* Q) {
    if (A==NULL){
        printf("Le point est vide");
        return;
    }
    Cell* P=(Cell*)malloc(sizeof(Cell));
    if (P == NULL) {
        printf("Error: Allocation Memoire\n");
        return;
    }
    int i=0,dist_min=INT_MAX;
    start:
    while (A->list[i]!=NULL){    
        for (P=Q->head;P!=NULL;P->next){    
            if (A->list[i]->nextPoints->name==P->Contentu->Data->name){
                i++;
                goto start;
            }
        }
        if (dist_min>A->list[i]->dist)
        {
            dist_min=A->list[i]->dist;
        }
        i++;
    }
    if (dist_min!=INT_MAX){
    for (int j = 0; j < i; j++)
    {
        if (dist_min==A->list[j]->dist)
        {
            i=j;
            break;
        }
    }
    free(P);
    A->list[i]->nextPoints->prev=A->Data->name;
    P->Contentu->Data=A->list[i]->nextPoints;
    P->Contentu->list[0]->dist=A->list[i]->dist;
    ajouter_a_queue(Q,P->Contentu);
    return P->Contentu;}
    return NULL;
}
void Affichage(Queue* Q){
    Cell* P;
    int Distance=0;
    for ( P = Q->head; P!=NULL ; P->next)
    {
        printf("Le Position de noeud current: %c\n",P->Contentu->Data->name);
        if (P->Contentu->Data->prev!='\0')
        {
            printf("Le noeud predecessor: %c\n",P->Contentu->Data->prev);
        }
        Distance+=P->Contentu->list[0]->dist;
        printf("La distance totale: %d\n",Distance);
    }
}
void Djikstra(Point* A,Queue* Q){
    Queue *Sol=(Queue*)malloc(sizeof(Queue));
    if (Sol == NULL) {
        printf("Error: Allocation Memoire\n");
        return;
    }

}