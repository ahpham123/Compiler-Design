import re
import os
#TODO: Store variable type (ie. Integer)
def translate_file(read, write):
    varval = {}                                                         #Dict to store variable and their respective value
    with open(read, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    varflag = False
    vars = []                                                           #List to find variables in their declaration
    
    #Algorithm to extract variable names from variable declaration
    for line in lines:
        if line.lower() == "var\n":
            varflag = True
            continue
        if varflag:
            variables = re.findall(r'(\w+)(?=\s*,|\s*:)', line)
            for variable in variables:
                vars.append(variable)

    #For testing, delete in final draft
    print(vars)

    for line in lines:
        if "=" in line:
            lhs, rhs = line.split("=")
            if lhs.strip() in vars:                                     #If line is in form of var = ...          
                pass
                #TODO: implement, print statements are for debugging
                # print(line)
                # print(lhs.strip())
                # print(rhs.strip())


    try:                                                                #Write correct lines into new file(write)
        if os.path.exists(write):
            with open(write, "w", encoding='utf-8') as file:            #If file already exists, wipe it before writing the editted version
                pass
        with open(write, "a+", encoding='utf-8') as file:               #Begin writing editted file, if file doesnt exist "a+" will make a newfile
            for line in vars:
                file.writelines(line)
    except Exception as e:
        print(f"An error has occured: {e}")


translate_file("cleanup.txt", "translaton.txt")
#  Output: 
#       5 
#       Value=72 