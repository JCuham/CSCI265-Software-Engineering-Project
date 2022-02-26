#ifndef ITEMTREE_H
#define ITEMTREE_H

#include "Address.h"
#include "Item.h"

#include <list>
#include <cwctype>
#include <cwchar>

// ItemTree is a quadtree of Items,
//    in which each tree node represents a specific quadrant,
//       with a list of Items directly associated with that quadrant
//       and subtrees for each of its four subquadrants
//
// The root of the item tree is associated with the global quadrant,
//     i.e. with the null address string,
// its four subtrees would be associated with quadrants "0", "1", "2", "3",
// quadrant 0's subtrees would be associated with quadrants 00, 01, 10, 11,
// etc
//
// Tree methods support Item insertion and removal, printing the contents
//    of a node or subtree (each specified by their quadrant/address string),
//    and finding a specific Item (by title) within a node or subtree
//
// Tree nodes are created during the insertion process when either an Item
//    is inserted into that specific node or into one of its subtrees,
// and tree nodes are deleted during the node removal process when there
//    are no further Items associated with the node and all of its subtrees
//    are empty
//
class ItemTree {
   protected:
      // the ItemTree is a quadtree of tnodes
      struct tnode {
         std::wstring quadrant;      // the address string for this quadtree
         tnode *q0, *q1, *q2, *q3;  // subtrees for each of its four quadrants
         tnode *parent;             // containing (parent) quadrant of current
         std::list<Item*> itemList; // the items associated with this tnode's quadrant
                                    //     (not including subtrees)
         long tsize;                // the total number of items in this quadtree
                                    //     (including its subtrees)
      } *root;

   public:
      ItemTree();  // initializes the root tnode
     ~ItemTree();  // deallocates all tnodes

     // lookup the tree size
     int size() { return size(root); }

     // insert i into the tree, return true iff successful
     bool insert(Item *i) { return insert(i, root); }

     // remove i from the tree, return true iff successful
     bool remove(Item *i) { return remove(i, root); }

     // find the item with the specified title and address, and return a pointer to it
     //    (null if not found, searchable tnode defaults to root of tree)
     // findInQuad only looks in the list for the specified quadrant,
     // findInTree looks in the list for the specified quadrant first,
     //    and if not found it then searches the subtrees
     Item* findInQuad(std::wstring title, std::wstring addr)
           { return findInQuad(title, find(addr)); }
     Item* findInTree(std::wstring title, std::wstring addr)
           { return findInTree(title, find(addr)); }

     // perform an inorder traversal of the specified subtree,
     //     printing the contents of each item
     void printTree(std::wstring taddr)
          { printTree(find(taddr)); }

     // print the contents of a specific quadrant
     void printQuad(std::wstring taddr)
          { printQuad(find(taddr)); }

   protected:
     // tnode-based recursive variants of the public methods of the same name
     bool insert(Item *i, tnode* &t);
     bool remove(Item *i, tnode* &t);
     Item* findInQuad(std::wstring title, tnode *t);
     Item* findInTree(std::wstring title, tnode *t);
     void printTree(tnode *t);
     void printQuad(tnode *t);
     int size(tnode *t);

     // see if a specific tnode contains a specific item
     bool findInQuad(Item* target, tnode *t);

     // delete a tnode and all its subtrees
     void destroy(tnode* &t);

     // create a new tnode, given the parent tnode and which child this is
     tnode* create(tnode *p, wchar_t q);

     // find a tnode, given its address string
     tnode* find(std::wstring addr);
};

#endif
