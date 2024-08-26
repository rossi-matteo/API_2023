#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#define MAX_CAR_FLEET_CAPACITY 512
#define COMMAND_SIZE 20

//cd /mnt/d/Desktop/polimi/ProjectAPI

void swap(int* x, int* y){
    int temp = *y;
    *y = *x;
    *x = temp;
}

//Definizione Struttura di Gestione del Parco Auto
typedef struct CarFleet{
    int* maxHeap;
    int currentHeapSize;
    int capacity;
}CarFleet;

typedef struct Station{
    int id;
    int written;
    struct Station* linked_to;
    int distance;
    CarFleet* carFleet;
}Station;

//MAX-HEAP DI DIMENSIONE DINAMICA
CarFleet* createCarFleet(int capacity){
    CarFleet* newCarFleet = (CarFleet*) malloc( sizeof(CarFleet));
    newCarFleet -> maxHeap = (int*) malloc(capacity * sizeof(int));
    newCarFleet -> currentHeapSize = 0;
    newCarFleet -> capacity = capacity;
    return newCarFleet;
}

void extendCarFleet(CarFleet* currentCarFleet, int number_new_elements){
    currentCarFleet -> maxHeap = (int*) realloc ( currentCarFleet -> maxHeap, ( currentCarFleet -> currentHeapSize + number_new_elements)*sizeof( int));
    currentCarFleet -> currentHeapSize = currentCarFleet -> currentHeapSize + number_new_elements;
    currentCarFleet -> capacity = currentCarFleet -> currentHeapSize;
}

int getMaxCarFleet( CarFleet* carFleet){
    if( carFleet -> currentHeapSize == 0) return 0;

    return (carFleet -> maxHeap[0]);
}

void max_heapify(CarFleet* carFleet, int index){
    int left = 2*index+1;
    int right = 2*index+2;
    int max = 0;
    
    if(left < carFleet -> currentHeapSize && carFleet -> maxHeap[left] > carFleet -> maxHeap[index])
        max = left;
    else 
        max = index;
    
    if(right < carFleet -> currentHeapSize && carFleet -> maxHeap[right] > carFleet -> maxHeap[max])
        max = right;

    if(max != index){
        swap( &carFleet -> maxHeap[index], &carFleet -> maxHeap[max]);
        max_heapify(carFleet, max);
    }
}

void insertIntoCarFleet(CarFleet* carFleet, int new_car){
    int currentIndex = 0;
    if(carFleet -> currentHeapSize+1 > carFleet -> capacity){ //Se eccedo la capacità massima raggiunta dal Max-Heap alloco nuova memoria
        extendCarFleet( carFleet, 1);
        currentIndex = carFleet->currentHeapSize-1;
    }else{
        currentIndex = carFleet->currentHeapSize;
        carFleet -> currentHeapSize++;
    }
    
    carFleet -> maxHeap[currentIndex] = new_car;
    
    while( currentIndex != 0 && carFleet -> maxHeap[currentIndex] > carFleet -> maxHeap[(currentIndex-1)/2]){
            swap( &carFleet -> maxHeap[currentIndex], &carFleet -> maxHeap[(currentIndex-1)/2]);
            currentIndex = (currentIndex-1)/2;
    }
}

