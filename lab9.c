/* COP 3502C Lab Assignment 9
This program is written by: Shreenandan Pendharkar */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType {
    int id;
    char name;
    int order; 
};

struct Node {
    struct RecordType data;
    struct Node* next;
};

struct HashType {
    struct Node** buckets;
    int size;
};

int hash(int x, int hashSize) {
    return (x * 2654435761 % hashSize);
}

struct HashType* createHashTable(int size) {
    struct HashType* hashTable = malloc(sizeof(struct HashType));
    if (!hashTable) {
        fprintf(stderr, "Memory allocation failed for hash table.\n");
        exit(EXIT_FAILURE);
    }
    hashTable->size = size;
    hashTable->buckets = calloc(size, sizeof(struct Node*));
    if (!hashTable->buckets) {
        fprintf(stderr, "Memory allocation failed for hash buckets.\n");
        exit(EXIT_FAILURE);
    }
    return hashTable;
}

void insertRecord(struct HashType* hashTable, struct RecordType newRecord) {
    int index = hash(newRecord.id, hashTable->size);
    struct Node* newNode = malloc(sizeof(struct Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed for new node.\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = newRecord;
    newNode->next = hashTable->buckets[index];
    hashTable->buckets[index] = newNode;
}

void displayRecordsInHash(struct HashType *hashTable) {
    for (int i = 0; i < hashTable->size; ++i) {
        struct Node* current = hashTable->buckets[i];
        if (current != NULL) {
            printf("Index %d -> ", i);
            while (current != NULL) {
                printf("(%d, %c, %d) -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
            printf("NULL\n");
        }
    }
}

struct RecordType* searchRecord(struct HashType* hashTable, int id) {
    int index = hash(id, hashTable->size);
    struct Node* current = hashTable->buckets[index];
    while (current) {
        if (current->data.id == id) {
            return &current->data;
        }
        current = current->next;
    }
    return NULL;
}

void deleteRecord(struct HashType* hashTable, int id) {
    int index = hash(id, hashTable->size);
    struct Node** current = &hashTable->buckets[index];
    while (*current) {
        struct Node* entry = *current;
        if (entry->data.id == id) {
            *current = entry->next;
            free(entry);
            return;
        }
        current = &entry->next;
    }
}

int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    if (!inFile) {
        fprintf(stderr, "Failed to open file: %s\n", inputFileName);
        exit(EXIT_FAILURE);
    }
    int dataSz = 0;
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = malloc(sizeof(struct RecordType) * dataSz);
    if (*ppData == NULL) {
        fprintf(stderr, "Cannot allocate memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < dataSz; ++i) {
        fscanf(inFile, "%d %c %d ", &((*ppData)[i].id), &((*ppData)[i].name), &((*ppData)[i].order));
    }
    fclose(inFile);
    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void) {
    struct RecordType *pRecords;
    int recordSz = parseData("input_lab_9.txt", &pRecords);
    struct HashType* hashTable = createHashTable(10);

    for (int i = 0; i < recordSz; i++) {
        insertRecord(hashTable, pRecords[i]);
    }
    printRecords(pRecords, recordSz);
    displayRecordsInHash(hashTable);

    struct RecordType* foundRecord = searchRecord(hashTable, pRecords[0].id);
    if (foundRecord) {
        printf("Record found: %d %c %d\n", foundRecord->id, foundRecord->name, foundRecord->order);
    }

    deleteRecord(hashTable, pRecords[0].id);
    printf("After deletion:\n");
    displayRecordsInHash(hashTable);

    for (int i = 0; i < hashTable->size; ++i) {
        struct Node *current = hashTable->buckets[i], *temp;
        while (current != NULL) {
            temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hashTable->buckets);
    free(hashTable);
    free(pRecords);

    return 0;
}
