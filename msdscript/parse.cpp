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
#include "val.h"
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
    return new NumExpr(n);
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
        std::string keyword = parse_keyword(in);
        if (keyword == "_let")
            return parse_let(in);
        else if(keyword == "_false")
            return new BoolExpr(false);
        else if(keyword == "_true")
            return new BoolExpr(true);
        else if(keyword == "_if")
            return parse_if(in);
        else
           throw std::runtime_error("invalid input");
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
        return new MultExpr(e, rhs);
    }
    else
        return e;
}

Expr *parse_comparg(std::istream &in){
    Expr *e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    } else
        return e;
}

static Expr *parse_expr(std::istream &in) {
    Expr *e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
        int c = in.peek();
        if (c == '=') {
            consume(in, '=');
            Expr *rhs = parse_expr(in);
            return new EqExpr(e, rhs);
        }
        throw std::runtime_error("invalid comparg");
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
    while (isalpha(c) == true) {
        consume(in, c);
        variable += c;
        c = in.peek();
    }
    return new VarExpr(variable);
}

Expr *parse_let(std::istream &in){
    Expr *var;
    Expr *rhs;
    Expr *body;
    // _let keyword was checked in multicand already
    var = parse_var(in);
    if(parse_keyword(in) == "=")
        rhs = parse_expr(in);
    else
        throw std::runtime_error("expecting =");
    if(parse_keyword(in) == "_in")
        body = parse_expr(in);
    else
        throw std::runtime_error("expecting _in");
    return new LetExpr(var->to_string(), rhs, body);
}

std::string parse_keyword(std::istream &in){
    std::string keyword = "";
    skip_whitespace(in);
    int c = in.peek();
    if(c == '='){
        consume(in, '=');
        keyword += '=';
        return keyword;
    }
    else if (c == '_'){
        consume(in, '_');
        keyword += '_';
    }
    c = in.peek();
    while(isalpha(c)){
        consume(in, c);
        keyword += c;
        c = in.peek();
    }
    return keyword;
}

Expr *parse_if(std::istream &in){
    Expr *_if;
    Expr *_then;
    Expr *_else;
    _if = parse_expr(in);
    if(parse_keyword(in) == "_then")
        _then = parse_expr(in);
    else
        throw std::runtime_error("expecting _then");
    if(parse_keyword(in) == "_else")
        _else = parse_expr(in);
    else
        throw std::runtime_error("expecting _else");
    return new IfExpr(_if, _then, _else);
}




TEST_CASE ("parse_interp") {
    std::string addition = "24+8";
    std::istringstream in_stream(addition);
    CHECK(parse(in_stream)->interp()->equals(new NumVal(32)));
    CHECK(parse_str("1+4")->interp()->equals(new NumVal(5)));
    CHECK(parse_str("-1+4")->interp()->equals(new NumVal(3)));
    CHECK(parse_str("_let x=6 _in x*3")->interp()->equals(new NumVal(18)));
    CHECK(parse_str("(5*(_let x=5 _in (x+1)))")->interp()->equals(new NumVal(30)));
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "missing close parenthesis");
    CHECK_THROWS_WITH(parse_str("! % (3"), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_lets x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_let x-5 _in ((_let y=3 _in (y+2))+x)")->interp(), "expecting =");
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _ins (y+2))+x)")->interp(), "expecting _in");
    CHECK_THROWS_WITH(parse_str("(_for x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(consume(in_stream, '2'), "consume mismatch");
    CHECK(parse_str("8")->interp()->equals(new NumVal(8)));
    CHECK(parse_str("_let woof=6 _in woof*3")->interp()->equals(new NumVal(18)));
    CHECK_THROWS_WITH((parse_str("_let same = 1 = 2 _in _if 1 ==2 _then _false +5 _else 88"))->interp(),"invalid comparg");
    CHECK_THROWS_WITH((parse_str("_let same = 1 == 2 _in _if 1 ==2 _true _false +5 _else 88"))->interp(),"expecting _then");
    CHECK_THROWS_WITH((parse_str("_let same = 1 == 2 _in _if 1 ==2 _then _false +5 _elses 88"))->interp(),"expecting _else");
    CHECK(parse_str("(1+3) == 4")->interp()->equals(new BoolVal(true)));
}

TEST_CASE ("parse_interp_spacing") {
    CHECK(parse_str("   (    1  +   2     )    +   3    ")->interp()->equals(new NumVal(6)));
    CHECK(parse_str(" 3 + 7 + _let x = 3        _in  _let x = 5             _in  x + _let x = 5                      _in  x * 1")->interp()->equals(new NumVal(20)));
    CHECK(parse_str("   (    1  *   2     )    +  ( 3 *4  ) ")->interp()->equals(new NumVal(14)));
    CHECK(parse_str("     24")->interp()->equals(new NumVal(24)));
}

TEST_CASE ("parse_print") {
    CHECK(parse_str("5   *    (   _let   x=  5 _in x   ) +1")->to_string() == "((5*(_let x=5 _in x))+1)");
    CHECK(parse_str("(1+   x)  *   ( y * 4 )")->to_string() == "((1+x)*(y*4))");
    CHECK(parse_str(" 1 + 2 + 3 ")->to_string() == "(1+(2+3))");
    CHECK(parse_str(" (1 + 2) + 3 ")->to_string() == "((1+2)+3)");
    CHECK_THROWS_WITH(parse_str("((2+2)+(1+3)")->to_string(), "missing close parenthesis");
    CHECK(parse_str("     24")->to_string() == "24");
    CHECK(parse_str("_false")->to_string() == "_false");
    CHECK(parse_str("_true")->to_string() == "_true");
    CHECK(parse_str("_let same = 1 == 2 _in _if 1 ==2 _then _false +5 _else 88")->to_string() == "(_let same=(1==2) _in (_if (1==2) _then (_false+5) _else 88))");

    
}


TEST_CASE ("parse_pretty_print") {
    CHECK(parse_str("(   _let    x    =   5   _in   x) +     1")->to_pretty_string() == "(_let x = 5\n _in  x) + 1");
    CHECK(parse_str("7+ _let    x = 3   _in  _let x=5    _in  x + _let x= 5  _in  x * 1")->to_pretty_string() == "7 + _let x = 3\n    _in  _let x = 5\n         _in  x + _let x = 5\n                  _in  x * 1");
    CHECK(parse_str("((((x+4))))*3*4       ")->to_pretty_string() == "(x + 4) * 3 * 4");
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _in (y+2))+x)")->to_pretty_string(), "missing close parenthesis");
    CHECK_THROWS_WITH(parse_str(" 3 + 7 + _let x = 3        _in  _lets x = 5             _in  x + _let x = 5                      _in  x * 1")->to_pretty_string(), "invalid input");
    CHECK(parse_str("     8")->to_string() == "8");
}
