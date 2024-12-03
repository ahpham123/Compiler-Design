import re
import os
#TODO: Store variable type (ie. Integer)
def translate_file(read, write):
    with open(read, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    
    #Algorithm to extract variable names from variable declaration
    varflag = False
    vars = []                                                          #List to find variables in their declaration
    for line in lines:
        if line.lower() == "var\n":
            varflag = True
            continue
        if varflag:
            try:
                variables = re.findall(r'(\w+)(?=\s*,|\s*:)', line)
                for variable in variables:
                    vars.append(variable)
            except Exception as e:
                print(f"An error has occured: {e}")
    #Algorithm for extracting variable values
    varval = {}                                                        #Dict to store variable and their respective value
    for line in lines:
        if not "=" in line:
            continue

        lhs, rhs = line.split("=")
        if not lhs.strip() in vars:                                    #If line is in form of var = ...  
            continue

        for var in vars:
            if re.search(r'\b' + re.escape(var) + r'\b', rhs):         #Var found in rhs
                for key, value in varval.items():
                    rhs = re.sub(r'\b' + re.escape(key.strip()) + r'\b', value, rhs) #Only replace the whole variable, not partial matches (ie, a in b2a)
                rhs = rhs.strip().rstrip(';')                          #Cleanup new rhs for eval() function
                try:
                    result = eval(rhs)                                 #Evaluate to a single number
                    varval[lhs.strip()] = result
                except Exception as e:
                    print(f"Error evaluating expression: {e}")
                break                                                  #Break the loop once a variable is found to prevent running multiple times in one line
            else:
                try:                        
                    target = r'(\d+)\s+;'
                    value = re.findall(target, rhs)
                    varval[lhs.strip()] = value[0]
                except Exception as e:
                    print(f"Error doing simple assignment: {e}")

    #For testing, delete in final draft
    print(varval)

    try:                                                                #Write correct lines into new file(write)
        if os.path.exists(write):
            with open(write, "w", encoding='utf-8') as file:            #If file already exists, wipe it before writing the editted version
                pass
        with open(write, "a+", encoding='utf-8') as file:               #Begin writing editted file, if file doesnt exist "a+" will make a newfile
            pass
    except Exception as e:
        print(f"An error has occured: {e}")


translate_file("cleanup.txt", "translaton.txt")
#  Output: 
#       5 
#       Value=72 