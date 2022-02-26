#! /bin/bash
#---------------------------------------------#
#               filecheck.sh                  #
#---------------------------------------------#
#The purpose of this script is to check if the arguments
#passed to it are files. Then show who has made changes to
#said file(s) and pipe the result of git blame to a text file
#called "less", Then run head on the file for the first 5 lines
#and display output via stdout. Then run wc -l and wc -m on the file
#save the number of lines and characters to variables
#and output the variables via stdout



#checking for files passed as arguments
#going to use for loop as arguements are valid
for arg in $@ ; do
    #if arg is a directory display error message and continue script
    if [[ -d ${arg} ]] ;then
        echo "{$arg} is a directory and is not valid quitting now"

	#check if it is a file and is readable then run git blame and wc
    elif [[ -f ${arg} && -r ${arg} ]]; then
        echo "{$arg} is a file"
	#running git blame on file
        echo "Now running git blame and piping to less on: ${arg}"
        git blame ${arg} | less
	#running head command and letting output go to stdout
        echo "Now running head -5 on: ${arg}"
        head -5 ${arg}
	#running wc command for lines and character count
        lines=$(wc -l ${arg} | cut -f1 -d' ')
        characters=$(wc -m ${arg} | cut -f1 -d' ')
	#outputing num lines and chars for current file
        echo "Number of lines followed by file name: ${lines}"
        echo "Number of characters followed by file name: ${characters}"

	#displays error message and continues script
    else
        echo "$arg is not a file or is an unreadable file"
    fi
done
