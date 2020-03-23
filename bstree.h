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
 ** File: 4004-as/src/bstree.h
 **
 ** Description:
 ** Header file that goes with bstree.c
 ********************************************************************************
 ** Version 1.0.0
 ********************************************************************************
 ** DJB 10/13/18 Created.
 ********************************************************************************
 */

#ifndef ___BSTREE_H___
#define ___BSTREE_H___

//------------------------------------------------------------------------------
//Defines
//None

//------------------------------------------------------------------------------
//Enumerations
//None

//------------------------------------------------------------------------------
//Structures
typedef struct tagBSTreeNode
{
    void* m_vpKey;
    void* m_vpDataElement;
    struct tagBSTreeNode* m_pLeft;
    struct tagBSTreeNode* m_pRight;
    struct tagBSTreeNode* m_pParent;
} BSTreeNode;

//------------------------------------------------------------------------------
//Function Pointers
//Binary search tree key compare function pointer.
//vpKey1 - Pointer to the first key to compare.
//vpKey2 - Pointer to the second key to compare.
//Returns:  An integer less than, equal to, or greater than zero if the first
//          key is found, respectively, to be less than, to match, or be greater
//          than the second key.
typedef int (*BSTreeKeyCompareFunction)(const void* vpKey1, const void* vpKey2);

//Binary search tree walk current node function pointer.  Use this function to
//print out, analyze, or otherwise use the data of the current node in the walk.
//pCurrentNode - Pointer to the current node of the walk.
//Returns:  None.
typedef void (*BSTreeWalkCurrentNodeFunction)(BSTreeNode* pCurrentNode);

//------------------------------------------------------------------------------
//Prototypes
BSTreeNode* bstree_insert(BSTreeNode** ppTreeRoot, void* vpKey, size_t iKeySize, BSTreeKeyCompareFunction fpBSTreeKeyCompareFunction);
BSTreeNode* bstree_search(BSTreeNode* pNode, void* vpKey, BSTreeKeyCompareFunction fpBSTreeKeyCompareFunction);
void bstree_delete(BSTreeNode** ppTreeRoot, BSTreeNode* pNodeToDelete);
void bstree_in_order_walk(BSTreeNode* pNode, BSTreeWalkCurrentNodeFunction fpBSTreeWalkCurrentNodeFunction);
int bstree_key_compare(const void* vpKey1, const void* vpKey2);

#endif /*___BSTREE_H___*/
