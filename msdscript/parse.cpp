//
//  parse.cpp
//  msdscript
//
//  Created by Harold Truong on 2/15/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//
#include "catch.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "parse.h"
//#include "expr.h"

static void consume(std::istream &in, int expect){
    int c = in.get();
    if(c != expect)
        throw std::runtime_error("consume mismatch");
}

static void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}

Expr *parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;
    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }
    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n*10 + (c - '0');
            
        } else
            break;
    }
    if (negative)
        n = -n;
    return new Num(n);
}

Expr *parse_multicand(std::istream &in){
    skip_whitespace(in);

    int c = in.peek();
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    }
    else if (isalpha(c))
        return parse_var(in);
    else if(c == '_'){
        return parse_let(in);
    }
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

Expr *parse_addend(std::istream &in){
    Expr *e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new Mult(e, rhs);
    } else
        return e;
}

static Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    } else
        return e;
}

Expr *parse_str(std::string s){
    std::istringstream str(s);
    return parse_expr(str);
}

Expr *parse(std::istream &in){
    return parse_expr(in);
}

Expr *parse_var(std::istream &in){
    skip_whitespace(in);
    std::string variable = "";
    int c = in.peek();
    if (isalpha(c)) {
        consume(in, c);
        variable += c;
    }
    return new Var(variable);
}

Expr *parse_let(std::istream &in){
    Expr *var;
    Expr *rhs;
    Expr *body;
    if(parse_keyword(in) == true){
        var = parse_var(in);
    }
    else
        throw std::runtime_error("invalid input");
    if(parse_keyword(in) == true)
        rhs = parse_expr(in);
    else
        throw std::runtime_error("invalid input");
    if(parse_keyword(in) == true)
        body = parse_expr(in);
    else
        throw std::runtime_error("invalid input");
    return new _let(var->to_string(), rhs, body);
}

bool parse_keyword(std::istream &in){
    std::string keyword;
    skip_whitespace(in);
    int c = in.peek();
    if(c == '='){
        consume(in, '=');
        return true;
    }
    else if (c == '_'){
        consume(in, '_');
        c = in.peek();
        if (c == 'l')
            keyword = "let ";
        else if (c == 'i')
            keyword = "in ";
        else
            return false;
        for (int i=0; i < keyword.size(); i++){
            c = in.peek();
            if(c == keyword[i]){
                consume(in, keyword[i]);
            } else {
                return false;
            }
        }
        return true;
    } else
        return false;
}


TEST_CASE ("parse_interp") {
    std::string addition = "24+8";
    std::istringstream in_stream(addition);
    CHECK(parse(in_stream)->interp() == 32);
    CHECK(parse_str("1+4")->interp() == 5);
    CHECK(parse_str("-1+4")->interp() == 3);
    CHECK(parse_str("_let x=6 _in x*3")->interp() == 18);
    CHECK(parse_str("(5*(_let x=5 _in (x+1)))")->interp() == 30);
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "missing close parenthesis");
    CHECK_THROWS_WITH(parse_str("! % (3"), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_lets x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_let x-5 _in ((_let y=3 _in (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _ins (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_for x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(consume(in_stream, '2'), "consume mismatch");
    CHECK(parse_str("8")->interp() == 8);
}

TEST_CASE ("parse_interp_spacing") {
    CHECK(parse_str("   (    1  +   2     )    +   3    ")->interp() == 6);
    CHECK(parse_str(" 3 + 7 + _let x = 3        _in  _let x = 5             _in  x + _let x = 5                      _in  x * 1")->interp() == 20);
    CHECK(parse_str("   (    1  *   2     )    +  ( 3 *4  ) ")->interp() == 14);
    CHECK(parse_str("     24")->interp() == 24);
}

TEST_CASE ("parse_print") {
    CHECK(parse_str("5   *    (   _let   x=  5 _in x   ) +1")->to_string() == "((5*(_let x=5 _in x))+1)");
    CHECK(parse_str("(1+   x)  *   ( y * 4 )")->to_string() == "((1+x)*(y*4))");
    CHECK(parse_str(" 1 + 2 + 3 ")->to_string() == "(1+(2+3))");
    CHECK(parse_str(" (1 + 2) + 3 ")->to_string() == "((1+2)+3)");
    CHECK_THROWS_WITH(parse_str("((2+2)+(1+3)")->to_string(), "missing close parenthesis");
    CHECK(parse_str("     24")->to_string() == "24");

    
}


TEST_CASE ("parse_pretty_print") {
    CHECK(parse_str("(   _let    x    =   5   _in   x) +     1")->to_pretty_string() == "(_let x = 5\n _in  x) + 1");
    CHECK(parse_str("7+ _let    x = 3   _in  _let x=5    _in  x + _let x= 5  _in  x * 1")->to_pretty_string() == "7 + _let x = 3\n    _in  _let x = 5\n         _in  x + _let x = 5\n                  _in  x * 1");
    CHECK(parse_str("((((x+4))))*3*4       ")->to_pretty_string() == "(x + 4) * 3 * 4");
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _in (y+2))+x)")->to_pretty_string(), "missing close parenthesis");
    CHECK_THROWS_WITH(parse_str(" 3 + 7 + _let x = 3        _in  _lets x = 5             _in  x + _let x = 5                      _in  x * 1")->to_pretty_string(), "invalid input");
    CHECK(parse_str("     8")->to_string() == "8");
}
