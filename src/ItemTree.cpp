#include "../hdr/ItemTree.h"
#include <iostream>
#include <cwctype>
#include <cwchar>
#include <clocale>
// initializes the root tnode
ItemTree:: ItemTree()
{
   root = new tnode;
   root->q0 = NULL;
   root->q1 = NULL;
   root->q2 = NULL;
   root->q3 = NULL;
   root->parent = NULL;
   root->tsize = 0;
   root->quadrant = L"";
}

// deallocates all tnodes
ItemTree::~ItemTree()
{
   destroy(root);
}

// delete a tnode and all its subtrees
//testing pass ptr instead of by ref &
void ItemTree::destroy(tnode* &t)
{
    if (!t) return;
      destroy(t->q0);
      destroy(t->q1);
      destroy(t->q2);
      destroy(t->q3);
      //While loop destroying itemList pointers and poping items
      //Obtained help from
      //https://stackoverflow.com/questions/307082/cleaning-up-an-stl-list-vector-of-pointers
      while(!t->itemList.empty()) delete t->itemList.front(), t->itemList.pop_front();
      // t->itemList.clear();
      delete t;
      t = NULL;
}

// insert i into the true, return true iff successful
bool ItemTree::insert(Item *i, tnode* &t)
{
   // we cannot insert null items, nor into null subtrees
   if (!i) return false;
   if (!t) return false;

   // look up the item's quadrant
   std::wstring ti, d, LL, UR, quad;
   i->lookup(ti, d, LL, UR, quad);

   // if our quadrant does not contain the item's quadrant,
   //    then the item does not belong in this tree
   if (!Address::contains(t->quadrant, quad)) return false;

   // if the item's quadrant exactly matches ours,
   //    then the item belongs in our list
   if (t->quadrant == quad) {
      t->itemList.push_back(i);
      t->tsize++;
      return true;
   }

   // otherwise the item belongs in a subtree, which we might need to create

   // lookup the character specifying which subtree we need to insert in
   bool success = false;
   switch (quad[t->quadrant.length()]) {
      case '0':
         if (!t->q0) t->q0 = create(t, '0');
         success = insert(i, t->q0);
         break;
      case '1':
         if (!t->q1) t->q1 = create(t, '1');
         success = insert(i, t->q1);
         break;
      case '2':
         if (!t->q2) t->q2 = create(t, '2');
         success = insert(i, t->q2);
         break;
      case '3':
         if (!t->q3) t->q3 = create(t, '3');
         success = insert(i, t->q3);
         break;
      default:
         // should not happen with valid addresses
         std::wcerr << "Error: invalid address being inserted, ";
         std::wcerr << quad << " into " << t->quadrant << std::endl;
         break;
   }

   // remember to adjust our tsize if we inserted anything
   if (success) t->tsize++;
   return success;
}

// create a new tnode, given the parent tnode and which child this is
ItemTree::tnode* ItemTree::create(tnode *p, wchar_t q)
{
   if (!p) return NULL;
   tnode* t = new tnode;
   if (!t) return NULL;
   t->q0 = NULL;
   t->q1 = NULL;
   t->q2 = NULL;
   t->q3 = NULL;
   t->parent = p;
   t->tsize = 0;
   t->quadrant = p->quadrant + q;
   return t;
}

// perform an inorder traversal of the specified subtree,
//     printing the contents of each item
void ItemTree::printTree(tnode *t)
{
   if (!t) return;
   std::wcout << "Start of quadrant " << t->quadrant << std::endl;
   printQuad(t);
   if (t->q0) printTree(t->q0);
   if (t->q1) printTree(t->q1);
   if (t->q2) printTree(t->q2);
   if (t->q3) printTree(t->q3);
   if (t->quadrant == L"") std::wcout << "end of tree" << std::endl << std::endl;
   else std::wcout << "end of quadrant " << t->quadrant << std::endl;
}

// find the tnode matching the specified address
ItemTree::tnode* ItemTree::find(std::wstring taddr)
{
   tnode *t = root;
   int pos = 0; // position of current character in address
   while (t && (t->quadrant != taddr)) {
      switch (taddr[pos]) {
         // identify which subtree to explore next
         case '0': t = t->q0; break;
         case '1': t = t->q1; break;
         case '2': t = t->q2; break;
         case '3': t = t->q3; break;
         default:
            // should not happen, means we have an invalid taddr
            std::wcerr << "Error: searching for invalid address " << taddr << std::endl;
            t = NULL;
            break;
      }
      pos++;
   }
   return t;
}

// find the item with the specified title and return a pointer to it,
//    if item not found in specified tnode it then searches subtrees
//    (null if not found)
Item* ItemTree::findInTree(std::wstring title, tnode *t)
{
   if (!t) return NULL;
   Item *i = findInQuad(title, t);
   if (!i) i = findInTree(title, t->q0);
   if (!i) i = findInTree(title, t->q1);
   if (!i) i = findInTree(title, t->q2);
   if (!i) i = findInTree(title, t->q3);
   return i;
}

// remove i from the tree, return true iff successful
bool ItemTree::remove(Item *i, tnode* &t)
{
   // can't remove null items or from null trees
   if (!i) return false;
   if (!t) return false;
   if (t->tsize < 1) return false;

   // see if the item is in this node
   bool success = false;
   if (findInQuad(i, t)) {
      t->itemList.remove(i);
      success = true;
   }

   // otherwise see if the item is in a subtree
   if (!success) success = remove(i, t->q0);
   if (!success) success = remove(i, t->q1);
   if (!success) success = remove(i, t->q2);
   if (!success) success = remove(i, t->q3);

   // remember to adjust size if we successfully removed
   if (success) t->tsize--;
   return success;
}

int ItemTree::size(tnode *t)
{
   if (!t) return 0;
   return t->tsize;
}


// print the contents of a specific node
void ItemTree::printQuad(tnode *t)
{
   if (!t) return;
   // iterate through the list of items, printing each
   std::list<Item*>::iterator i;

   // setting iterator to beginning of list position
   i = t->itemList.begin();
   
   while(i != t->itemList.end()){

     Item* current = (*i);

     current->print();

     i++;
   }


}

// find the item with the specified title and tnode, and return a pointer to it,
//    only searches the list for the given tnode (i.e. non-recursive)
//    (null if not found)
Item* ItemTree::findInQuad(std::wstring title, tnode *t)
{
   if (!t) return NULL;

   //iterator for iteration through list of items
   std::list<Item*>::iterator i;

   std::wstring ti,d,LL,UR,quad;
   // iterate through the list of items, checking each
   i = t->itemList.begin();
   while(i != t->itemList.end()){
      Item* cNode = (*i);
      cNode->lookup(ti,d,LL,UR,quad);

      if(ti == title){
       return cNode;
     }


     i++;
   }


   return NULL;
}

// see if the specified tnode contains the target item
bool ItemTree::findInQuad(Item *target, tnode *t)
{
   if (!t) return NULL;

   // iterate through the list of items, checking each
   //iterator for tnode object
   std::list<Item*>::iterator i;
   i = t->itemList.begin();//setting iterator to beginning of list
   //while iterator is not at the end of list
   while(i != t->itemList.end()){
     Item *current = (*i);//current item in the list
     //check if target item is in list and returns true if target matches
     if(current == target){
       return true;
     }
   }

   return false;
}

