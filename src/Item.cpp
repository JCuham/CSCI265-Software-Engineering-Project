#include "../hdr/Item.h"
#include "../hdr/Address.h"
#include <iostream>
#include <cwctype>
#include <cwchar>
#include <clocale>

// create item from strings for title, description, lower left and upper right address
// (if invalid addresses are given then just the first valid prefix is used, and
//  if only partial addresses are given then LL is padded with 0's, UR with 3's)
Item::Item(std::wstring t, std::wstring d, std::wstring LL, std::wstring UR)
{
   title = t;
   desc = d;

   // process LL, checking for validity, truncate and pad as needed
   lowerLeft = Address::prefixAddr(LL);
   lowerLeft = Address::quadLL(lowerLeft);

   // process UR, checking for validity, truncate and pad as needed
   upperRight = Address::prefixAddr(UR);
   upperRight = Address::quadUR(upperRight);

   // identify item's quadrant, i.e. longest common prefix string
   //    shared by lowerLeft and upperRight
   quadrant = Address::commonQuad(lowerLeft, upperRight);
}

// lookup the item's stored string content
void Item::lookup(std::wstring &t, std::wstring &d,
            std::wstring &LL, std::wstring &UR, std::wstring &quad)
{
   t = title;
   d = desc;
   LL = lowerLeft;
   UR = upperRight;
   quad = quadrant;
}

// does this item overlap (or touch) another?
bool Item::overlap(Item i)
{
   std::wstring it, id, iLL, iUR, iquad;
   i.lookup(it, id, iLL, iUR, iquad);
   float top, left, bottom, right;
   float itop, ileft, ibottom, iright;

   // get the coordinates for the top, bottom, left, and right of this Item's enclosing box
   if (!Address::quadCentre(lowerLeft, left, bottom)) return false;
   if (!Address::quadCentre(upperRight, right, top)) return false;

   // get the coordinates for the top, bottom, left, and right of this Item's enclosing box
   if (!Address::quadCentre(iLL, ileft, ibottom)) return false;
   if (!Address::quadCentre(iUR, iright, itop)) return false;

   // they don't overlap if one is entirely above/below the other
   if (top < ibottom) return false;
   if (itop < bottom) return false;

   // they don't overlap if one is entirel to the left/right of the other
   if (left > iright) return false;
   if (ileft > right) return false;

   // otherwise they do overlap (or touch)
   return true;
}

// print the item's information
void Item::print()
{
   std::wcout << title << "::" << desc;
   std::wcout << "(" << quadrant << ":";
   std::wcout << lowerLeft << "," << upperRight << ")" << std::endl;
}

