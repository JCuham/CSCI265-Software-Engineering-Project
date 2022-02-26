#ifndef DEMO_H
#define DEMO_H

#include "Item.h"
#include "Address.h"
#include "ItemTree.h"
//added for unicode support
#include <cwchar>
#include <cwctype>

// valid command characters
enum Command {   None='n', Error='E', Quit='Q', Help='H', Print='P', New='N',
                 Delete='D', Find='F', Verbose='V', Insert='I', Remove='R' };

// valid command modifiers
enum Modifier { Invalid='I', Quad='Q', Tree='T' };

// obtain the next command from the user (silent mode if verbose is false)
Command getCommand(bool verbose=true);

// let the user specify whether they want to work on a single quadrant or an entire subtree
Modifier getModifier(bool verbose=true);

// let the user specify a quaternary address
std::wstring getAddress(bool verbose=true);

// prompt the user (if verbose) and read a line of text into the target
void getString(std::wstring prompt, std::wstring &target, bool verbose=true);

// obtain any required arguments then apply the command to the tree
// (silent mode if verbose is true)
void processCommand(ItemTree* &IT, Command cmd, bool &verbose);

// display the invalid command message (silent mode if verbose is false)
void errorCmd(bool verbose=true);

// display the help menu
void helpCmd();

// terminate the program (silent mode if verbose is false)
void quitCmd(ItemTree* &IT, bool verbose=true);

// obtain any required arguments then print the requested tree components
// (silent mode if verbose is true)
void printCmd(ItemTree* IT, bool verbose=true);

// obtain any required arguments then inserted specified data into tree
// (silent mode if verbose is true)
void insertCmd(ItemTree* &IT, bool verbose=true);

// obtain any required arguments then remove the requested tree components
// (silent mode if verbose is true)
void removeCmd(ItemTree* &IT, bool verbose=true);

// obtain any required arguments then search the requested tree components,
//     returning a pointer to the relevant item
// (null if not found, silent mode if verbose is true)
Item* findCmd(ItemTree* IT, bool verbose=true);

// delete and nullify current tree
// (silent mode if verbose is true)
void deleteCmd(ItemTree* &IT, bool verbose=true);

// create a new item tree (deletes and replaces old)
// (silent mode if verbose is true)
void newCmd(ItemTree* &IT, bool verbose=true);

#endif

