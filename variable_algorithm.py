    #Algorithim to collect variable names
    varflag = False
    vars = []
    for line in res:
        if line.lower() == "var\n":
            varflag = True
            continue
        if varflag:
            if line.lower() == "begin":
                varflag = False
            if varflag:
                variables = re.findall(r'(\w+)(?=\s*,|\s*:)', line)     #Look for all raw strings with patterns of words (\w+) and look ahead (?=\s*,|\s*:) for commas or colons in line and make it into a list
                for variable in variables:
                    vars.append(variable)

    #For testing, delete in final draft
    print(vars)