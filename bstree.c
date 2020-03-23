/*
 ********************************************************************************
 ** Copyright (C) 2018 Donald J. Bartley <djbcoffee@gmail.com>
 **
 ** This source file may be used and distributed without restriction provided
 ** that this copyright statement is not removed from the file and that any
 ** derivative work contains the original copyright notice and the associated
 ** disclaimer.
 **
 ** This source file is free software; you can redistribute it and/or modify it
 ** under the terms of the GNU General Public License as published by the Free
 ** Software Foundation; either version 2 of the License, or (at your option) any
 ** later version.
 **
 ** This source file is distributed in the hope that it will be useful, but
 ** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 ** FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 ** more details.
 **
 ** You should have received a copy of the GNU General Public License along with
 ** this source file.  If not, see <http://www.gnu.org/licenses/> or write to the
 ** Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 ** 02110-1301, USA.
 ********************************************************************************
 ** File: 4004-as/src/bstree.c
 **
 ** Description:
 ** This translation (compilation) unit contains the functions for building,
 ** maintaining, and deleting a binary search tree.
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

//System #includes
#ifndef _WINDOWS_H
#include <windows.h>
#endif
#ifndef _STDINT_H
#include <stdint.h>
#endif
#ifndef _STDLIB_H_
#include <stdlib.h>
#endif
#ifndef _STRING_H
#include <string.h>
#endif

//Project-wide #includes
#ifndef ___UNIVERSAL_H___
#include "universal.h"
#endif

//Project #includes
#ifndef ___LOG_H___
#include "log.h"
#endif

//Reflective #includes
#ifndef ___BSTREE_H___
#include "bstree.h"
#endif

//------------------------------------------------------------------------------
//Global Data
//None

//------------------------------------------------------------------------------
//Static Data
//None

//------------------------------------------------------------------------------
//Static Prototypes
static void bstree_transplant(BSTreeNode** root, BSTreeNode* pSubtreeOne, BSTreeNode* pSubtreeTwo);
static BSTreeNode* bstree_minimum(BSTreeNode* pNode);

//==============================================================================
//Functions
/*------------------------------------------------------------------------------
 * Function name:  bstree_insert
 * Function Description:  Inserts a node into the binary search tree.
 * Parameters:
 * ppTreeRoot - A pointer to a pointer that is the root node of the binary tree.
 * vpKey - A pointer to the key to be stored in the node.
 * iKeySize - The length of the key in bytes.
 * fpBSTreeKeyCompareFunction - A pointer to the compare function to use when
 *                              comparing keys.
 * Returns:  If the new node is created and inserted into the binary search tree
 *           a pointer to that node is returned, otherwise NULL is returned.
------------------------------------------------------------------------------*/
BSTreeNode* bstree_insert(BSTreeNode** ppTreeRoot, void* vpKey, size_t iKeySize, BSTreeKeyCompareFunction fpBSTreeKeyCompareFunction)
{
    BSTreeNode* pCurrentNode;
    BSTreeNode* pCurrentParentNode;

    //Initialize variables.
    pCurrentNode = *ppTreeRoot;
    pCurrentParentNode = NULL;

    //Loop until an available node is located.
    while(pCurrentNode != NULL)
    {
        //Node not available.  Make it the parent node and select the next node
        //based on a comparison of the passed symbol and the current node symbol.
        pCurrentParentNode = pCurrentNode;
        pCurrentNode = (fpBSTreeKeyCompareFunction(vpKey, pCurrentNode->m_vpKey) < 0) ? pCurrentNode->m_pLeft : pCurrentNode->m_pRight;
    }

    //Available node found.  Allocate memory for the node.
    pCurrentNode = (BSTreeNode*)malloc(sizeof(BSTreeNode));
    if(pCurrentNode == NULL)
        return NULL;

    //Memory allocated for the node, now allocate memory for the key.
    pCurrentNode->m_vpKey = malloc(iKeySize);
    if(pCurrentNode->m_vpKey == NULL)
        return NULL;

    //Memory allocated for the key.  Setup the initial values for the node.
    memcpy(pCurrentNode->m_vpKey, vpKey, iKeySize);
    pCurrentNode->m_vpDataElement = NULL;
    pCurrentNode->m_pParent = pCurrentParentNode;
    pCurrentNode->m_pLeft = NULL;
    pCurrentNode->m_pRight = NULL;
    if(pCurrentParentNode == NULL)
        *ppTreeRoot = pCurrentNode;
    else if(fpBSTreeKeyCompareFunction(vpKey, pCurrentParentNode->m_vpKey) < 0)
        pCurrentParentNode->m_pLeft = pCurrentNode;
    else
        pCurrentParentNode->m_pRight = pCurrentNode;

    return pCurrentNode;
}

/*------------------------------------------------------------------------------
 * Function name:  bstree_search
 * Function Description:  Searches the binary search tree for a specific symbol.
 * Parameters:
 * pNode - A pointer to the node of the binary tree from which to start the
 *         search.
 * vpKey - A pointer to the key for which to search.
 * fpBSTreeKeyCompareFunction - A pointer to the compare function to use when
 *                              comparing keys.
 * Returns:  If the specific symbol is found a pointer to that node is returned,
 *           otherwise NULL is returned.
------------------------------------------------------------------------------*/
BSTreeNode* bstree_search(BSTreeNode* pNode, void* vpKey, BSTreeKeyCompareFunction fpBSTreeKeyCompareFunction)
{
    while(pNode != NULL && fpBSTreeKeyCompareFunction(vpKey, pNode->m_vpKey) != 0)
        pNode = (fpBSTreeKeyCompareFunction(vpKey, pNode->m_vpKey) < 0) ? pNode->m_pLeft : pNode->m_pRight;

    return pNode;
}

