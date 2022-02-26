#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include <cwctype>
#include <cwchar>
#include <clocale>

// Overview of our quaternary addressing system:
//
// Addresses are a string consisting of 0 or more quaternary
//    digits (0-3), identifying a 2D quadrant in which something
//    is located (note these differ from the traditional math
//    quadrant numbering)
//
// - the null string "" simply identifies the global space
//
// - the first digit specifies the item as belonging in one
//   of four quadrants in the global space:
//                   |
//              1    |    3
//                   |
//      -------------+-----------
//                   |
//              0    |    2
//                   |
// - the second digit specifies a "quadrant within a quadrant",
//   e.g. 01 indicates the 1 quadrant within the 0 quadrant
//         11 | 13   |   31 | 33
//        ----+----  |  ----+----
//         10 | 12   |   30 | 32
//      -------------+-----------
//         01 | 03   |   21 | 23
//        ----+----  |  ----+----
//         00 | 02   |   20 | 22
//
// - the third digit is used to specify a "quad within a quad within a quad",
//   etc etc, thus a full map coordinates for four digits would look like
//
//  1111 1113 1131 1133  1311 1313 1331 1333   3111 3113 3131 3133  3311 3313 3331 3333
//  1110 1112 1130 1132  1310 1312 1330 1332   3110 3112 3130 3132  3310 3312 3330 3332
//  1101 1103 1121 1123  1301 1303 1321 1323   3101 3103 3121 3123  3301 3303 3321 3323
//  1100 1102 1120 1122  1300 1302 1320 1322   3100 3102 3120 3122  3300 3302 3320 3322
//
//  1011 1013 1031 1033  1211 1213 1231 1233   3011 3013 3031 3033  3211 3213 3231 3233
//  1010 1012 1030 1032  1210 1212 1230 1232   3010 3012 3030 3032  3210 3212 3230 3232
//  1001 1003 1021 1023  1201 1203 1221 1223   3001 3003 3021 3023  3201 3203 3221 3223
//  1000 1002 1020 1022  1200 1202 1220 1222   3000 3002 3020 3022  3200 3202 3220 3222
//
//  0111 0113 0131 0133  0311 0313 0331 0333   2111 2113 2131 2133  2311 2313 2331 2333
//  0110 0112 0130 0132  0310 0312 0330 0332   2110 2112 2130 2132  2310 2312 2330 2332
//  0101 0103 0121 0123  0301 0303 0321 0323   2101 2103 2121 2123  2301 2303 2321 2323
//  0100 0102 0120 0122  0300 0302 0320 0322   2100 2102 2120 2122  2300 2302 2320 2322
//
//  0011 0013 0031 0033  0211 0213 0231 0233   2011 2013 2031 2033  2211 2213 2231 2233
//  0010 0012 0030 0032  0210 0212 0230 0232   2010 2012 2030 2032  2210 2212 2230 2232
//  0001 0003 0021 0023  0201 0203 0221 0223   2001 2003 2021 2023  2201 2203 2221 2223
//  0000 0002 0020 0022  0200 0202 0220 0222   2000 2002 2020 2022  2200 2202 2220 2222
//

// The Address class is used to store and perform basic calculations
// on our quaternary addresses, including methods to check the validity
// of address strings, find valid prefix strings, checking if the quadrant
// described by one address contains another, etc

class Address {
   public:
      // -- static class constants and methods --
      static const long MaxAddr;        // maximum number of digits in an address
      static const long AddrBase;       // value base of addresses, i.e. 4 for quadtree

      // does a string represent a valid address?
      static bool isValidAddr(std::wstring astr);

      // in case a string does not represent an entirely valid address,
      //    return the prefix portion that does form a valid address (possibly "")
      static std::wstring prefixAddr(std::wstring astr);

      // given strings for two addresses, return the string for the most
      //    specific quadrant containing both (longest common valid prefix)
      static std::wstring commonQuad(std::wstring a1, std::wstring a2);

      // for the quadrant specified by the parameter string,
      //     return the address string for its lower left corner
      static std::wstring quadLL(std::wstring qstr);

      // for the quadrant specified by the parameter string,
      //     return the address string for its upper right corner
      static std::wstring quadUR(std::wstring qstr);

      // return true if the quadrant represented by the first address string
      //     contains the quadrant represented by the second
      static bool contains(std::wstring a1, std::wstring a2);

      // look up the x,y coordinates of the centre of the specified quadrant,
      //    assumes global quadrant centered at 0,0
      //    returns false if address is invalid
      static bool quadCentre(std::wstring addr, float &x, float &y);

      // --- standard methods ---
      // destructor (no action required currently)
     ~Address();

      // constructor uses global address string by default or if invalid astr provided
      Address(std::wstring astr=L"");

      // return the stored valid address string
      std::wstring get();

      // return true iff our quadrant contains a's
      bool contains(Address a);
      bool contains(std::wstring a);

      // if str is valid, set address and return true,
      // otherwise do not change, just return false
      bool set(std::wstring astr);

      // return size of quadrant represented by our address
      long getQuadSize();

      // 

   protected:
      std::wstring addr; // stored valid address

};

#endif

