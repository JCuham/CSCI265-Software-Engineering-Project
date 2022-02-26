#include "../hdr/Address.h"
#include <iostream>
#include <cwctype>
#include <cwchar>
#include <clocale>

// --- specify values for the static class constants ---
const long Address::MaxAddr=10;  // maximum number of digits in an address
const long Address::AddrBase=4;  // value base of addresses, e.g.
                                 //    4 for quadtree, 8 for octree

// --- standard class methods ---

// destructor: no action currently needed
Address::~Address()
{
}


// constructor checks/stores address string
Address::Address(std::wstring astr)
{
    set(astr);
}


// getter for address string
std::wstring Address::get()
{
   return addr;
}


// return true iff our quadrant contains a's
bool Address::contains(Address a)
{
   return contains(addr, a.get());
}


// return true iff our quadrant contains that specified by astr
bool Address::contains(std::wstring astr)
{
   return contains(addr, astr);
}


// if str is valid, set address and return true,
// otherwise do not change, just return false
bool Address::set(std::wstring astr) {
     if (!isValidAddr(astr)) return false;
     addr = astr;
     return true;
}


// return size of quadrant represented by our address
long Address::getQuadSize()
{
   // 2^(MaxAddr - length of addr string)
   return (1 << (MaxAddr - addr.length()));
}


// --- static class methods ---

// return true if the quadrant represented by the first address string
//     contains the quadrant represented by the second
bool Address::contains(std::wstring a1, std::wstring a2)
{
   // false if a1 is smaller quadrant, i.e. longer specified address
   if (a1.length() > a2.length()) return false;

   // false if a1 specifies different quadrant than that containing a2
   for (unsigned int i = 0; i < a1.length(); i++) {
       if (a1[i] != a2[i]) return false;
   }
   return true;
}

// does a string represent a valid address?
bool Address::isValidAddr(std::wstring astr)
{
   // make sure string is not too long
   if (astr.length() > MaxAddr) return false;

   // make sure string only contains digits smaller than AddrBase
   for (unsigned int i = 0; i < astr.length(); i++) {
       if (!isdigit(astr[i])) return false;
       if (astr[i] >= ('0' + AddrBase)) return false;
   }
   return true;
}

// in case a string does not represent an entirely valid address,
//    return the prefix portion that does form a valid address (possibly "")
std::wstring Address::prefixAddr(std::wstring astr)
{
   std::wstring prefix = L"";
   unsigned int pos = 0;
   while (pos < astr.length()) {
      if (iswdigit(astr[pos]) && (astr[pos] < ('0' + AddrBase))) {
         prefix += astr[pos];
         pos++;
      }
      else return prefix;
   }
   return prefix;
}

// given strings for two addresses, return the string for the most
//    specific quadrant containing both (longest common valid prefix)
std::wstring Address::commonQuad(std::wstring a1, std::wstring a2)
{
   std::wstring prefixA = prefixAddr(a1);
   std::wstring prefixB = prefixAddr(a2);
   unsigned int pos = 0;
   std::wstring quad = L"";
   while ((pos < prefixA.length()) && (pos < prefixB.length())) {
      if (prefixA[pos] == prefixB[pos]) quad += prefixA[pos];
      else return quad;
      pos++;
   }
   return quad;
}

// for the quadrant specified by the parameter string,
//     return the address string for its lower left corner
// (i.e. if string is shorter than MaxAddr, pad with 0's)
std::wstring Address::quadLL(std::wstring qstr)
{
   std::wstring LL = qstr;
   while (LL.length() < MaxAddr) {
      LL += '0';
   }
   return LL;
}

// for the quadrant specified by the parameter string,
//     return the address string for its upper right corner
// (i.e. if string is shorter than MaxAddr, pad with 3's)
std::wstring Address::quadUR(std::wstring qstr)
{
   std::wstring UR = qstr;
   while (UR.length() < MaxAddr) {
      UR += '3';
   }
   return UR;
}

// look up the x,y coordinates of the centre of the specified quadrant,
//    assumes global quadrant centered at 0,0
//    returns false if address is invalid
bool Address::quadCentre(std::wstring addr, float &x, float &y)
{
   if (!isValidAddr(addr)) return false;
   unsigned int pos = 0;
   x = 0;
   y = 0;
   int width = 1 << MaxAddr; // width (and height) of the global space
   float offsetDist = width/2.0; // offset distance to midpoint of a subquadrant
   while (pos < addr.length()) {
      offsetDist /= 2;  // with each new specified position we halve our quadrant width/height
      switch (addr[pos]) {
         case '0': // lower left quarter
            x -= offsetDist;
            y -= offsetDist;
            break;
         case '1': // upper left quarter
            x -= offsetDist;
            y += offsetDist;
            break;
         case '2': // lower right quarter
            x += offsetDist;
            y -= offsetDist;
            break;
         case '3': // upper right quarter
            x += offsetDist;
            y += offsetDist;
            break;
         default:  // should not be possible with valid address
            break;
      }
      pos++;
   }
   return true;
}

