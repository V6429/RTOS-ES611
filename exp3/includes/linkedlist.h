/**
 * @file functions.h
 * @author Vivek 
 * @brief Provides a node typedef and function prototypes for linked list implementation.
 * @version 0.1
 * @date 2022-01-03
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef FUNC
#define FUNC
/**
 * @brief Node element for linked list.
 * Modify this as per requirement.
 * 
 */
typedef struct sensor
{
    int sensor_number;
    char sensor_type;
    char sensor_lastvalue;
    char sensor_port;
    char sensor_pin;
    struct node *nextNode;

} NODE;

/**
 * @brief Insert a node to the beginning of the list
 * 
 * @param headPtr pointer to the HEAD of the list
 * @param newNODE pointer to the node to be added
 * @return NODE*  pointer to the HEAD 
 */
NODE *insertListBeg(NODE *headPtr, NODE *newNODE);

/**
 * @brief Insert a node to the end of the list
 * @param headPtr pointer to the HEAD of the list
 * @param newNODE pointer to the node to be added
 * @return NODE*  pointer to the HEAD 
 */
NODE *insertListEND(NODE *headPtr, NODE *newNODE);

/**
 * @brief Insert a node at a particular position.
 * @param headPtr pointer to the HEAD of the list
 * @param newNODE  pointer to the node to be added
 * @param index the index positon 
 * @return NODE* pointer to the HEAD  
 */
NODE *insertListPos(NODE *headPtr, NODE *newNODE, int index);


/**
 * @brief Delete node at the beginning of the LIST
 * @param headPtr pointer to the HEAD of the list
 * @return NODE* pointer to the HEAD 
 */
NODE *deleteListBeg(NODE *headPtr);
/**
 * @brief Delete the npde at the end of list
 * 
 * @param headPtr  pointer to the HEAD of the list
 */
NODE *deleteListEND(NODE *headPtr);

/**
 * @brief Delete the node at particular position
 * @param headPtr pointer to the HEAD of the list
 * @param index the index positon 
 */
void deleteListPos(NODE *headPtr, int index);

/**
 * @brief Function to move through the List and reach the index position
 * 
 * @param headPtr pointer to the HEAD of the list
 * @param index the index positon 
 * @return NODE* Returns the adrress of the Postion
 */
NODE *traverseListPos(NODE *const headPtr, int index);
/**
 * @brief Create a Node object
 * 
 * @param number data part
 * @return NODE* pointer
 */
NODE *createNode(int number);

/**
 * @brief Prints out the entire List
 * 
 * @param headPtr pointer to the HEAD of the list
 */
void printList(NODE *const headPtr);

/**
 * @brief Print the Node to display
 * 
 * @param nodePtr address of the Node
 */
void printNode(NODE *const nodePtr);
#endif