void printMaxHeap(int* array, int dim){
    int i;
    for(i = 0; i < dim; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
}

bool removeFromCarFleet(CarFleet* carFleet, int removed_car){
    if(carFleet->currentHeapSize == 0) return false;

    int last_index = carFleet -> currentHeapSize-1;
    int i;

    for( i = 0; i < carFleet -> currentHeapSize; i++){
        if( carFleet -> maxHeap[i] == removed_car)
            break;
    }
    if( i == carFleet -> currentHeapSize)
        return false; //Car not Found

    carFleet -> maxHeap[i] = carFleet -> maxHeap[last_index];

    carFleet -> currentHeapSize--; //Funziona? Cosa Cambia al posto di fare una chiamata con max_heapify e valore di Heap ridotto di 1?
    max_heapify( carFleet, i);

    return true;
}

//Binary Search Tree
typedef struct BSTNode{
    Station* key;
    struct BSTNode* p;
    struct BSTNode* left;
    struct BSTNode* right;
}BSTNode;

BSTNode* bstSearch(BSTNode* root, int key){
    while( root != NULL && (root -> key -> id != key)){
        if(key < root -> key -> id)
            root = root -> left;
        else
            root = root -> right;
    }
    return root;
}

BSTNode* bstMinimum(BSTNode* root){
    while(root -> left != NULL)
        root = root -> left;
    return root;
}

BSTNode* bstMaximum(BSTNode* root){
    while(root -> right != NULL)
        root = root -> right;
    return root;
}

BSTNode* bstSuccessor(BSTNode* targetNode){
    BSTNode* parent = NULL;
    if( targetNode -> right != NULL)
        return bstMinimum( targetNode -> right);
    parent = targetNode -> p;

    while( parent != NULL && targetNode == parent -> right){
        targetNode = parent;
        parent = parent -> p;
    }
    return parent;
}

BSTNode* bstPredecessor(BSTNode* targetNode){
    BSTNode* parent = NULL;
    if(targetNode -> left != NULL)
        return bstMaximum( targetNode -> left);
    parent = targetNode -> p;

    while( parent != NULL && targetNode == parent -> left){
        targetNode = parent;
        parent = parent -> p;
    }
    return parent;
}

BSTNode* bstInsert(BSTNode* root, Station* key){
    BSTNode* new_node = (BSTNode*) malloc (sizeof(BSTNode));
    new_node -> key = key;
    new_node -> left = NULL;
    new_node -> right = NULL;
    new_node -> p = NULL;

    BSTNode* parent = NULL;
    BSTNode* current_root = root;

    while( current_root != NULL){
        parent = current_root;
        if( key -> id < current_root -> key -> id)
            current_root = current_root -> left;
        else
            current_root = current_root -> right;
    }
    new_node -> p = parent;

    if( parent == NULL)
        return new_node; //as Root;
    else if( key -> id < parent -> key -> id){
        parent -> left = new_node;
        return root;
    }else{
        parent -> right = new_node;
        return root;
    }
}

BSTNode* transplant(BSTNode* root, BSTNode* u, BSTNode* v){
    if(u -> p == NULL)
        root = v; //as Root
    else if(u == u -> p -> left)
        u -> p -> left = v;
    else
        u -> p -> right = v;

    if( v != NULL)
        v -> p = u -> p;

    return root;
}

BSTNode* bstDelete(BSTNode* root, BSTNode* to_delete){
    BSTNode* successor = NULL;

    if(to_delete -> left == NULL)
        root = transplant(root, to_delete, to_delete -> right);
    else if(to_delete -> right == NULL)
        root = transplant(root, to_delete, to_delete -> left);
    else{
        successor = bstMinimum( to_delete -> right);
        if( successor -> p != to_delete){
            root = transplant(root, successor, successor -> right);
            successor -> right = to_delete -> right;
            successor -> right -> p = successor;
        }
        root = transplant(root, to_delete, successor);
        successor -> left = to_delete -> left;
        successor -> left -> p = successor;
    }
    if(to_delete -> key -> carFleet -> maxHeap != NULL)
        free(to_delete -> key -> carFleet -> maxHeap);
    if(to_delete -> key -> carFleet != NULL)
        free(to_delete -> key -> carFleet);
    if(to_delete -> key != NULL)
        free(to_delete -> key);
    free(to_delete);

    return root;
}

void reset_distance_treeWalk(BSTNode* root){
    if(root == NULL)
        return;
    reset_distance_treeWalk(root -> left);
    root -> key -> distance = INT_MAX;
    root -> key -> written = 0;
    root -> key -> linked_to = NULL;
    reset_distance_treeWalk(root -> right);
}

void inorderTreeWalk(BSTNode* root){
    if(root == NULL)
        return;
    inorderTreeWalk(root -> left);
    printf("%d - Auto:%d\n", root -> key -> id, getMaxCarFleet( root -> key -> carFleet));
    inorderTreeWalk(root -> right);
}

//QUEUE FOR Storing BFS-Neighbors 
//Implemented as a Queue and when's needed the size is increased by one (for Time/Memory optimization)

//If slow, change the constant growth of the size ( a x2, x4, n^2 growth ...)

//Questa implementazione non può deallocare le celle inizialmente allocate

typedef struct Queue{
    BSTNode** first_mem_cell;
    int front;   //end of the queue can be reached as queue->first_mem_cell[ queue->front + queue -> capacity]
    int capacity;
}Queue;

Queue* queue = NULL;
BSTNode* root = NULL;

Queue* createQueue(BSTNode* node){
    Queue* new_queue = (Queue*) malloc( (sizeof(Queue)));
    new_queue -> first_mem_cell = (BSTNode**) malloc( (sizeof(BSTNode*)));
    new_queue -> first_mem_cell[0] = node;
    new_queue -> front = 0;
    new_queue -> capacity = 1;
    return new_queue;
}

bool isEmpty(Queue* q){
    if( q->capacity == 0)
     return true;
    return false;
}

void enqueue(Queue* queue, BSTNode* item){
    if(queue -> capacity == 0){
        queue -> first_mem_cell[queue->front] = item;
        queue -> capacity++;
    }else{
        queue -> first_mem_cell = (BSTNode**) realloc ( queue -> first_mem_cell, ( queue -> front + queue -> capacity + 1)*sizeof( BSTNode*));
        queue -> first_mem_cell[queue -> front + queue -> capacity] = item;
        queue -> capacity++;
    }
}

void dequeue(Queue* queue){     
    if(queue->capacity == 0) return;

    queue -> capacity--;
    if(queue->capacity == 0){
        queue -> first_mem_cell[queue->front] = NULL;
    }
    else
        queue -> front++;
}

void print_queue(Queue* queue){
    int i;

    if( queue -> capacity == 0){
        printf("No elem\n");
        return;
    }

    for( i = 0; i < queue -> capacity; i++){
        printf("Elemento: %d\n", queue ->first_mem_cell[(queue->front)+i] -> key -> id);
    }
    printf("-------\n");
}

void destroyQueue(Queue* queue){
    free(queue->first_mem_cell);
    free(queue);
}

void pianifica_percorso_diretto(int start, int end){
    reset_distance_treeWalk(root);

    BSTNode* successor_node = NULL; 
    BSTNode* current_node = NULL;
    BSTNode* highest_node_reached = NULL;
    int max_distance_reachable = 0;

    current_node = bstSearch(root, start);
    queue = createQueue( current_node);
    current_node -> key -> written = 1;

    highest_node_reached = current_node;
    current_node -> key -> linked_to = current_node -> key;

    do{
        max_distance_reachable = current_node -> key -> id + getMaxCarFleet(current_node -> key -> carFleet);
        if(max_distance_reachable >= (highest_node_reached) -> key -> id){
            successor_node = bstSuccessor(highest_node_reached);
            if(successor_node -> key -> id > max_distance_reachable ){
                dequeue(queue);
                if(queue -> capacity != 0)
                    current_node = queue -> first_mem_cell[queue -> front];
                continue;
            }
            while(1){
                if(successor_node -> key -> id <= max_distance_reachable && successor_node -> key -> written == 0){
                    enqueue(queue, successor_node);
                    successor_node -> key -> written = 1;
                    successor_node -> key -> linked_to = current_node -> key;
                    highest_node_reached = successor_node;

                    if(successor_node -> key -> id == end )
                        break;
                    
                    successor_node = bstSuccessor( successor_node);
                }
                else if( successor_node -> key -> written == 1)
                    continue;
                else{
                    break;
                }
            }
            if(successor_node -> key -> id == end && successor_node -> key -> written == 1)
                break;

            dequeue(queue);
            current_node = queue -> first_mem_cell[queue -> front]; //Nodo successivo in testa, si può fare anche in altri modi
        }
        else{
            current_node -> key -> written = 1;
            dequeue(queue);
            current_node = queue -> first_mem_cell[queue -> front];
        }
    }while(!isEmpty(queue));

    if(isEmpty(queue)){
        printf("nessun percorso\n");
    }
    else{
        int* array_to_be_printed = (int*) malloc( sizeof(int*));
        int counter = 1;
        array_to_be_printed[0] = successor_node -> key -> id;

        Station* cursor = successor_node -> key -> linked_to;
        while( cursor -> linked_to -> id != cursor -> id){
            array_to_be_printed = (int*) realloc (array_to_be_printed, (counter+1)*sizeof(int*));
            array_to_be_printed[counter++] = cursor -> id;
            cursor = cursor -> linked_to;
        }
        array_to_be_printed = (int*) realloc (array_to_be_printed, (counter+1)*sizeof(int*));
        array_to_be_printed[counter++] = cursor -> id;

        for(; counter != 0; counter--){
            if(counter == 1)
                printf("%d", array_to_be_printed[counter-1]);
            else
                printf("%d ", array_to_be_printed[counter-1]);
        }
        printf("\n");

        free(array_to_be_printed);
    }
    destroyQueue(queue);
}

void pianifica_percorso_inverso_lineare(int start, int end){
    reset_distance_treeWalk(root);
    int current_max_distance_reachable = 0, next_max_distance_reachable = INT_MAX, temp_distance, distance = 1;
    bool path_not_found = false, multiple_nodes_same_distance = false, equal = false;
    BSTNode* current_node = NULL;

    /*
    1. Nella prima iterazione l'algoritmo deve assegnare il valore di distanza di un nodo a partire dalla stazione di inizio del percorso a tutti i nodi
    2. A partire dal nodo che si vuole raggiungere, si leggono rispettivamente id e distanza, si risalgono i nodi fino a che non incontro un nodo a distanza-1, da qui
       eseguo il calcolo per ogni nodo di max_distance_reachable, fino a che non individuo il primo nodo che raggiunge il target, scrivo nel nodo precendente che sarà puntato
       da un puntatore qual è il suo precedente
    3. Rieseguo il punto due con il nuovo nodo target
    */

    current_node = bstSearch(root, start);
    current_node -> key -> linked_to = current_node -> key;

    current_max_distance_reachable = current_node -> key -> id - getMaxCarFleet(current_node -> key -> carFleet);

    while(true){
        temp_distance = current_node -> key -> id - getMaxCarFleet(current_node -> key -> carFleet);

        if(current_max_distance_reachable < end)
            current_max_distance_reachable = end;

        if(current_node -> key -> id > current_max_distance_reachable){
            multiple_nodes_same_distance = true;
            current_node -> key -> distance = distance;
            if(temp_distance <= next_max_distance_reachable){
                next_max_distance_reachable = temp_distance;
            }
        }
        else{
            if(current_node -> key -> id < current_max_distance_reachable && next_max_distance_reachable > current_node -> key -> id){
                path_not_found = true;
                break;
            }
            if(current_node -> key -> id == current_max_distance_reachable){
                equal = true;
                current_node -> key -> distance = distance;
                if(temp_distance <= next_max_distance_reachable){
                    next_max_distance_reachable = temp_distance;
                }
            }
            else if(current_node -> key -> id < current_max_distance_reachable){
                multiple_nodes_same_distance = false;
                current_node -> key -> distance = distance+1;
            }

            if(current_node -> key -> id == end && !multiple_nodes_same_distance){ 
                distance++;
                break; 
            }else if(current_node -> key -> id == end && multiple_nodes_same_distance){ 
                break; 
            }

            current_max_distance_reachable = next_max_distance_reachable;
            if(equal)
                next_max_distance_reachable = INT_MAX;
            else
                next_max_distance_reachable = temp_distance;

            distance++;
            multiple_nodes_same_distance = false;
            equal = false;
        }
        if(current_node -> key -> id == start)
            current_node -> key -> distance = 0;

        current_node = bstPredecessor(current_node);
    }

    if(path_not_found)
        printf("nessun percorso\n");
    else{
        //PUNTO 2 DELL'ALGORITMO
        BSTNode* node_to_reach = current_node;
        BSTNode* end_node = current_node;

        while(current_node -> key -> id != start){
            if(distance-1 != current_node -> key -> distance){
                current_node = bstSuccessor( current_node);
            }
            else{
                temp_distance = current_node -> key -> id - getMaxCarFleet(current_node -> key -> carFleet);
                if(temp_distance <= node_to_reach -> key -> id ){
                    node_to_reach -> key -> linked_to = current_node -> key;
                    node_to_reach = current_node;
                    distance--;
                }
                current_node = bstSuccessor( current_node);
            }
        }
        node_to_reach -> key -> linked_to = current_node -> key;

        current_node = end_node;

        int* array_to_be_printed = (int*) malloc( sizeof(int*));
        int counter = 1;
        array_to_be_printed[0] = current_node -> key -> id;

        Station* cursor = current_node -> key -> linked_to;
        while( cursor -> linked_to -> id != cursor -> id){
            array_to_be_printed = (int*) realloc (array_to_be_printed, (counter+1)*sizeof(int*));
            array_to_be_printed[counter++] = cursor -> id;
            cursor = cursor -> linked_to;
        }
        array_to_be_printed = (int*) realloc (array_to_be_printed, (counter+1)*sizeof(int*));
        array_to_be_printed[counter++] = cursor -> id;

        for(; counter != 0; counter--){
            if(counter == 1)
                printf("%d", array_to_be_printed[counter-1]);
            else
                printf("%d ", array_to_be_printed[counter-1]);
        }
        printf("\n");

        free(array_to_be_printed);
    }
}

//TO DO
//Fare un parser anche per la lettura dei numeri in input
//CAPACITY di CarFleet ora è statica a 512 elementi

void read_input(){
    char command_buffer[COMMAND_SIZE], c;
    int counter=0, distanza=0, num_auto=0, i, car;

    while( (c = getchar_unlocked()) != EOF){
        if(c != ' '){
            command_buffer[counter++] = c;
        }
        else{
            if( strncmp(command_buffer, "aggiungi-stazione", 17) == 0){
            while(counter != 0){
                counter--;
                command_buffer[counter] = ' ';
            }

            if(scanf("%d %d", &distanza, &num_auto));

            if( bstSearch(root, distanza)){ //Se la stazione esiste, non faccio nulla
                printf("non aggiunta\n");
                for( i = 0; i < num_auto; i++)
                    if(scanf("%d", &car));
                getchar_unlocked();
                continue;
            }

            Station* station = (Station*) malloc ( sizeof(Station));
            station -> id = distanza;
            station -> written = 0;
            station -> linked_to = NULL;
            station -> distance = INT_MAX;
            station -> carFleet = createCarFleet(512); //CAMBIARE LA CAPACITY INIZIALE 

            root = bstInsert(root, station);
                
            for( i = 0; i < num_auto; i++){
                if(scanf("%d", &car));
                insertIntoCarFleet( station->carFleet, car);
            }
            //printMaxHeap(station->carFleet->maxHeap, station->carFleet->currentHeapSize);

            printf("aggiunta\n");
            }
            else if( strncmp(command_buffer, "demolisci-stazione", 18) == 0){
                while(counter != 0){
                    counter--;
                    command_buffer[counter] = ' ';
                }

                if(scanf("%d", &distanza));
                BSTNode* to_delete = NULL;
                if( (to_delete = bstSearch(root, distanza) )){
                    root = bstDelete(root, to_delete);
                    printf("demolita\n");
                }
                else
                    printf("non demolita\n");
            }
            else if( strncmp(command_buffer, "aggiungi-auto", 13) == 0){
                while(counter != 0){
                    counter--;
                    command_buffer[counter] = ' ';
                }

                if(scanf("%d %d", &distanza, &car));

                BSTNode* node = NULL;
                if( (node = bstSearch(root, distanza) )){
                    insertIntoCarFleet( node -> key -> carFleet, car);
                    printf("aggiunta\n");
                }
                else
                    printf("non aggiunta\n");
            }
            else if( strncmp(command_buffer, "rottama-auto", 12) == 0){
                bool flag_found = false;
                while(counter != 0){
                    counter--;
                    command_buffer[counter] = ' ';
                }
                if(scanf("%d %d", &distanza, &car));

                BSTNode* node = NULL;
                if( (node = bstSearch(root, distanza)) != NULL ){
                    if( node -> key -> carFleet -> currentHeapSize != 0){
                        flag_found = removeFromCarFleet(node -> key -> carFleet, car);
                    }
                }
                if(flag_found)
                    printf("rottamata\n");
                else
                    printf("non rottamata\n");
            }
            else if( strncmp(command_buffer, "pianifica-percorso", 18) == 0){
                while(counter != 0){
                    counter--;
                    command_buffer[counter] = ' ';
                }

                int start=0, end=0;
                if(scanf("%d %d", &start, &end));

                if(start == end)
                    printf("%d\n", start);
                else if(start < end) 
                    pianifica_percorso_diretto(start, end);
                else{
                    pianifica_percorso_inverso_lineare(start, end);
                }
            }
            getchar_unlocked();
        }
    }
}

int main(int argc, char* argv[]){
    read_input();
    return 0;
}