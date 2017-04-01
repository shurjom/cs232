#!/bin/sh

#CS232_proj2_BourneShellScripts 
#Shurjo Maitra
#sm47
#Febuary 27, 2017

shutdown="No route to host." #to verify with the machine is off
windows="Connection timed out." #to check if the machhine is running windows

filename='names.txt' #for storing machine names
fileline=`cat $filename` #for reading out lines in names.txt

for line in $fileline; do #to iterate through machine names from text files
	echo #whitespace 
	echo $line #writes machine name

	output=$(ssh -n -o StrictHostKeyChecking=no -o ConnectTimeout=5 $line 'users && exit') 2> error.txt 1> output.txt
	#ssh into remote machines and writes errors if any, exit from both users and root
	read error < error.txt 
	#reads in errors to a variable
	read output < output.txt
	#reads in output for machines on
	if  echo "$error" | grep -q "$shutdown" #if "No route to host" is in error.txt
		then
		echo "Machine is off." #machine is off
	else
		echo "Machine is on." #otherwise machine is on
		if echo "$error" | grep -q "$windows" #if "Connection timed out." is in error.txt
			then
			echo "Windows is running on $line." #write windows as machine to output
		else
			echo "Ubuntu is running on $line." #else output ubuntu 
			if [ -z "$output" -a -z "$error" ]; then #if no error and no ouput
				echo "$line has no users logged in." #there are no users on the machine
			else 
				echo "$line has users logged in." #there are users on the machine
			fi 
		fi
	fi
done
