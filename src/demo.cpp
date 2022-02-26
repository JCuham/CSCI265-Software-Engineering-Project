#include "../hdr/demo.h"
#include <iostream>
#include <clocale>
#include <cwchar>
#include <cwctype>
/*
Demo.cpp for csci265, Created by Dave Wessels for use by
James Cunningham
*/
/*
 This program is designed to create and manage a quadtree of data 
 */
int main()
{

  //Need to initialize locale for unicode to display and work properly
  //creates a warning because im not using the locale variable anywhere
  //except to initialize UTF8
   char *locale;
   locale = setlocale(LC_ALL, "");

   std::wcout<<"Using: "<<locale<<std::endl;
  
   bool verbose = true;
   Command cmd;
   ItemTree *iTree = NULL;
   helpCmd();
   do {
      cmd = getCommand(verbose);
      processCommand(iTree, cmd, verbose);
   } while (cmd != Quit);
   
   return 0;
}

// obtain the next command from the user (silent mode if verbose is false)
Command getCommand(bool verbose)
{
   wchar_t ch; // user-entered command character
   Command cmd; // translated command (None if invalid command entered)
   //variable to hold smiley face
   wchar_t face = 0x263A;

   //outputs enter command prompt if user is in verbose mode
   if (verbose)
     std::wcout << L"Enter a command (QHNDPFVIR: H is help) "<< face << face << face;
   std::wcout << "> ";
   std::wcin >> ch;
   ch = towupper(ch);
   //switch statement for user input options
   switch (ch) {
      case Quit:
      case Help:
      case Print:
      case New:
      case Delete:
      case Find:
      case Verbose:
      case Insert:
      case Remove:
         // valid command entered
         cmd = (Command)(ch);
         break;

      default:
         // anything else is invalid
         cmd = None;
	 //Adding output for invalid command even if quiet mode is active
	 if(!verbose){
	   std::wcout<<"Enter a command (QHNDPFVIR: H is help)"<<std::endl;
	 }
   }
   return cmd;
}

// let the user specify whether they wish to work on a single quadrant or an entire subtree
Modifier getModifier(bool verbose)
{
   wchar_t ch; // user-entered command character
   Modifier cmd; // translated modifier (Invalid unless valid choise made)
   if (verbose)
     std::wcout << "Single quadrant (Q) or entire subtree (T)?";
   std::wcout << "> ";
   std::wcin >> ch;
   ch = towupper(ch);
   switch (ch) {
      case Tree:
      case Quad:
         // valid command entered
         cmd = (Modifier)(ch);
         break;

      default:
         // anything else is invalid
         cmd = Invalid;
   }
   return cmd;
}

// let the user specify a quaternary address
std::wstring getAddress(bool verbose)
{
   if (verbose)
     std::wcout << "Enter the quaternary address or * for global" << std::endl;
   
   std::wstring addr = L"";
   std::wcin >> addr;
   
   if (addr == L"*")
     addr = L"";
   
   if (!Address::isValidAddr(addr) && verbose) {
      std::wcout << "WARNING: " << addr << " is not a valid address" << std::endl;
      std::wcout << "(either just *, or 1-" << Address::MaxAddr << " digits 0-3)" << std::endl;
   }
   return addr;
}

void helpCmd()
{
   std::wcout << std::endl;
   std::wcout << "Welcome to the ItemTree demo" << std::endl;
   std::wcout << "Enter any one of the following commands:" << std::endl;
   std::wcout << "   " << (wchar_t)(Quit) << " to quit" << std::endl;
   std::wcout << "   " << (wchar_t)(Help) << " to display this menu" << std::endl;
   std::wcout << "   " <<
     (wchar_t)(Verbose) << " to toggle verbose mode (prompts/messages)" << std::endl;
   std::wcout << "   " << (wchar_t)(New) << " to create a new tree (deletes old)" << std::endl;
   std::wcout << "   " << (wchar_t)(Delete) << " to delete the current tree" << std::endl;
   std::wcout << "   " << (wchar_t)(Print) << " to print items from the tree" << std::endl;
   std::wcout << "   " << (wchar_t)(Insert) << " to insert new elements in the tree" << std::endl;
   std::wcout << "   " << (wchar_t)(Remove) << " to remove elements from the tree" << std::endl;
   std::wcout << "   " << (wchar_t)(Find) << " to find elements in the tree" << std::endl;
   std::wcout << std::endl;
}

