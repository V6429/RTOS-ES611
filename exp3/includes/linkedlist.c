/**
 * @file functions.c
 * @author your name (you@domain.com)
 * @brief Dunctions for linked list manipulation
 * Modify the dummy codes for printing as per requirement
 * @version 0.1
 * @date 2022-01-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"

// create the required functions.

void clearNodeMem(NODE *ptr)
{
    free(ptr);
}

void printNode(NODE *const nodePtr)
{

    printf("\nNumberData   =%d", nodePtr->numberData);
    printf("\nNext node at =%p", nodePtr->nextNode);
}

NODE *createNode(int number)
{
    NODE *temp;
    temp = (NODE *)malloc(sizeof(NODE));
    temp->numberData = number;
    return (temp);
}

void printList(NODE *const headPtr)
{
    if (headPtr == NULL)
        printf("\nEMPTY LIST\n");
    int i = 0;
    NODE *tempPtr = headPtr;
    while (tempPtr != NULL)
    {
        printf("\n**********************\n\tNODE-%d\n*********************", i);
        printNode(tempPtr);
        tempPtr = tempPtr->nextNode;
        i++;
    }
}

// ***************************************LIST Functions**************************************8

NODE *insertListBeg(NODE *headPtr, NODE *newNODE)
{

    if (headPtr == NULL)
    {

        newNODE->nextNode = NULL;
        headPtr = newNODE;
    }
    else
    {
        newNODE->nextNode = headPtr;
        headPtr = newNODE;
    }
    return (headPtr);
}

NODE *insertListEND(NODE *headPtr, NODE *newNODE)
{

    NODE *tempPtr = headPtr; // creating  a copy for manipulation

    if (headPtr == NULL)
    {
        newNODE->nextNode = NULL;
        headPtr = newNODE;
    }
    else
    {
        while (tempPtr->nextNode != NULL)
        {
            tempPtr = tempPtr->nextNode;
        }
        // we have reached the last node on tempPtr
        newNODE->nextNode = NULL;
        tempPtr->nextNode = newNODE;
    }
    return (headPtr); // which can change if list is empty
}

NODE *insertListPos(NODE *headPtr, NODE *newNODE, int index)
{
    NODE *tempPtr;
    int i = 0;

    if (headPtr == NULL)
    { // if list empty and index 0
        if (index == 0)
        {
            headPtr = newNODE;
            newNODE->nextNode = NULL;
        }
        else
        {
            // printf("\nEmpty List");
            // printf("\n possible index 0\n");
            return NULL;
        }
    }
    else
    {

        // we need to change next @ n-1 location
        tempPtr = traverseListPos(headPtr, index - 1);
        if (tempPtr == NULL)
        {
            printf("\nINDEX ERROR");
            return NULL;
        }

        // adding original nth address to new node
        newNODE->nextNode = tempPtr->nextNode;
        //replacing nth position
        tempPtr->nextNode = newNODE;
    }

    return (headPtr);
}

NODE *deleteListBeg(NODE *headPtr)
{

    if (headPtr == NULL)
    {
        printf("The LIST is empty");
        return (NULL);
    }

    NODE *tempPtr = headPtr;
    headPtr = headPtr->nextNode;
    clearNodeMem(tempPtr);
    return (headPtr);
}

NODE* deleteListEND(NODE *headPtr)
{

    NODE *curPtr = headPtr; // creating  a copy for manipulation
    NODE *nextPtr;
    if (headPtr == NULL)
    {
        printf("Empty LIST");
        return NULL;
    }
    else
    {
        while (curPtr->nextNode != NULL)
        {
            // looking into next value of the curPtr+1 node
            nextPtr = curPtr->nextNode;
            if (nextPtr->nextNode == NULL)
            {
                // we have reached second last node
                clearNodeMem(nextPtr);   // delete last node
                curPtr->nextNode = NULL; // set current as last
                return(curPtr);
            }

            curPtr = curPtr->nextNode;
        }
        if(curPtr->nextNode ==NULL){ // case when list have 1 element
        clearNodeMem(curPtr);
        return NULL;
    
    
        }
        // we have reached the last node on curPtr
    }
}

void deleteListPos(NODE *headPtr, int index)
{

    NODE *tempPtrBehind, *tempPtr;

    if (headPtr == NULL)
    {
        printf("Empty LIST");
        return;
    }
    else
    {

        // we need to change next @ n-1 location
        tempPtrBehind = traverseListPos(headPtr, index - 1);
        if (tempPtrBehind == NULL || tempPtrBehind->nextNode == NULL)
        {
            printf("\nINDEX ERROR");
            return;
        }

        tempPtr = tempPtrBehind->nextNode;           // node to be deleted
        tempPtrBehind->nextNode = tempPtr->nextNode; // extracting next node data to the node behind
        clearNodeMem(tempPtr);
    }
}

NODE *traverseListPos(NODE *headPtr, int index)
{
    int i = 0;
    NODE *currentNode = headPtr;
    while (i < index)
    {

        if (currentNode == NULL)
            return (NULL);

        currentNode = currentNode->nextNode;
        i++;
    }

    return (currentNode);
}
