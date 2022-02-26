#ifndef ITEM_H
#define ITEM_H
#include <string>
#include <cwctype>
#include <cwchar>
#include <clocale>

// Items consist of titles, description strings, and address strings
// identifying the (quaternary) addresses of the lower left and upper right
// corners the item occupies.
//
// For instance, we might use an item to describe a sales region for a representative
//     Item("Fred\'s territory", "Fred is in charge of sales here", "1013", "120")
//
// During Item creation, the addresses for the lower left and upper right corners
//    are padded to full addresses (extreme lower left, upper right) and the
//    quadrant containing the entire region is also calculated and stored
// (e.g., if our full addresses were 10 characters long, then for Fred's region
//  the padded addresses would be 1013000000 and 1200000000, and the enclosing
//  quadrant would be 1)
//
// Items are currently used only for data storage and retrieval, so the only associated
//    methods are the constructor, destructor, and lookup

class Item {
   public:
      // create item from strings for title, description, lower left and upper right address
      // (if invalid addresses are given then just the first valid prefix is used, and
      //  if only partial addresses are given then LL is padded with 0's, UR with 3's)
      Item(std::wstring t=L"", std::wstring d=L"", std::wstring LL=L"", std::wstring UR=L"");

      // lookup the item's stored string content
      void lookup(std::wstring &t, std::wstring &d,
                  std::wstring &LL, std::wstring &UR, std::wstring &quad);

      // print the item's information
      void print();

      // does this item overlap (or touch) another
      bool overlap(Item i);

     ~Item() { } // no action required by destructor

   protected:

      std::wstring title; // title/identifier (not necessarily unique)
      std::wstring desc;  // text description

      // full address string for lower left and upper right corner of the rectangle
      //    enclosing the item
      std::wstring lowerLeft, upperRight;

      // item's enclosing quadrant given by longest shared prefix string
      //    for its lowerleft and upperright addresses
      std::wstring quadrant;

};

#endif
