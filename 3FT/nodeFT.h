/*--------------------------------------------------------------------*/
/* nodeFT.h                                                           */
/* Authors: Alex Delistathis and Matthew Zhang                        */
/*--------------------------------------------------------------------*/

#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include <stddef.h>
#include "a4def.h"
#include "path.h"

/* A Node_T is a node in a File Tree */
typedef struct node *Node_T;

/*
  Creates a new directory node in the File Tree, 
  with path oPPath and parent oNParent.
  Returns an int SUCCESS status and sets *poNResult
  to be the new node if successful. Otherwise, sets *poNResult to NULL
  and returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * CONFLICTING_PATH if oNParent's path is not an ancestor of oPPath
  * NO_SUCH_PATH if oPPath is of depth 0
                 or oNParent's path is not oPPath's direct parent
                 or oNParent is NULL but oPPath is not of depth 1
  * ALREADY_IN_TREE if oNParent already has a child with this path
  * INVALID_PARENT if oNParent is a file not a directory
*/
int Node_newDir(Path_T oPPath, Node_T oNParent, Node_T *poNResult);

/*
  Creates a new file node in the File Tree, with path oPPath and
  parent oNParent. Returns an int SUCCESS status and sets *poNResult
  to be the new node if successful. Otherwise, sets *poNResult to NULL
  and returns status:
  * MEMORY_ERROR if memory could not be allocated to complete request
  * CONFLICTING_PATH if oNParent's path is not an ancestor of oPPath
  * NO_SUCH_PATH if oPPath is of depth 0
                 or oNParent's path is not oPPath's direct parent
                 or oNParent is NULL but oPPath is not of depth 1
  * ALREADY_IN_TREE if oNParent already has a child with this path
  * INVALID_PARENT if oNParent is a file not a directory
  *
*/
int Node_newFile(Path_T oPPath, Node_T oNParent, Node_T *poNResult);

/*
  Destroys and frees all memory allocated for the subtree rooted at
  oNNode, i.e., deletes this node and all its descendants. Returns the
  number of nodes deleted.
*/
size_t Node_free(Node_T oNNode);

/* Returns the path object representing oNNode's absolute path. */
Path_T Node_getPath(Node_T oNNode);

/*
  Returns TRUE if oNParent has a child with path oPPath. Returns
  FALSE if it does not or if there is an error while checking.

  If oNParent has such a child, stores in *pulChildID the child's
  identifier (as used in Node_getChild). If oNParent does not have
  such a child, stores in *pulChildID the identifier that such a
  child would have if inserted.
*/
boolean Node_hasChild(Node_T oNParent, Path_T oPPath,
                         size_t *pulChildID);

/* Returns the number of children that oNParent has.
   Returns 0 if oNParent is a file node.
*/
size_t Node_getNumChildren(Node_T oNParent);

/*
  Returns INVALID_PARENT status if parent is a file node.
  Otherwise, returns an int SUCCESS status 
  and sets *poNResult to be the child
  node of oNParent with identifier ulChildID, if one exists.
  Otherwise, sets *poNResult to NULL and returns status:
  * NO_SUCH_PATH if ulChildID is not a valid child for oNParent 
*/
int Node_getChild(Node_T oNParent, size_t ulChildID,
                  Node_T *poNResult);

/*
  Returns the parent node of oNNode.
  Returns NULL if oNNode is the root and thus has no parent.
*/
Node_T Node_getParent(Node_T oNNode);

/*
  Compares file nodes 
  oNFirst and oNSecond lexicographically based on their paths.
  Returns <0, 0, or >0 if onFirst is "less than", "equal to", or
  "greater than" oNSecond, respectively. Returns status:
  * NOT_A_FILE if oNFirst or oNSecond is not a file node
*/
int Node_compareFiles(Node_T oNFirst, Node_T oNSecond);

/*
  Compares oNFirst and oNSecond lexicographically based on their paths.
  Returns <0, 0, or >0 if onFirst is "less than", "equal to", or
  "greater than" oNSecond, respectively. Returns status:
  * NOT_A_DIRECTORY if oNFirst or oNSecond is not a directory node
*/
int Node_compareDirs(Node_T oNFirst, Node_T oNSecond);

/*
  Returns TRUE if oNNode is a file node, and FALSE otherwise.
*/
boolean Node_isFile(Node_T oNNode);

/*
  Returns TRUE if oNNode is a directory node, and FALSE otherwise.
*/
boolean Node_isDir(Node_T oNNode);

/*
  Returns a string representation for oNNode, or NULL if
  there is an allocation error.

  Allocates memory for the returned string, which is then owned by
  the caller!
*/
char *Node_toString(Node_T oNNode);

#endif
