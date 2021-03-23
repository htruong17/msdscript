//
//  parse.hpp
//  msdscript
//
//  Created by Harold Truong on 2/15/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp

#include "pointer.h"
#include <stdio.h>
#include "expr.h"

static void consume(std::istream &in, int expect);
static void skip_whitespace(std::istream &in);
PTR(Expr) parse_num(std::istream &in);
PTR(Expr) parse_addend(std::istream &in);
static PTR(Expr) parse_expr(std::istream &in);
PTR(Expr) parse_multicand(std::istream &in);
PTR(Expr) parse_str(std::string s);
PTR(Expr) parse_var(std::istream &in);
PTR(Expr) parse_let(std::istream &in);
std::string parse_keyword(std::istream &in);
PTR(Expr) parse(std::istream &in);
PTR(Expr) parse_if(std::istream &in);
PTR(Expr) parse_comparg(std::istream &in);
PTR(Expr) parse_inner(std::istream &in);
PTR(Expr) parse_fun(std::istream &in);


#endif /* parse_hpp */
