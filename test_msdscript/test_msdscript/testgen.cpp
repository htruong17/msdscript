//
//  testgen.cpp
//  test_msdscript
//
//  Created by Harold Truong on 2/22/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "testgen.h"

// Randomly add blank spaces
std::string blank_space() {
    if (rand() % 5 == 1)
        return std::string(rand() % 10, ' ');
    else
        return "";
}

// Randomly include parentheses
std::string parentheses(std::string input) {
    if (rand() % 10 == 1)
        return "(" + input + ")";
    else
        return input;
}

// Randomly turns a number string negative
std::string negative(std::string input) {
    if (rand() % 10 == 1)
        return "-" + input;
    else
        return input;
}

// Use to get a long string of variables
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

// Use to get just one variable
std::string random_var() {
    std::string letter = "";
    letter += (char)((rand() % 26) + 97);
    return letter;
}

// Helper function to create _let string
std::string random_let_string() {
    return "_let " + blank_space() + random_var() + blank_space() + "=" + blank_space() + random_expr_string() + blank_space() + " _in " + blank_space() + random_expr_string();
}

std::string random_if_string(){
    return "_if " + blank_space() + random_eq_string() + blank_space() + "_then " + blank_space() + random_expr_string() + blank_space() + " _else " + blank_space() + random_expr_string();
}

std::string random_eq_string(){
    return random_expr_string() + "==" + random_expr_string();
}

std::string random_bool(){
    if (rand() % 2 == 0)
        return "_false";
    else
        return "_true";
}

// Expression function that mixes number, variable, _let, add, and mult to form a collective expression
std::string random_expr_string() {
    if (rand() % 30 < 18)
        return negative(std::to_string(rand()));
    else if (rand() % 30 == 18)
        return parentheses(random_var());
    else if (rand() % 30 == 19)
        return parentheses(random_var_string());
    else if (rand() % 30 == 20 || rand() % 30 == 21){
        return parentheses(random_expr_string()) + blank_space() + "*" + blank_space() + parentheses(random_expr_string());
    }
    else if (rand() % 30 == 22 || rand() % 30 == 23){
        return random_let_string();
    }
    else if (rand() % 30 == 24 || rand() % 30 == 25){
        return parentheses(random_bool());
    }
    else if (rand() % 30 == 26 || rand() % 30 == 27){
        return random_if_string();
    }
    else
        return parentheses(random_expr_string()) + blank_space() + "+" + blank_space() +  parentheses(random_expr_string());
}
