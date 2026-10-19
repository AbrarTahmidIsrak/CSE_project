#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 101
#define MAX_QUEUE 100
#define MAX_STACK 100



typedef struct Donor {
    int id;
    char name[50];
    int age;
    char gender[15];
    char bloodGroup[5];
    char phone[20];
    char address[100];
    char lastDonation[20];

    struct Donor *next;
} Donor;



Donor *head = NULL;




typedef struct BSTNode {
    Donor *donor;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

BSTNode *root = NULL;


//Hash Table

typedef struct HashNode {
    int id;
    Donor *donor;
    struct HashNode *next;
} HashNode;

HashNode *hashTable[TABLE_SIZE];



   //BLOOD REQUEST QUEUE

typedef struct {
    int requestId;
    char patientName[50];
    char bloodGroup[5];
    int units;
} BloodRequest;

BloodRequest normalQueue[MAX_QUEUE];

int front = 0;
int rear = -1;



   //PRIORITY QUEUE / MAX HEAP


typedef struct {
    BloodRequest request;
    int priority;
} EmergencyRequest;

EmergencyRequest heap[MAX_QUEUE];

int heapSize = 0;



   //STACK FOR UNDO


typedef struct {
    int operation;
    Donor donor;
} UndoRecord;

UndoRecord undoStack[MAX_STACK];

int top = -1;



   //UTILITY FUNCTIONS


void clearInputBuffer() {
    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}


void readString(char *str, int size) {
    fgets(str, size, stdin);

    str[strcspn(str, "\n")] = '\0';
}



   //LINKED LIST FUNCTIONS


Donor *createDonor() {

    Donor *newDonor;

    newDonor = (Donor *)malloc(sizeof(Donor));

    if (newDonor == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    printf("\nEnter Donor ID: ");
    scanf("%d", &newDonor->id);
    clearInputBuffer();

    printf("Enter Name: ");
    readString(newDonor->name, 50);

    printf("Enter Age: ");
    scanf("%d", &newDonor->age);
    clearInputBuffer();

    printf("Enter Gender: ");
    readString(newDonor->gender, 15);

    printf("Enter Blood Group: ");
    readString(newDonor->bloodGroup, 5);

    printf("Enter Phone Number: ");
    readString(newDonor->phone, 20);

    printf("Enter Address: ");
    readString(newDonor->address, 100);

    printf("Enter Last Donation Date: ");
    readString(newDonor->lastDonation, 20);

    newDonor->next = NULL;

    return newDonor;
}


Donor *findDonorByID(int id) {

    Donor *temp = head;

    while (temp != NULL) {

        if (temp->id == id)
            return temp;

        temp = temp->next;
    }

    return NULL;
}


void displayDonor(Donor *d) {

    printf("\n-----------------------------\n");

    printf("ID                 : %d\n", d->id);
    printf("Name               : %s\n", d->name);
    printf("Age                : %d\n", d->age);
    printf("Gender             : %s\n", d->gender);
    printf("Blood Group        : %s\n", d->bloodGroup);
    printf("Phone              : %s\n", d->phone);
    printf("Address            : %s\n", d->address);
    printf("Last Donation Date : %s\n", d->lastDonation);

    printf("-----------------------------\n");
}


void insertDonorLinkedList(Donor *newDonor) {

    newDonor->next = head;
    head = newDonor;
}


void displayAllDonors() {

    Donor *temp = head;

    if (temp == NULL) {
        printf("\nNo donors available.\n");
        return;
    }

    printf("\n========== ALL DONORS ==========\n");

    while (temp != NULL) {

        displayDonor(temp);

        temp = temp->next;
    }
}



   //BST FUNCTIONS


BSTNode *createBSTNode(Donor *donor) {

    BSTNode *newNode;

    newNode = (BSTNode *)malloc(sizeof(BSTNode));

    if (newNode == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    newNode->donor = donor;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}


BSTNode *insertBST(BSTNode *root, Donor *donor) {

    if (root == NULL)
        return createBSTNode(donor);

    if (donor->id < root->donor->id)
        root->left = insertBST(root->left, donor);

    else if (donor->id > root->donor->id)
        root->right = insertBST(root->right, donor);

    return root;
}


Donor *searchBST(BSTNode *root, int id) {

    if (root == NULL)
        return NULL;

    if (root->donor->id == id)
        return root->donor;

    if (id < root->donor->id)
        return searchBST(root->left, id);

    return searchBST(root->right, id);
}


void inorderBST(BSTNode *root) {

    if (root != NULL) {

        inorderBST(root->left);

        displayDonor(root->donor);

        inorderBST(root->right);
    }
}


void freeBST(BSTNode *root) {

    if (root == NULL)
        return;

    freeBST(root->left);
    freeBST(root->right);

    free(root);
}



   //HASH TABLE


int hashFunction(int id) {

    return id % TABLE_SIZE;
}


void clearHashTable() {

    int i;

    for (i = 0; i < TABLE_SIZE; i++)
        hashTable[i] = NULL;
}


void insertHash(Donor *donor) {

    int index = hashFunction(donor->id);

    HashNode *newNode;

    newNode = (HashNode *)malloc(sizeof(HashNode));

    newNode->id = donor->id;
    newNode->donor = donor;

    newNode->next = hashTable[index];

    hashTable[index] = newNode;
}


Donor *searchHash(int id) {

    int index = hashFunction(id);

    HashNode *temp = hashTable[index];

    while (temp != NULL) {

        if (temp->id == id)
            return temp->donor;

        temp = temp->next;
    }

    return NULL;
}


void freeHashTable() {

    int i;

    for (i = 0; i < TABLE_SIZE; i++) {

        HashNode *temp = hashTable[i];

        while (temp != NULL) {

            HashNode *next = temp->next;

            free(temp);

            temp = next;
        }

        hashTable[i] = NULL;
    }
}



   //REBUILD BST AND HASH TABLE


void rebuildStructures() {

    freeBST(root);

    root = NULL;

    freeHashTable();

    Donor *temp = head;

    while (temp != NULL) {

        root = insertBST(root, temp);

        insertHash(temp);

        temp = temp->next;
    }
}



   //REGISTER DONOR


void registerDonor() {

    Donor *newDonor;

    int id;

    printf("\nEnter Donor ID: ");
    scanf("%d", &id);
    clearInputBuffer();

    if (findDonorByID(id) != NULL) {

        printf("Donor ID already exists!\n");

        return;
    }

    newDonor = (Donor *)malloc(sizeof(Donor));

    if (newDonor == NULL) {

        printf("Memory allocation failed!\n");

        return;
    }

    newDonor->id = id;

    printf("Enter Name: ");
    readString(newDonor->name, 50);

    printf("Enter Age: ");
    scanf("%d", &newDonor->age);
    clearInputBuffer();

    printf("Enter Gender: ");
    readString(newDonor->gender, 15);

    printf("Enter Blood Group: ");
    readString(newDonor->bloodGroup, 5);

    printf("Enter Phone Number: ");
    readString(newDonor->phone, 20);

    printf("Enter Address: ");
    readString(newDonor->address, 100);

    printf("Enter Last Donation Date: ");
    readString(newDonor->lastDonation, 20);

    newDonor->next = NULL;

    insertDonorLinkedList(newDonor);

    rebuildStructures();

    printf("\nDonor registered successfully!\n");
}



   //SEARCH BY BLOOD GROUP


void searchByBloodGroup() {

    char group[5];

    int found = 0;

    Donor *temp = head;

    printf("\nEnter Blood Group: ");
    readString(group, 5);

    printf("\nDonors with blood group %s:\n", group);

    while (temp != NULL) {

        if (strcasecmp(temp->bloodGroup, group) == 0) {

            displayDonor(temp);

            found = 1;
        }

        temp = temp->next;
    }

    if (!found)
        printf("No donor found with blood group %s.\n", group);
}



   //UPDATE DONOR


void pushUndo(int operation, Donor *donor) {

    if (top == MAX_STACK - 1) {

        printf("Undo stack is full!\n");

        return;
    }

    top++;

    undoStack[top].operation = operation;

    undoStack[top].donor = *donor;
}


void updateDonor() {

    int id;

    Donor *donor;

    printf("\nEnter Donor ID to update: ");
    scanf("%d", &id);
    clearInputBuffer();

    donor = searchBST(root, id);

    if (donor == NULL) {

        printf("Donor not found!\n");

        return;
    }

    //Save old information

    pushUndo(1, donor);

    printf("\nEnter new information:\n");

    printf("Enter Name: ");
    readString(donor->name, 50);

    printf("Enter Age: ");
    scanf("%d", &donor->age);
    clearInputBuffer();

    printf("Enter Gender: ");
    readString(donor->gender, 15);

    printf("Enter Blood Group: ");
    readString(donor->bloodGroup, 5);

    printf("Enter Phone Number: ");
    readString(donor->phone, 20);

    printf("Enter Address: ");
    readString(donor->address, 100);

    printf("Enter Last Donation Date: ");
    readString(donor->lastDonation, 20);

    rebuildStructures();

    printf("\nDonor information updated successfully!\n");
}



   //DELETE DONOR


void deleteDonor() {

    int id;

    Donor *temp;
    Donor *previous = NULL;

    printf("\nEnter Donor ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    temp = head;

    while (temp != NULL && temp->id != id) {

        previous = temp;
        temp = temp->next;
    }

    if (temp == NULL) {

        printf("Donor not found!\n");

        return;
    }

    /* Save deleted donor for undo */

    pushUndo(2, temp);

    if (previous == NULL)
        head = temp->next;

    else
        previous->next = temp->next;

    free(temp);

    rebuildStructures();

    printf("\nDonor deleted successfully!\n");
}


//UNDO

void undoOperation() {

    if (top == -1) {

        printf("\nNothing to undo.\n");

        return;
    }

    UndoRecord record = undoStack[top];

    top--;

    if (record.operation == 1) {

        /* Undo UPDATE */

        Donor *donor = searchBST(root, record.donor.id);

        if (donor != NULL) {

            donor->id = record.donor.id;
            strcpy(donor->name, record.donor.name);
            donor->age = record.donor.age;
            strcpy(donor->gender, record.donor.gender);
            strcpy(donor->bloodGroup, record.donor.bloodGroup);
            strcpy(donor->phone, record.donor.phone);
            strcpy(donor->address, record.donor.address);
            strcpy(donor->lastDonation, record.donor.lastDonation);

            rebuildStructures();

            printf("\nUpdate operation undone.\n");
        }
    }

    else if (record.operation == 2) {

        /* Undo DELETE */

        Donor *newDonor;

        newDonor = (Donor *)malloc(sizeof(Donor));

        *newDonor = record.donor;

        newDonor->next = head;

        head = newDonor;

        rebuildStructures();

        printf("\nDelete operation undone.\n");
    }
}



   //COUNT DONORS BY BLOOD GROUP


void countBloodGroups() {

    char groups[8][5] = {
        "A+",
        "A-",
        "B+",
        "B-",
        "AB+",
        "AB-",
        "O+",
        "O-"
    };

    int count[8] = {0};

    int i;

    Donor *temp = head;

    while (temp != NULL) {

        for (i = 0; i < 8; i++) {

            if (strcasecmp(temp->bloodGroup, groups[i]) == 0) {

                count[i]++;

                break;
            }
        }

        temp = temp->next;
    }

    printf("\n========== BLOOD GROUP COUNT ==========\n");

    for (i = 0; i < 8; i++) {

        printf("%-4s : %d donors\n", groups[i], count[i]);
    }
}



   //NORMAL QUEUE


void addNormalRequest() {

    if (rear == MAX_QUEUE - 1) {

        printf("Normal request queue is full!\n");

        return;
    }

    rear++;

    printf("\nEnter Request ID: ");
    scanf("%d", &normalQueue[rear].requestId);
    clearInputBuffer();

    printf("Enter Patient Name: ");
    readString(normalQueue[rear].patientName, 50);

    printf("Enter Required Blood Group: ");
    readString(normalQueue[rear].bloodGroup, 5);

    printf("Enter Required Units: ");
    scanf("%d", &normalQueue[rear].units);
    clearInputBuffer();

    printf("\nNormal blood request added.\n");
}


void processNormalRequest() {

    int i;

    if (front > rear) {

        printf("\nNo normal blood requests.\n");

        return;
    }

    printf("\nProcessing request:\n");

    printf("Request ID: %d\n",
           normalQueue[front].requestId);

    printf("Patient: %s\n",
           normalQueue[front].patientName);

    printf("Blood Group: %s\n",
           normalQueue[front].bloodGroup);

    printf("Units: %d\n",
           normalQueue[front].units);

    for (i = front; i < rear; i++)
        normalQueue[i] = normalQueue[i + 1];

    rear--;

    printf("\nRequest processed successfully.\n");
}


   //PRIORITY QUEUE / MAX HEAP


void swapEmergency(EmergencyRequest *a,
                   EmergencyRequest *b) {

    EmergencyRequest temp = *a;

    *a = *b;

    *b = temp;
}


void addEmergencyRequest() {

    int i;

    EmergencyRequest newRequest;

    if (heapSize == MAX_QUEUE) {

        printf("Emergency queue is full!\n");

        return;
    }

    printf("\nEnter Request ID: ");
    scanf("%d", &newRequest.request.requestId);
    clearInputBuffer();

    printf("Enter Patient Name: ");
    readString(newRequest.request.patientName, 50);

    printf("Enter Required Blood Group: ");
    readString(newRequest.request.bloodGroup, 5);

    printf("Enter Required Units: ");
    scanf("%d", &newRequest.request.units);
    clearInputBuffer();

    printf("Enter Priority (1-10): ");
    scanf("%d", &newRequest.priority);
    clearInputBuffer();

    i = heapSize;

    heap[heapSize] = newRequest;

    heapSize++;

    /* Heapify Up */

    while (i > 0) {

        int parent = (i - 1) / 2;

        if (heap[parent].priority >= heap[i].priority)
            break;

        swapEmergency(&heap[parent], &heap[i]);

        i = parent;
    }

    printf("\nEmergency request added.\n");
}


void processEmergencyRequest() {

    EmergencyRequest current;

    int i;
    int largest;
    int left;
    int right;

    if (heapSize == 0) {

        printf("\nNo emergency requests.\n");

        return;
    }

    current = heap[0];

    printf("\nProcessing EMERGENCY request:\n");

    printf("Request ID: %d\n",
           current.request.requestId);

    printf("Patient: %s\n",
           current.request.patientName);

    printf("Blood Group: %s\n",
           current.request.bloodGroup);

    printf("Units: %d\n",
           current.request.units);

    printf("Priority: %d\n",
           current.priority);

    heap[0] = heap[heapSize - 1];

    heapSize--;

    /* Heapify Down */

    i = 0;

    while (1) {

        left = 2 * i + 1;
        right = 2 * i + 2;
        largest = i;

        if (left < heapSize &&
            heap[left].priority > heap[largest].priority)

            largest = left;

        if (right < heapSize &&
            heap[right].priority > heap[largest].priority)

            largest = right;

        if (largest == i)
            break;

        swapEmergency(&heap[i], &heap[largest]);

        i = largest;
    }

    printf("\nEmergency request processed.\n");
}



   //SEARCH DONOR USING HASH TABLE


void searchDonorHash() {

    int id;

    Donor *donor;

    printf("\nEnter Donor ID: ");
    scanf("%d", &id);
    clearInputBuffer();

    donor = searchHash(id);

    if (donor == NULL)
        printf("\nDonor not found.\n");

    else {

        printf("\nDonor found :\n");

        displayDonor(donor);
    }
}



   //SEARCH DONOR USING BST


void searchDonorBST() {

    int id;

    Donor *donor;

    printf("\nEnter Donor ID: ");
    scanf("%d", &id);
    clearInputBuffer();

    donor = searchBST(root, id);

    if (donor == NULL)
        printf("\nDonor not found.\n");

    else {

        printf("\nDonor found :\n");

        displayDonor(donor);
    }
}
//main menu
void menu() {

    printf("\n\n");
    printf("=============================================\n");
    printf("       BLOOD GROUP MANAGEMENT SYSTEM\n");
    printf("=============================================\n");

    printf("\n--- DONOR MANAGEMENT ---\n");

    printf("1.  Register Donor\n");
    printf("2.  Search Donor(BST) \n");
    printf("3.  Search Donor(Fast Search) \n");
    printf("4.  Search Donor by Blood Group\n");
    printf("5.  Update Donor\n");
    printf("6.  Delete Donor\n");
    printf("7.  Undo Last Update\n");
    printf("8.  Display All Donors\n");
    printf("9.  Count Donors by Blood Group\n");
    printf("10. Display Donors(Sorted by ID)\n");

    printf("\n--- BLOOD REQUEST MANAGEMENT ---\n");

    printf("11. Add Normal Blood Request\n");
    printf("12. Process Normal Blood Request\n");

    printf("13. Add Emergency Blood Request\n");
    printf("14. Process Emergency Blood Request\n");

    printf("\n0. Exit\n");

    printf("=============================================\n");

    printf("Enter your choice: ");
}



int main() {

    int choice;

    clearHashTable();

    do {

        menu();

        scanf("%d", &choice);

        clearInputBuffer();

        switch (choice) {

            case 1:
                registerDonor();
                break;

            case 2:
                searchDonorBST();
                break;

            case 3:
                searchDonorHash();
                break;

            case 4:
                searchByBloodGroup();
                break;

            case 5:
                updateDonor();
                break;

            case 6:
                deleteDonor();
                break;

            case 7:
                undoOperation();
                break;

            case 8:
                displayAllDonors();
                break;

            case 9:
                countBloodGroups();
                break;

            case 10:

                if (root == NULL)
                    printf("\nNo donors available.\n");

                else {

                    printf("\n===== DONORS SORTED BY ID =====\n");

                    inorderBST(root);
                }

                break;

            case 11:
                addNormalRequest();
                break;

            case 12:
                processNormalRequest();
                break;

            case 13:
                addEmergencyRequest();
                break;

            case 14:
                processEmergencyRequest();
                break;

            case 0:
                printf("\nThank you for using the system!\n");
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 0);


    /* Free memory */

    freeHashTable();

    freeBST(root);

    while (head != NULL) {

        Donor *temp = head;

        head = head->next;

        free(temp);
    }

    return 0;
}
