/*--------------------------------------------------------------------*/
/* checkerDT.c                                                        */
/* Author:                                                            */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerDT.h"
#include "dynarray.h"
#include "path.h"



/* see checkerDT.h for specification */
boolean CheckerDT_Node_isValid(Node_T oNNode) {
   Node_T oNParent;
   Path_T oPNPath;
   Path_T oPPPath;
   size_t ulIndex;
   size_t numChildren;
   Node_T oNChild;
   Path_T oPPath1;
   Path_T oPPath2;
   
   /* Sample check: a NULL pointer is not a valid node */
   if(oNNode == NULL) {
      fprintf(stderr, "A node is a NULL pointer\n");
      return FALSE;
   }

   /* Sample check: parent's path must be the longest possible
      proper prefix of the node's path */
   oNParent = Node_getParent(oNNode);
   if(oNParent != NULL) {
      oPNPath = Node_getPath(oNNode);
      oPPPath = Node_getPath(oNParent);

      if(Path_getSharedPrefixDepth(oPNPath, oPPPath) !=
         Path_getDepth(oPNPath) - 1) {
         fprintf(stderr, "P-C nodes don't have P-C paths: (%s) (%s)\n",
                 Path_getPathname(oPPPath), Path_getPathname(oPNPath));
         return FALSE;
      }
   }

   /* Check: Same node must not appear more than once in the same
      subtree. */
   if (oNParent != NULL)
   {
      numChildren = Node_getNumChildren(oNParent);
      oPPath1 = Node_getPath(oNNode);
      for (ulIndex = 0; ulIndex < numChildren; ulIndex++)
      {
         int iStatus = Node_getChild(oNParent, ulIndex, &oNChild);
         
         if (iStatus == SUCCESS)
         {
            oPPath2 = Node_getPath(oNChild);
            if (Path_comparePath(oPPath1, oPPath2) == 0 && oPPath1
                != oPPath2)
            {
               fprintf(stderr, "Same node appears more than once in tree: (%s)\n", Node_toString(oNNode));
               return FALSE;
            }
         }
      }
   }
   
   return TRUE;
}

/*
   Performs a pre-order traversal of the tree rooted at oNNode.
   Returns FALSE if a broken invariant is found and
   returns TRUE otherwise.
*/
static boolean CheckerDT_treeCheck(Node_T oNNode) {
   size_t ulIndex1;
   size_t ulIndex2;
   Path_T oPPath1;
   Path_T oPPath2;
   
   if(oNNode!= NULL) {

      /* Sample check on each node: node must be valid */
      /* If not, pass that failure back up immediately */
      if(!CheckerDT_Node_isValid(oNNode))
         return FALSE;

      /* Recur on every child of oNNode */
      for(ulIndex1 = 0; ulIndex1 < Node_getNumChildren(oNNode); ulIndex1++)
      {
         Node_T oNChild1 = NULL;
         Node_T oNChild2 = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex1, &oNChild1);

         if(iStatus != SUCCESS) {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            return FALSE;
         }

         for (ulIndex2 = ulIndex1 + 1; ulIndex2 < Node_getNumChildren(oNNode); ulIndex2++)
         {
            iStatus = Node_getChild(oNNode, ulIndex2, &oNChild2);
            if (iStatus != SUCCESS)
            {
               fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
               return FALSE;
            }

            oPPath1 = Node_getPath(oNChild1);
            oPPath2 = Node_getPath(oNChild2);

            if (Path_comparePath(oPPath1, oPPath2) > 0)
            {
               fprintf(stderr, "Children are not in lexicographic order: (%s) (%s)\n",
                       Path_getPathname(oPPath1),
                       Path_getPathname(oPPath2));
               return FALSE;
            }
         }
         
         
         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         if(!CheckerDT_treeCheck(oNChild1))
            return FALSE;
      }
   }
   return TRUE;
}

/* see checkerDT.h for specification */
boolean CheckerDT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount) {

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0. */
   if(!bIsInitialized)
      if(ulCount != 0) {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }

   /* Now checks invariants recursively at each node from the root. */
   return CheckerDT_treeCheck(oNRoot);
}
