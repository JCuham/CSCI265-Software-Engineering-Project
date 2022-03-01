#! /bin/bash

#------------------------#
#     runtests.sh        #
#------------------------#


#This script takes a directory name or text file and executible
#searches the directory and any subdirectories for any readable files
#then runs the executible on each file uses the file content as input

#checking if is a directory and is readable
    read -p "Please enter a directory or text file name> " directory
    if [[ ! -d "${directory}" || ! -f "${directory} ]] && [[ ! -r "${directory}" || ! -r "${directory} "]] ; then
    echo "Not a valid directory or readable directory"
    exit 1
    fi

#checking if exe is readable executible and a file
    read -p "Please enter an executible name> " executible
    if [[ ! -f "${executible}" ]] && [[ ! -r "${executible}" ]] && [[ ! -x "${executible}" ]] ; then
    echo "Not a valid executible file or is not readable. QUITTING"
    exit 1
    fi


#recursive explore function
explore(){
    #local vars for exe and dir
    local dir=$1
    local exe=$2

    #for loop looking for files in current directory
    for crnt in $(ls -d "${dir}")
    #for crnt in "$dir"
    do
    #checking if file or another directory
    if [[ ! -d "${dir}/${crnt}" ]] ; then
    ${exe} < "${crnt}"
    #if another directory
    else
    explore "${dir}/${crnt}" "${exe}"
    fi
    done
}

#passing dir and exe to explore
    explore ${directory} ${executible}

