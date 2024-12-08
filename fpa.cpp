#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_map>
#include <unordered_set>
std::vector<std::pair<std::string, std::string>> R;
std::unordered_map<int, std::unordered_map<std::string, std::string>> LRPT;
std::unordered_set<std::string> variables;

void Rules() {   // use the same method to build table in h8
    R = {
        {"<prog>", "program <identifier>; var <dec-list> begin <stat-list> end"},
        {"<identifier>", "<letter-or-digit> {<letter-or-digit>}"},
        {"<dec-list>", "<dec>:<type>;"},
        {"<dec>", "<identifier>,<dec>"},
        {"<dec>", "<identifier>"},
        {"<type>", "integer"},
        {"<stat-list>", "<stat>"},
        {"<stat-list>", "<stat><stat-list>"},
        {"<stat>", "<write>"},
        {"<stat>", "<assign>"},
        {"<write>", "print (<str><identifier>);"},
        {"<str>", "\"value=\", "},
        {"<str>", "λ"},
        {"<assign>", "<identifier>=<expr>;"},
        {"<expr>", "<expr>+<term>"},
        {"<expr>", "<expr>-<term>"},
        {"<expr>", "<term>"},
        {"<term>", "<term>*<factor>"},
        {"<term>", "<term>/<factor>"},
        {"<term>", "<factor>"},
        {"<factor>", "<identifier>"},
        {"<factor>", "<number>"},
        {"<factor>", "(<expr>)"},
        {"<number>", "<sign><digit>{<digit>}"},
        {"<sign>", "+"},
        {"<sign>", "-"},
        {"<sign>", "λ"},
        {"<digit>", "0|1|2|3|4|5|6|7|8|9"},
        {"<letter-or-digit>", "a|b|c|d|l|f|0|1|2|3|4|5|6|7|8|9"}
    };
}

void Table() { //LRP table learned from h8
    LRPT = {
        {0, {{"program", "S1"}, {"<identifier>", "2"}, {"<dec-list>", "3"}, {"var", "S4"}, {"begin", "S5"}, {"<stat-list>", "6"}, {"end", "ACC"}}},
        {1, {{";", "S7"}}},
        {2, {{";", "R2"}, {",", "R2"}, {":", "R2"}}},
        {3, {{"integer", "S8"}, {";", "R3"}}},
        {4, {{"<identifier>", "S9"}, {"<dec>", "10"}}},
        {5, {{"print", "S11"}, {"<stat>", "12"}, {"<stat-list>", "13"}, {"<assign>", "S14"}}},
        {6, {{"end", "S15"}}},
        {7, {{"var", "S4"}, {"<dec-list>", "3"}}},
        {8, {{";", "S16"}}},
        {9, {{";", "R4"}, {",", "S17"}}},
        {10, {{";", "S16"}}},
        {11, {{"(", "S18"}}},
        {12, {{"print", "R6"}, {"<identifier>", "R6"}, {"=", "R6"}}},
        {13, {{"end", "R7"}}},
        {14, {{"<identifier>", "R8"}, {"=", "S19"}}},
        {15, {{"$", "ACC"}}},
        {16, {{"begin", "S5"}}},
        {17, {{"<identifier>", "S9"}}},
        {18, {{"\"value=\",", "S20"}, {"<identifier>", "S21"}, {")", "S22"}}},
        {19, {{"<expr>", "S23"}}},
        {20, {{"<identifier>", "S21"}, {")", "S22"}}},
        {21, {{";", "S24"}}},
        {22, {{";", "S25"}}},
        {23, {{"+", "S26"}, {"-", "S27"}, {")", "R10"}}},
        {24, {{"begin", "R11"}}},
        {25, {{"begin", "R12"}}},
        {26, {{"<term>", "S28"}}},
        {27, {{"<term>", "S29"}}},
        {28, {{"*", "S30"}, {"/", "S31"}}},
        {29, {{"*", "S30"}, {"/", "S31"}}},
        {30, {{"<factor>", "S32"}}},
        {31, {{"<factor>", "S33"}}},
        {32, {{";", "R13"}, {"<identifier>", "R13"}}},
        {33, {{";", "R14"}, {"<identifier>", "R14"}}}
    };
}

bool program(const std::string& keyword) { // The words need to be detect
    return keyword == "program" || keyword == "var" || keyword == "begin" || 
           keyword == "end" || keyword == "integer" || keyword == "print";
}

