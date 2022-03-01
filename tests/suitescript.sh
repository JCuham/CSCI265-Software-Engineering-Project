#!/bin/bash

#------------------------#
#     suitescript.sh     #
#------------------------#

#This script takes the type of tests to run as arguments
#and calls runtests.sh to run individual tests then
#compares the difference between the result of the test and the
#expected result and pipes the output to a results file

# More than one argument specified
if [! $# == 1 ] ; then
	echo "More than one or no argument specified this script requires one argument either testCmds, testFunc or testAll"
	exit
fi

#Source Files
binary="../bin/demo.exe"
expCmds="testCmdsExpected"
expFunc="testFuncExpected"

#Start testing
if [ "$1" == "testCmds" ] ; then
	./runtests.sh testCmds ${binary} > results
	diff results.txt ${expCmds} > resultsCmd
	echo "Completed testCmds test results located in resultsCmd.txt"
	exit
elif [ "$1" == "testFunc" ] ; then
	./runtests.sh testFunc ${binary} > results
	diff results.txt ${expFunc} > resultsFunc
	echo "Completed testFunc test results located in resultsFunc.txt"
	exit
elif [ " $1" == "testAll" ] ; then
	./runtests.sh testCmds ${binary} > results
	diff results.txt ${expCmds} > resultsCmd
	./runtests.sh testFunc ${binary} > results
	diff results.txt ${expFunc} > resultsFunc
	cat resultsCmd resultsFunc > results
	echo "Completed testAll test results located in results.txt"
	exit
else
	echo "Invalid Command"
	exit
fi

