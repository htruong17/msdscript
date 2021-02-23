//
//  testgen.cpp
//  test_msdscript
//
//  Created by Harold Truong on 2/22/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "testgen.h"


std::string blank_space() {
    if (rand() % 5 == 1)
        return std::string(rand() % 10, ' ');
    else
        return "";
}

std::string parentheses(std::string input) {
    if (rand() % 10 == 1)
        return "(" + input + ")";
    else
        return input;
}

std::string negative(std::string input) {
    if (rand() % 10 == 1)
        return "-" + input;
    else
        return input;
}

std::string random_var_string() {
    std::string letter = "";
    for (int i = 0; i < (rand() % 20) + 1; i++){
        int letter_case = 0;
        if (rand() % 2 == 0)
            letter_case = 65;
        else
            letter_case = 97;
        letter += (char)((rand() % 26) + letter_case);
    }
    return letter;
}

std::string random_var() {
    std::string letter = "";
    letter += (char)((rand() % 26) + 97);
    return letter;
}


std::string random_let_string() {
    return "_let " + blank_space() + random_var() + blank_space() + "=" + blank_space() + random_expr_string() + blank_space() + " _in " + blank_space() + random_expr_string();
}

std::string random_expr_string() {
    if (rand() % 20 < 12)
        return negative(std::to_string(rand()));
    else if (rand() % 20 == 12)
        return parentheses(random_var());
    else if (rand() % 20 == 13)
        return parentheses(random_var_string());
    else if (rand() % 20 == 14 || rand() % 20 == 15){
        return parentheses(random_expr_string()) + blank_space() + "*" + blank_space() + parentheses(random_expr_string());
    }
    else if (rand() % 20 == 16 || rand() % 20 == 17){
        return random_let_string();
    }
    else
        return parentheses(random_expr_string()) + blank_space() + "+" + blank_space() +  parentheses(random_expr_string());
}


