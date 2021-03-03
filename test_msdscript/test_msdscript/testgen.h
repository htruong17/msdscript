//
//  testgen.hpp
//  test_msdscript
//
//  Created by Harold Truong on 2/22/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef testgen_h
#define testgen_h

#include <stdio.h>
#include <iostream>


std::string random_expr_string();
std::string random_var_string();
std::string random_let_string();
std::string random_if_string();
std::string random_eq_string();
std::string random_bool();
std::string random_var();
std::string blank_space();
std::string parentheses();
std::string negative();

#endif /* testgen_hpp */
