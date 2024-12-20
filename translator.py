import re
import os
def translate_file(read, write):
    with open(read, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    
    #Algorithm to extract variable names from variable declaration
    varflag = False
    vars = []                                                           #List to find variables in their declaration
    for line in lines:
        if line.lower() == "var\n":                                     #Entering variable declaration
            varflag = True
            continue
        if line.lower() == "begin\n":                                   #Exitting variable declaration
            varflag = False
            break
        if varflag:
            try:
                variables = re.findall(r'(\w+)(?=\s*,|\s*:)', line)
                match = re.search(r':\s*(.*?)\s*;', line)               #Extract variable type found after : before ;
                vartype = match.group(1).strip()

                if vartype == "integer":                                #Change to C++ syntax var type declaration
                    vartype = "int"

                for variable in variables:
                    vars.append(variable)
            except Exception as e:
                print(f"An error has occured: {e}")

    
    #Algorithm for extracting variable values
    varval = {}                                                         #Dict to store variable and their respective value
    for line in lines:
        if not "=" in line:
            continue

        lhs, rhs = line.split("=")
        if not lhs.strip() in vars:                                     #If line is in form of var = ...  
            continue
        for var in vars:
            if re.search(r'\b' + re.escape(var) + r'\b', rhs):          #Var found in rhs
                for key, value in varval.items():
                    rhs = re.sub(r'\b' + re.escape(key.strip()) + r'\b', value, rhs) #Only replace the whole variable, not partial matches (ie, a in b2a)
                rhs = rhs.strip().rstrip(';')                           #Cleanup new rhs for eval() function
                try:
                    result = eval(rhs)                                  #Evaluate to a single number
                    varval[lhs.strip()] = result
                except Exception as e:
                    print(f"Error evaluating expression: {e}")
                break                                                   #Break the loop once a variable is found to prevent running multiple times in one line
            else:
                try:                        
                    target = r'(\d+)\s+;'
                    value = re.findall(target, rhs)
                    varval[lhs.strip()] = value[0]
                except Exception as e:
                    print(f"Error doing simple assignment: {e}")

    translated_lines = []
    translated_lines.append("#include <iostream>\n")
    translated_lines.append("using namespace std;\n")
    translated_lines.append("int main()\n")
    translated_lines.append("{\n")
    #Algorithm to begin translating txt lines to code
    beginflag = False
    for line in lines:
        lineappend = ""

        if line == "end":                                               #Reach end
            translated_lines.append("\treturn 0;\n}")
            break

        if line == "var\n":                                             #Entering variable delcaration section
            lineappend += "\t" + vartype + " "
            for index, var in enumerate(vars):
                if index == len(vars) - 1:
                    lineappend += var + ";"
                else:
                    lineappend += var + ", "
            lineappend = lineappend + "\n"
            translated_lines.append(lineappend)
            continue

        if line == "begin\n":
            beginflag = True
        if beginflag:
            lineappend += "\t"
            if not "print" in line:
                pass
            else:
                lineappend += "cout<< "
                value = re.search(r"print\s*\(\s*(.*?)\s*\)\s*;",line) #Extracts the contents inside print statements
                printcontent = value.group(1).strip()                  #Formats value into a string
                if "“" in printcontent or "”" in printcontent:
                    printcontent = re.sub(r'[“”]', '"', printcontent)
                for var in vars:
                    if re.search(r'\b' + re.escape(var) + r'\b', line):#For each variable, check if it is in print statement
                        stringsplit = printcontent.split(",")
                        for string in stringsplit:
                            lineappend += string + " <<"
                lineappend += " endl;\n"
                translated_lines.append(lineappend)
                continue
            if "=" in line:
                lhs, rhs = line.split("=")
                for var in vars:
                    if re.search(r'\b' + re.escape(var) + r'\b', lhs): #If variable assignment
                        line = re.sub(r'\s*;', ';', line)
                        line = "\t" + line
                        translated_lines.append(line)

    try:                                                                #Write correct lines into new file(write)
        if os.path.exists(write):
            with open(write, "w", encoding='utf-8') as file:            #If file already exists, wipe it before writing the editted version
                pass
        with open(write, "a+", encoding='utf-8') as file:               #Begin writing editted file, if file doesnt exist "a+" will make a newfile
            for line in translated_lines:
                file.write(line)
    except Exception as e:
        print(f"An error has occured: {e}")

#Sample usage
translate_file("cleanup.txt", "translation.cpp")