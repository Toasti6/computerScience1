#Dr. Andrew Steinberg
#COP3502C Student Python Script v1.1
#This script will automatically compile, run, and compare your results to a provided txt file that contains sample output.
#DO NOT MODIFY THE CONTENTS OF THIS FILE!
#You do not need understand this code as this is out of the scope of this course. However, if you are interested you can discuss with Dr. Steinberg

import os
import sys
import subprocess
import functools

#this is where we will put the input... yes they are global variables but this is ok to do for this scenario since this is a script
#it is list and each item will be string representing input for each question
#example  ['1 2 3', '2' , '4.2 41.4']
qinputs = ['0 89'] 

#important info to input
exefile = "program2"
txtsolfile = "/samplesolutionp2.txt"
studentsoltxtfile = "/program2solution_student_output.txt"
cfile = "program2_lastname_firstname.c"

def result():
    cwd = os.getcwd()
    
    f1 = open(cwd + txtsolfile, "r")
    f2 = open(cwd + studentsoltxtfile, "r")
    
    #read output from each text file
    l1 = f1.readlines()
    l2 = f2.readlines()
    
    
    count = 0
    #compare the content read
    if len(l1) == len(l2) and functools.reduce(lambda x, y : x and y, map(lambda p, q: p == q, l1, l2), True):   
        print("  ------  ")
        print(" / o  o \ ")
        print("|        |")
        print("| \____/ |")
        print(" \      / ")
        print("  ------  ")
        print("YES!!!! You matched the expected output. Double check for memory leaks by using Valgrind!")
    else: 
        print("  ------  ")
        print(" / o  o \ ")
        print("|        |")
        print("|  ____  |")
        print("| /    \ |")
        print(" \      / ")
        print("  ------  ")
        print("Oh no! Your output was incorrect. Try again and don't give up! You got this!") 
    
    

def compilerunfile():
    cwd = os.getcwd()
    
    #gcc compile command the executable will be program1 instead of a.out

    for subdir, dirs, files in os.walk(cwd):
        for myfile in files:
            if myfile[:8] == exefile and myfile[-2:] == '.c':
                gcccmd = "gcc -o " + exefile + " " + myfile + " -lm" #compile solution file command and generate executable called program2

                proc = subprocess.run([gcccmd], capture_output=True, text = True, shell = True)
                
                if(len(str(proc.stderr)) > 0):
                    print("Ugh oh... Your program had warning/error messages! Try to fix them or else points are deducted.")
                    print("------------------------------------------------------------------------")
                    print(str(proc.stderr))
                    print("------------------------------------------------------------------------")
        break
    
    crun = "./" + exefile
    
    f = open(studentsoltxtfile[1:], "w")
    
    #list comprehension
    inputs = ' '.join([str(item) for item in qinputs])
  
    
     
    
    
    
    #catch infinite loops
    try:
        proc2 = subprocess.run([crun], capture_output=True, input = inputs, text = True, shell = True, timeout= 0.3) #nicer than above
        f.write(str(proc2.stdout))
        print("Done running the student's solution.")
        f.close()        
    except subprocess.TimeoutExpired as e:
        print("Your program is stuck in an infinite loop! Terminating Script!")
        exit(1)


def setupchecker():
    print("We are now checking to make sure you have all necessary files in the same directory.")
    cwd = os.getcwd() #this grabs the current directory as it varies for each student based on NID
    
    #first check to see if solution txt file exists in the same directory as this script
    
    if not os.path.exists(cwd + txtsolfile):
        raise FileNotFoundError("The provided sample solution text file is not in the currect directory as this script." + 
        " Please place the samplesolutionp2 text file in this directory and try again. Script Exiting!")
        
    
    
    #check to see if c file is placed properly. Only checks parts of it. Student is responsible for naming it properly with respective name as stated in the directions.
    flag = 0
    for subdir, dirs, files in os.walk(cwd):
        for myfile in files:
            if myfile[:8] == exefile and myfile[-2:] == '.c':
                flag = 1
    
    if flag == 0:
        raise FileNotFoundError("You are missing your C source!! Please put in your "+ cfile + " file. Script Exiting!")
        
    print("We are done with checking to make sure necessary files were provided.")
    

def main():
    print("Beginning the test of my program compared with a provided txt file from Dr. Steinberg.")
    print("Before this script tests the program, we need to make sure you have everything in order.")
    setupchecker()
    print("Setup is now complete. Begin testing.")
    print("Now it is time to compile the file.")
    compilerunfile()
    print("Now it is time for the moment of truth. Comparing my file output with Dr. Steinberg's sample solution file.")
    result()
    
    
    

if __name__ == "__main__":
    main()