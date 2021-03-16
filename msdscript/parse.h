//
//  parse.hpp
//  msdscript
//
//  Created by Harold Truong on 2/15/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp

#include <stdio.h>
#include "expr.h"

static void consume(std::istream &in, int expect);
static void skip_whitespace(std::istream &in);
Expr *parse_num(std::istream &in);
Expr *parse_addend(std::istream &in);
static Expr *parse_expr(std::istream &in);
Expr *parse_multicand(std::istream &in);
Expr *parse_str(std::string s);
Expr *parse_var(std::istream &in);
Expr *parse_let(std::istream &in);
std::string parse_keyword(std::istream &in);
Expr *parse(std::istream &in);
Expr *parse_if(std::istream &in);
Expr *parse_comparg(std::istream &in);
Expr *parse_inner(std::istream &in);
Expr *parse_fun(std::istream &in);


#endif /* parse_hpp */