// display the invalid command message (silent mode if verbose is false)
void errorCmd(bool verbose)
{
   if (verbose) {
      std::wcout << L"Error: an invalid command was entered, please try again" << std::endl;
      std::wcout << L"       (H is the command for the help menu)" << std::endl;
   }
}

// obtain any required arguments then apply the command to the tree
// (silent mode if verbose is true)
void processCommand(ItemTree* &IT, Command cmd, bool &verbose)
{
   switch (cmd) {
      case Quit: quitCmd(IT, verbose);
         break;
      case Help: helpCmd();
         break;
      case Verbose: verbose = !verbose;
         break;
      case Print: printCmd(IT, verbose);
         break;
      case Insert: insertCmd(IT, verbose);
         break;
      case Remove: removeCmd(IT, verbose);
         break;
      case Find: findCmd(IT, verbose);
         break;
      case New: newCmd(IT, verbose);
         break;
      case Delete: deleteCmd(IT, verbose);
         break;
      default: errorCmd(verbose);
         break;
   }
}

// terminate the program (silent mode if verbose is false)
void quitCmd(ItemTree* &IT, bool verbose)
{
   if (verbose) {
      std::wcout << L"Deallocating item tree and shutting down" << std::endl;
   }
   deleteCmd(IT, verbose);
   std::wcout << std::endl;
}

// delete and nullify current tree
// (silent mode if verbose is true)
void deleteCmd(ItemTree* &IT, bool verbose)
{
   if (IT) {
      delete IT;
      IT = NULL;
   } else if (verbose) {
      std::wcout << L"Warning: no item tree present to delete" << std::endl;
   }
}

// create a new item tree (deletes and replaces old)
// (silent mode if verbose is true)
void newCmd(ItemTree* &IT, bool verbose)
{
   if (IT)
     deleteCmd(IT, verbose);
   IT = new ItemTree;
}

// obtain any required arguments then print the requested tree components
// (silent mode if verbose is true)
void printCmd(ItemTree* IT, bool verbose)
{
   if (!IT) {
      if (verbose) std::wcout << L"WARNING: no tree has been created to print" << std::endl;
      return;
   }

   // see if the user wishes to print a single quadrant,
   //     or an entire subtree
   Modifier m;
   do {
      m = getModifier(verbose);
      if (m == Invalid) {
         std::wcout << L"Error: neither quadrant nor tree was selected, ";
         std::wcout << L"please try again" << std::endl;
      }
   } while (m == Invalid);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   // print the appropriate component
   if (m == Tree) {
      if (verbose)
         std::wcout << std::endl << L"Printing tree from \"" << addr << L"\"" << std::endl;
      IT->printTree(addr);
   } else if (m == Quad) {
      if (verbose)
         std::wcout << std::endl << L"Printing quad from \"" << addr << L"\"" << std::endl;
      IT->printQuad(addr);
   }
}

// prompt the user (if verbose) and read a line of text into the target
void getString(std::wstring prompt, std::wstring &target, bool verbose)
{
    if (verbose)
      std::wcout << prompt;
    
    std::getline(std::wcin, target);
}