bool parse_input(const std::vector<std::string>& inputs) {  // this structure come from cpsc 131
    int index = 0;  // use index for get through the input

    if (index < inputs.size() && inputs[index] == "program") { // All words and symbols need to have error message if do not match(structure learned from 121)
        index++;
    } else {
        std::cout << "program is expected." << std::endl;
        return false;
    }

    if (index < inputs.size() && std::regex_match(inputs[index], std::regex("[a-zA-Z][a-zA-Z0-9]*"))) { // the method to detect the digits and letters
    // get from Quora and cpp reference of regex 
        index++;
    } else {
        std::cout << "Unknown identifier." << std::endl;
        return false;
    }

    if (index < inputs.size() && inputs[index] == ";") {
        index++;
    } else {
        std::cout << "; semicolon is missing." << std::endl;
        return false;
    }
    if (index < inputs.size() && inputs[index] == "var") {
        index++;
    } else {
        std::cout << "var is expected." << std::endl;
        return false;
    }

 while (index < inputs.size() && std::regex_match(inputs[index], std::regex("[a-zA-Z][a-zA-Z0-9]*"))) {
    variables.insert(inputs[index]); // take mutiple variab;es for one times use while and insert
    index++;

    if (index < inputs.size()) {
        if (inputs[index] == ",") {
            index++;
        } else if (std::regex_match(inputs[index], std::regex("[a-zA-Z][a-zA-Z0-9]*"))) {
            std::cout << ", comma is missing." << std::endl;
            return false;
        }
    }
}


    if (index < inputs.size() && inputs[index] == ":") {
        index++;
    } else {
        std::cout << ": colon is missing." << std::endl;
        return false;
    }

    if (index < inputs.size() && inputs[index] == "integer") {
        index++;
    } else {
        std::cout << "integer is expected." << std::endl;
        return false;
    }

    if (index < inputs.size() && inputs[index] == ";") {
        index++;
    } else {
        std::cout << "; semicolon is missing." << std::endl;
        return false;
    }

    if (index < inputs.size() && inputs[index] == "begin") {
        index++;
    } else {
        std::cout << "begin is expected." << std::endl;
        return false;
    }

   while (index < inputs.size() && inputs[index] != "end") {
    if (std::regex_match(inputs[index], std::regex("[a-zA-Z][a-zA-Z0-9]*"))) {
        if (program(inputs[index])) {
            if (inputs[index] == "print") {
                index++;
                if (index < inputs.size() && inputs[index] == "(") {
                    index++;
                } else {
                    std::cout << "( left parentheses is missing." << std::endl;
                    return false;
                }

                bool has_string_or_variable = false;
                if (index < inputs.size() &&
                    (inputs[index][0] == '"' || variables.find(inputs[index]) != variables.end())) { // after "" gets the variables
                    has_string_or_variable = true;
                    index++;
                } else {
                    std::cout << "Unknown identifier." << std::endl;
                    return false;
                }

                while (index < inputs.size() && inputs[index] == ",") {
                    index++;
                    if (index < inputs.size() &&
                        (inputs[index][0] == '"' || variables.find(inputs[index]) != variables.end())) {
                        index++;
                    } else {
                        std::cout << "Unknown identifier." << std::endl;
                        return false;
                    }
                }

                if (index < inputs.size() && inputs[index] == ")") {
                    index++;
                } else {
                    std::cout << ") right parentheses is missing." << std::endl;
                    return false;
                }

                if (index < inputs.size() && inputs[index] == ";") {
                    index++;
                } else {
                    std::cout << "; semicolon is missing." << std::endl;
                    return false;
                }
            } else {
                std::cout << "print is expected." << std::endl;
                return false;

            }
        } else if (variables.find(inputs[index]) != variables.end()) { 
            index++;
            if (index < inputs.size() && inputs[index] == "=") {
                index++;
                while (index < inputs.size() && inputs[index] != ";") {
                    index++;
                }
                if (index < inputs.size() && inputs[index] == ";") {
                    index++;
                } else {
                    std::cout << "; semicolon is missing." << std::endl;
                    return false;
                }
            } else {
                std::cout << "= equal is missing. (I am not sure but also unknown identifier if missing?)" << std::endl;
                return false;
            }
        } else {
            std::cout << "end is expected" << std::endl;
            return false;
        }
    } else {
        std::cout << "Unknown identifier." << std::endl;
        return false;
    }
}


    if (index < inputs.size() && inputs[index] == "end") {
        index++;
    } else {
        std::cout << "end is expected." << std::endl;
        return false;
    }

    if (index < inputs.size() && inputs[index] == ".") {
        index++;
    } else {
        std::cout << ". period is missing." << std::endl;
        return false;
    }

    return true;
}


int main() {
    std::string input;
     std::vector<std::string> tokens;
    std::cout << "Enter program: \n";
    getline(std::cin, input);
    std::regex regex(R"((\".*?\"|\w+|[;,:;=,+*/();.]))"); // same as previous get from Quora
     for (std::sregex_iterator j(input.begin(), input.end(), regex), end_it; j != end_it; ++j) {
        tokens.push_back(j->str());  // sregex_iterator get suggestion from google
    }
    if (parse_input(tokens)) {
        std::cout << "Ready to compile\n";
    } else {
        std::cout << "\n";
    }

    return 0;
}
