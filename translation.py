import re
import os
#TODO: Store variable type (ie. Integer)
def translate_file(read, write):
    with open(read, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    
    #TODO: Modify algorithm to use a dict and store type aswell as name
    #Algorithm to extract variable names from variable declaration
    varflag = False
    vars = []                                                          #List to find variables in their declaration
    for line in lines:
        if line.lower() == "var\n":                                    #Entering variable declaration
            varflag = True
            continue
        if line.lower() == "begin\n":                                  #Exitting variable declaration
            varflag = False
            break
        if varflag:
            try:
                variables = re.findall(r'(\w+)(?=\s*,|\s*:)', line)
                match = re.search(r':\s*(.*?)\s*;', line)             #Extract variable type found after : before ;
                vartype = match.group(1).strip()
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
    # print("Varval: ")
    # print(varval)
    # print("Vars: ")
    # print(vars)


    translated_lines = []
    translated_lines.append("#include <iostream>")
    translated_lines.append("using namespace std;")
    translated_lines.append("int main()")
    translated_lines.append("{")
    #Algorithm to begin translating txt lines to code
    beginflag = False
    for line in lines:
        lineappend = ""

        if line == "end":                                               #Reach end
            translated_lines.append("\tsystem (“pause”);\n\treturn 0;\n}")
            break

        if line == "var\n":                                             #Entering variable delcaration section
            lineappend += "\t" + vartype + " "
            for index, var in enumerate(vars):
                if index == len(vars) - 1:
                    lineappend += var + ";"
                else:
                    lineappend += var + ", "
            translated_lines.append(lineappend)

        if line == "begin\n":
            beginflag = True
        if beginflag:
            lineappend += "\t"
            if "print" in line:
                lineappend += "cout<< "
                value = re.search(r"print\s*\(\s*(.*?)\s*\)\s*;",line) #Extracts the contents inside print statements
                printcontent = value.group(1).strip()                  #Formats value into a string
                for var in vars:
                    if re.search(r'\b' + re.escape(var) + r'\b', line):                            #For each variable, check if it is in print statement
                        stringsplit = printcontent.split(",")
                        for string in stringsplit:
                            if re.search(r'\b' + re.escape(var) + r'\b', string):
                                lineappend += string + " <<"
                            else:
                                lineappend += "“" + string + "” <<"
                lineappend += " endl;"
                translated_lines.append(lineappend)
    for line in translated_lines:
        print(line)
                        #printcontent = re.sub(r'\b' + re.escape(var) + r'\b', str(varval[var]), printcontent) #Replace all instances of variables found in print statements with the value
                        #printcontent = re.sub(r'[“”"]([^“”"]*)["”"]', r'\1', printcontent) #Remove curly quotes
                        #output = printcontent

                        # if "," in output:                               #Outputs print statements
                        #     output = output.replace(",", "")
                        # if " " in output:
                        #     output = output.replace(" ", "")
                        # #print("\t" + output.strip())



    #For debugging
    # for line in translated_lines:
    #     print(line)

    try:                                                                #Write correct lines into new file(write)
        if os.path.exists(write):
            with open(write, "w", encoding='utf-8') as file:            #If file already exists, wipe it before writing the editted version
                pass
        with open(write, "a+", encoding='utf-8') as file:               #Begin writing editted file, if file doesnt exist "a+" will make a newfile
            for line in translated_lines:
                line += "\n"
                file.write(line)
    except Exception as e:
        print(f"An error has occured: {e}")


translate_file("cleanup.txt", "translaton.txt")
#  Output: 
#       5 
#       Value=72 