// obtain any required arguments then inserted specified data into tree
// (silent mode if verbose is true)
void insertCmd(ItemTree* &IT, bool verbose)
{
    if (!IT) {
       std::wcout << L"WARNING: no tree exists to insert to" << std::endl;
       return;
    }
    std::wstring junk, title, desc, LL, UR;
    std::getline(std::wcin, junk); // discard leftover newline from buffer
    getString(L"Enter a single line of text for the new item title: >", title, verbose);
    getString(L"Enter a single line of text for the new item description: >", desc, verbose);
    if (verbose)
      std::wcout << L"Next provide the address of the item\'s lower left corner";
    
    LL = getAddress(verbose);
    if (verbose)
      std::wcout << L"Next provide the address of the item\'s upper right corner";
    
    UR = getAddress(verbose);
    Item *i = new Item(title, desc, LL, UR);
    if (!i) {
       std::wcout << "WARNING: item creation failed" << std::endl;
    } else {
       std::wstring t, d, l, u, q;
       i->lookup(t, d, l, u, q); // lookup to get the quadrant
       if (!IT->insert(i)) {
          if (verbose)
	    std::wcout << "WARNING: item insert failed" << std::endl;
	  delete i;
       } else{
       
	 std::wcout << "Item inserted in quadrant " << q << std::endl;
       }
    }

    
}

// obtain any required arguments then search the requested tree components,
//     returning a pointer to the relevant item
// (null if not found, silent mode if verbose is true)
Item* findCmd(ItemTree* IT, bool verbose)
{
   if (!IT) {
      if (verbose)
	std::wcout << L"WARNING: no tree has been created to search" << std::endl;
      return NULL;
   }
   // see if the user wishes to print a search quadrant,
   //     or an entire subtree
   Modifier m;
   do {
      m = getModifier(verbose);
      if (m == Invalid) {
         std::wcout << L"Error: neither quadrant nor tree was selected, ";
         std::wcout << L"please try again" << std::endl;
      }
   } while (m == Invalid);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   std::wstring junk, title, desc, LL, UR, quad;
   std::getline(std::wcin, junk); // discard leftover newline from buffer
   getString(L"Enter a single line of text for the title you are searching for: >", title, verbose);

   Item* iptr = NULL;
   if (m == Quad)
     iptr = IT->findInQuad(title, addr);
   
   else iptr = IT->findInTree(title, addr);

   if (!iptr) {
      std::wcout << L"No matching item found for \"" << title << "\" in ";
      std::wcout << addr << std::endl;
   } else {
      std::wcout << L"Found: ";
      iptr->print();
   }
   return iptr;
}

// obtain any required arguments then remove the requested tree components
// (silent mode if verbose is true)
void removeCmd(ItemTree* &IT, bool verbose)
{
   if (!IT) {
      if (verbose)
	std::wcout << L"WARNING: no tree has been created yet" << std::endl;
      return;
   }
   // see if the user wishes to print a search quadrant,
   //     or an entire subtree
   Modifier m;
   do {
      m = getModifier(verbose);
      if (m == Invalid) {
         std::wcout << L"Error: neither quadrant nor tree was selected, ";
         std::wcout << L"please try again" << std::endl;
      }
   } while (m == Invalid);

   std::wstring junk, title, desc, LL, UR, quad;
   std::getline(std::wcin, junk); // discard leftover newline from buffer
   getString(L"Enter a single line of text for the title of the item you wish removed >", title, verbose);

   // get the address of the quadrant or subtree
   std::wstring addr = getAddress(verbose);

   Item* iptr = NULL;
   if (m == Quad) iptr = IT->findInQuad(title, addr);
   else iptr = IT->findInTree(title, addr);

   if (!iptr) {
      std::wcout << L"No matching item found for \"" << title << "\" in ";
      std::wcout << addr << std::endl;
   } else {
       // ************ TRY THE REMOVE HERE ****************
       if (IT->remove(iptr)) {
           std::wcout << L"Removed item \"" << title << "\"" << std::endl;
       } else {
           std::wcout << L"Remove unsuccessful for item \"" << title << "\"" << std::endl;
       }
   }
}

