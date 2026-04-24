/** pagequeue.c
 * ===========================================================
 * Name: _________Gavin Smith______________, _23_ __APR_ 2026
 * Section: CS483 / __M3__
 * Project: PEX3 - Page Replacement Simulator
 * Purpose: Implementation of the PageQueue ADT — a doubly-linked
 *          list for LRU page replacement.
 *          Head = LRU (eviction end), Tail = MRU end.
 * =========================================================== */
#include <stdio.h>
#include <stdlib.h>

#include "pagequeue.h"

/**
 * @brief Create and initialize a page queue with a given capacity
 */
PageQueue *pqInit(unsigned int maxSize) {
    // TODO: malloc a PageQueue, set head and tail to NULL,
    //       size to 0, maxSize to maxSize, and return the pointer

    //malloc the pq and set values to their intials
    PageQueue* pq = (PageQueue*) malloc(sizeof(PageQueue));
    pq->head = NULL;
    pq->tail = NULL;
    pq->maxSize = maxSize;
    pq->size = 0;
    return pq;
}

void qInsert(PageQueue* pq, unsigned long page){
    if (!(pq->head)) {   // empty queue — new node becomes both head and tail
        pq->head = (PqNode*) malloc(sizeof(PqNode));
        pq->tail = pq->head;
        pq->head->pageNum = page;
        pq->size++;
        pq->head->next = NULL;
        pq->head->prev = NULL;
    } else {            // non-empty — append new node at the tail
        PqNode* oldTail = pq->tail;
        PqNode* newTail = (PqNode*) malloc(sizeof(PqNode));
        newTail->next = NULL;
        newTail->prev = oldTail;
        newTail->pageNum = page;
        oldTail->next = newTail;
        pq->size++;
        pq->tail = newTail;
    }
}

void qRemove(PageQueue* pq, int which) {
    // walk forward to the requested position
    PqNode* curr = pq->head;
    while (curr && which > 0) {
        curr = curr->next;
        which--;
    }

    if (!curr) {                                // position is out of bounds or queue is empty
        return;
    } else if (curr == pq->head && curr == pq->tail) {   // only node in the list
        pq->head = NULL;
        pq->tail = NULL;
    } else if (curr == pq->head) {              // removing the head
        pq->head = curr->next;
        pq->head->prev = NULL;
    } else if (curr == pq->tail) {              // removing the tail
        pq->tail = curr->prev;
        pq->tail->next = NULL;
    } else {                                   // removing an interior node
        curr->prev->next = curr->next;
        curr->next->prev = curr->prev;
    }

    pq->size--;

    // PqNode* removed = curr;
    free(curr);
    return;
}

/**
 * @brief Access a page in the queue (simulates a memory reference)
 */
long pqAccess(PageQueue *pq, unsigned long pageNum) {
    // TODO: Search the queue for pageNum (suggest searching tail->head
    //       so you naturally count depth from the MRU end).
    //
    // HIT path (page found at depth d):
    //   - Remove the node from its current position and re-insert
    //     it at the tail (most recently used).
    //   - Return d.
    //
    // MISS path (page not found):
    //   - Allocate a new node for pageNum and insert it at the tail.
    //   - If size now exceeds maxSize, evict the head node (free it).
    //   - Return -1.

    //initalize the counter, depth, and curr var to hold the incrementing nodes
    int i = 0;
    int depth = -1;
    PqNode* curr = pq->tail;

    //if curr is NULL skip to end
    if (curr)   {
    

        //increment curr until it reaches the pagenum or the end of the queue
        while ((i< pq->size)&&(curr->pageNum != pageNum)){
    
            i++;

            curr = curr->prev;
        }


        //if it is null skip becuase it reached the end of the queue
        if (curr){
            //ensure same pagenum then remove it and add it to the LRU spot
            if (curr->pageNum == pageNum)   {
                depth = i;
                qRemove(pq, (pq->size - 1)- depth);
                qInsert(pq, pageNum);
                return depth;
            }

            //error case, just insert and remove for max size
            else 
            {
                qInsert(pq, pageNum);

                if (pq->size > pq->maxSize)   {
                    qRemove(pq, 0);
                }

                return -1;
            }
        }

    }

    //Must not be in queue so it is a miss, add to queue and enforce max size. Then return -1
    qInsert(pq,pageNum);

    if (pq->size > pq->maxSize)   {
        qRemove(pq, 0);
    }   
    
    return -1;
}

/**
 * @brief Free all nodes in the queue and reset it to empty
 */
void pqFree(PageQueue *pq) {
    // TODO: Walk from head to tail, free each node, then free
    //       the PageQueue struct itself.

    //if there is no list return
    if (pq == NULL)   {
        printf("Queue does not exist.\n");
        return;
    }

    // if there is no nodes just free the list
    if (pq->head == NULL)   {
        free(pq);
        return;
    }

    //free all nodes first
    PqNode* currentNode = pq->head;
    PqNode* prevNode = NULL;

    while (currentNode != NULL)   {
        prevNode = currentNode;
        currentNode =currentNode->next;

        prevNode->next = NULL;
        prevNode->prev = NULL;
        free(prevNode);
    }
    pq->head = NULL;

    //free list
    free(pq);
}

/**
 * @brief Print queue contents to stderr for debugging
 */
void pqPrint(PageQueue *pq) {
    // TODO (optional): Print each page number from head to tail,
    //                  marking which is head and which is tail.
    //                  Useful for desk-checking small traces.

    //set temp value to increment through queue.
    PqNode* curr = pq->head;

    //loop through pq and print page number
    for (int i = 0; i < pq->size; i++)
    {
        if (curr)
        {
            printf("%d:%lu\n", i, curr->pageNum);
        }

        if (i!= pq->size - 1)   {
            curr = curr->next;
        }
        

    }
    
}
