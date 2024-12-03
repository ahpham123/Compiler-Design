import re
import os
def clean_file(read, write):                                           #Read lines from input file(read) and store into list lines
    try:
        with open(read, 'r', encoding='utf-8') as file:
            lines = file.readlines()
    except UnicodeDecodeError:
        print(f"Error: Unable to decode the file '{read}' with UTF-8 encoding.")
    except FileNotFoundError:
        print(f"Error: The file '{read}' was not found.")
    except Exception as e:
        print(f"A different error: {e} has occurred")

    #Algorithim to get rid of comments
    singleline = ''.join(lines)                                         #Joins all lines into single string for re.sub formatting
    singleline = re.sub(r'\(\*.*?\*\)', '', singleline, flags=re.DOTALL)#Removes anything inbetween (* and *)

    clean_lines = []
    clean_lines = singleline.splitlines()                               #Seperates giant list into respective lines and stores into new list

    #Algorithim to get rid of spaces
    res = []
    for line in clean_lines:
        if line == "":
            continue
        line = re.sub(r'\s+', ' ', line).strip()                        #r'\s+' is an expression for recognizing one or more whitespace chars (ie. space tab newline) and replaces them with singular ''
        if line != "":                                                  #.sub deals with internal white space, .strip() deals with external white space (beginning or end)
            if line == "end":
                res. append(line)
            else:
                res.append(line + "\n")

    #Algorithim to adjust spaces between commas
    res = [re.sub(r'\s*,\s*', ' , ', line) for line in res]             # Fix spaces around commas
    for i, line in enumerate(res):
        if i == 0:
            continue
        res[i] = re.sub(r'(?<!\s);', ' ;', line)                        # Add space before semicolons

    try:                                                                #Write correct lines into new file(write)
        if os.path.exists(write):
            with open(write, "w", encoding='utf-8') as file:            #If file already exists, wipe it before writing the editted version
                pass
        with open(write, "a+", encoding='utf-8') as file:               #Begin writing editted file, if file doesnt exist "a+" will make a newfile
            for line in res:
                file.writelines(line)
    except Exception as e:
        print(f"An error has occured: {e}")

#EXAMPLE USAGE, reading from final.txt and writing into empty.txt
clean_file("final.txt", "cleanup.txt")