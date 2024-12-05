#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <stack>
#include <sstream>

std::vector<std::pair<std::string, std::string>> R;
std::unordered_map<int, std::unordered_map<std::string, std::string>> LRPT;           // LR Parsing Table from h8

void Rules() {
    R = {
        {"<prog>", "program <identifier>; var <dec-list> begin <stat-list> end"},
        {"<identifier>", "<letter> {<letter> | <digit>}"},
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
        {"<letter>", "a|b|c|d|l|f"}
    };
}

void Table() {
    LRPT = { // the table for whole grammars
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

bool program(const std::string& keyword) { // detect different function
    return keyword == "program" || keyword == "var" || keyword == "begin" || keyword == "end" || keyword == "integer" || keyword == "print";
}

bool parse_input(const std::vector<std::string>& inputs) {
    std::stack<int> stateStack;
    std::stack<std::string> symbolStack;
    stateStack.push(0);

    int index = 0;
    bool error = false;
    bool Detected = false;

    while (index < inputs.size()) {
        int State = stateStack.top();
        std::string a = inputs[index];

        if (LRPT[State].find(a) != LRPT[State].end()) {
            std::string action = LRPT[State][a];

            if (action[0] == 'S') {
                stateStack.push(stoi(action.substr(1)));
                symbolStack.push(a);
                index++;
            } else if (action[0] == 'R') {
                int rule = stoi(action.substr(1)) - 1;
                std::string head = R[rule].first;
                std::string body = R[rule].second;
                for (size_t i = 0; i < body.size(); i++) {
                    stateStack.pop();
                    symbolStack.pop();
                }
                symbolStack.push(head);
                State = stateStack.top();
                stateStack.push(stoi(LRPT[State][head]));
            } else if (action == "ACC") {
                std::cout << "Ready to compile" << std::endl;
                return true;
            }
        } else {
            if (a == "program" && !program(a)) {
                std::cout << "This is the list of error\n" << std::endl;
                std::cout << "program is expected." << std::endl;
                error = true;
            }
            else if (a == "var" && !program(a)) {
                std::cout << "This is the list of error\n" << std::endl;
                std::cout << "var is expected." << std::endl;
                error = true;
            }
            else if (a == "begin" && !program(a)) {
                std::cout << "This is the list of error\n" << std::endl;
                std::cout << "begin is expected." << std::endl;
                error = true;
            }
            else if (a == "end" && !program(a)) {
                std::cout << "This is the list of error\n" << std::endl;
                std::cout << "end is expected." << std::endl;
                error = true;
            } else if (a == "integer" && !program(a)) {
                std::cout << "This is the list of error\n" << std::endl;
                std::cout << "integer is expected." << std::endl;
                error  = true;
            } else if (a == "print" && !program(a)) {
                std::cout << "This is the list of error\n" << std::endl;
                std::cout << "print is expected." << std::endl;
                error = true;
            } else {
                std::cout << "This is the list of error\n" << std::endl;
                std::cout << "Unexpected token: " << a << std::endl;
                Detected = true;
                break;
            }
        }

        if (Detected) {
            break;
        }
    }

    return !Detected;
}


 int main() {
    Rules();
    Table();

    std::string input;
    std::cout << "Enter program: ";
    getline(std::cin, input);
    std::stringstream ss(input);
    std::vector<std::string> tokens;
    std::string token;
    while (ss >> token) {
        tokens.push_back(token);
    }

    parse_input(tokens);
    return 0;
}