/*------------------------------------------------------------------------------
 * Function name:  bstree_delete
 * Function Description:  Deletes a node from the binary search tree.  This frees
 *                        all allocated memory including m_vpDataElement.
 * Parameters:
 * ppTreeRoot - A pointer to a pointer that is the root node of the binary tree.
 * pNodeToDelete - A pointer to the node to be deleted.
 * Returns:  None.
------------------------------------------------------------------------------*/
void bstree_delete(BSTreeNode** ppTreeRoot, BSTreeNode* pNodeToDelete)
{
    BSTreeNode* pMinimumNode;

    if(pNodeToDelete->m_pLeft == NULL)
    {
        bstree_transplant(ppTreeRoot, pNodeToDelete, pNodeToDelete->m_pRight);
    }
    else if(pNodeToDelete->m_pRight == NULL)
    {
        bstree_transplant(ppTreeRoot, pNodeToDelete, pNodeToDelete->m_pLeft);
    }
    else
    {
        pMinimumNode = bstree_minimum(pNodeToDelete->m_pRight);
        if(pMinimumNode->m_pParent != pNodeToDelete)
        {
            bstree_transplant(ppTreeRoot, pMinimumNode, pMinimumNode->m_pRight);
            pMinimumNode->m_pRight = pNodeToDelete->m_pRight;
            pMinimumNode->m_pRight->m_pParent = pMinimumNode;
        }
        bstree_transplant(ppTreeRoot, pNodeToDelete, pMinimumNode);
        pMinimumNode->m_pLeft = pNodeToDelete->m_pLeft;
        pMinimumNode->m_pLeft->m_pParent = pMinimumNode;
    }

    if(pNodeToDelete->m_vpDataElement != NULL)
        free(pNodeToDelete->m_vpDataElement);
    free(pNodeToDelete->m_vpKey);
    free(pNodeToDelete);
}

/*------------------------------------------------------------------------------
 * Function name:  bstree_in_order_walk
 * Function Description:  Walks the binary search tree in ascending order.  This
 *                        a recursive function and does not return to the caller
 *                        until the entire walk is done.
 * Parameters:
 * pNode - A pointer to a node from which to start the walk.'
 * fpBSTreeWalkCurrentNodeFunction - Pointer to the function to call when the
 *                                   walk reaches a point where it has the next
 *                                   node in ascending order ready.
 * Returns:  None.
------------------------------------------------------------------------------*/
void bstree_in_order_walk(BSTreeNode* pNode, BSTreeWalkCurrentNodeFunction fpBSTreeWalkCurrentNodeFunction)
{
    if(pNode != NULL)
    {
        bstree_in_order_walk(pNode->m_pLeft, fpBSTreeWalkCurrentNodeFunction);
        if(fpBSTreeWalkCurrentNodeFunction != NULL)
            fpBSTreeWalkCurrentNodeFunction(pNode);
        bstree_in_order_walk(pNode->m_pRight, fpBSTreeWalkCurrentNodeFunction);
    }
}

/*------------------------------------------------------------------------------
 * Function name:  symbol_tree_key_compare
 * Function Description:  The compare function used for comparing keys in the
 *                        symbol tree.
 * Parameters:
 * vpKey1 - Pointer to the first key to compare.
 * vpKey2 - Pointer to the second key to compare.
 * Returns:  An integer less than, equal to, or greater than zero if the first
 *           key is found, respectively, to be less than, to match, or be greater
 *           than the second key.
------------------------------------------------------------------------------*/
int bstree_key_compare(const void* vpKey1, const void* vpKey2)
{
    return strcmp((const char*)vpKey1, (const char*)vpKey2);
}

/*------------------------------------------------------------------------------
 * Function name:  bstree_transplant
 * Function Description:  Replaces ones subtree as a child of its parent with
 *                        another subtree.
 * Parameters:
 * ppTreeRoot - A pointer to a pointer that is the root node of the binary tree.
 * pSubtreeOne - A pointer to the subtree that is to be replaced.
 * pSubtreeTwo - A pointer to the subtree that is the replacement.
 * Returns:  None.
------------------------------------------------------------------------------*/
static void bstree_transplant(BSTreeNode** ppTreeRoot, BSTreeNode* pSubtreeOne, BSTreeNode* pSubtreeTwo)
{
    if(pSubtreeOne->m_pParent == NULL)
        *ppTreeRoot = pSubtreeTwo;
    else if(pSubtreeOne == pSubtreeOne->m_pParent->m_pLeft)
        pSubtreeOne->m_pParent->m_pLeft = pSubtreeTwo;
    else
        pSubtreeOne->m_pParent->m_pRight = pSubtreeTwo;

    if(pSubtreeTwo != NULL)
        pSubtreeTwo->m_pParent = pSubtreeOne->m_pParent;
}

/*------------------------------------------------------------------------------
 * Function name:  bstree_minimum
 * Function Description:  Finds the minimum node of a binary search tree.
 * Parameters:
 * pNode - A pointer to the node at which to start searching for the minimum.
 * Returns:  Returns the node containing the minimum.
------------------------------------------------------------------------------*/
static BSTreeNode* bstree_minimum(BSTreeNode* pNode)
{
    while(pNode->m_pLeft != NULL)
        pNode = pNode->m_pLeft;

    return pNode;
}
