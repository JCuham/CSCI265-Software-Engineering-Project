#! /bin/bash

# this script takes an integer number of items from the user,
#    and generates a random test case for the demo program
#    (using that number of items)
# the test case operates in quiet mode except the final quit,
#    generates somewhat random items to insert in the ItemTree,
#    and does a search for each item immediately after insertion

# hardcoded target directory for storing test cases
testDir="tests/quiet/"

# generate and return a random quaternary string 0-7 characters long
function gen()
{
   local str=""
   local len=$RANDOM
   (( len = len % 8 ))
   for (( i=0; i<len; i++)) {
       local digit=$RANDOM
       (( digit = digit % 4 ))
       local tmp="${str}${digit}"
       str="${tmp}"
   }
   echo "$str"
}

# expects a positive integer from the user,
#    indicating the desired number of items
#    to insert in the tree

if [ $# -lt 1 ] ; then
   echo "Error: number of items expected as command line argument"
   exit 0
fi

# capture command line argument and test it is an integer
numItems=$1
intPat='^[0-9]+$'

if [[ $numItems =~ $intPat ]] ; then

   # valid integer obtained, generate file
   fname="${testDir}genFile.$numItems"
   echo "generating $numItems items, storing in file $fname"
   echo "V" > $fname
   echo "N" >> $fname
   for (( i=0; i<$numItems; i++)) {
       # insert a randomly generated item into the tree
       echo "I" >> $fname
       echo "item $i" >> $fname
       base=$(gen)
       ll="${base}0"
       ur="${base}3"
       echo "ll $ll, ur $ur" >> $fname
       echo "$ll" >> $fname
       echo "$ur" >> $fname
       # now search the entire tree for that same item
       echo "F" >> $fname
       echo "T" >> $fname
       echo "*" >> $fname
       echo "item $i" >> $fname
   }
   echo "V" >> $fname
   echo "Q" >> $fname

else

   # invalid command line argument supplied
   echo "Error: positive integer number of items expected, not ${numItems}"

fi

