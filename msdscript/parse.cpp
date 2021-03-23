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

PTR(Expr) parse_num(std::istream &in) {
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
    return NEW(NumExpr)(n);
}

PTR(Expr) parse_inner(std::istream &in){
    skip_whitespace(in);

    int c = in.peek();
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    else if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
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
            return NEW(BoolExpr)(false);
        else if(keyword == "_true")
            return NEW(BoolExpr)(true);
        else if(keyword == "_if")
            return parse_if(in);
        else if(keyword == "_fun"){
            return parse_fun(in);
        }
        else
           throw std::runtime_error("invalid input");
    }
    else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}

PTR(Expr) parse_multicand(std::istream &in){
    PTR(Expr) e;
    e = parse_inner(in);
    skip_whitespace(in);
    int c = in.peek();
    while(c == '(') {
        //skip_whitespace(in);
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        skip_whitespace(in);
        consume(in, ')');
        
        e = NEW(CallExpr)(e, actual_arg);
        c = in.peek();
    }
    return e;
}

PTR(Expr) parse_addend(std::istream &in){
    PTR(Expr) e;
    e = parse_multicand(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        PTR(Expr) rhs = parse_addend(in);
        return NEW(MultExpr)(e, rhs);
    }
    else
        return e;
}

PTR(Expr) parse_comparg(std::istream &in){
    PTR(Expr) e;
    e = parse_addend(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_comparg(in);
        return NEW(AddExpr)(e, rhs);
    } else
        return e;
}

static PTR(Expr) parse_expr(std::istream &in) {
    PTR(Expr) e;
    e = parse_comparg(in);
    skip_whitespace(in);
    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
        int c = in.peek();
        if (c == '=') {
            consume(in, '=');
            PTR(Expr) rhs = parse_expr(in);
            return NEW(EqExpr)(e, rhs);
        }
        throw std::runtime_error("invalid comparg");
    } else
        return e;
}

PTR(Expr) parse_str(std::string s){
    std::istringstream str(s);
    return parse_expr(str);
}

PTR(Expr) parse(std::istream &in){
    return parse_expr(in);
}

PTR(Expr) parse_var(std::istream &in){
    skip_whitespace(in);
    std::string variable = "";
    int c = in.peek();
    while (isalpha(c) == true) {
        consume(in, c);
        variable += c;
        c = in.peek();
    }
    return NEW(VarExpr)(variable);
}

PTR(Expr) parse_let(std::istream &in){
    PTR(Expr) var;
    PTR(Expr) rhs;
    PTR(Expr) body;
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
    return NEW(LetExpr)(var->to_string(), rhs, body);
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
//    if (c == '('){
//        consume(in, '(');
//        keyword += '(';
//    }
//    if (c == ')'){
//        consume(in, ')');
//        keyword += ')';
//    }
    return keyword;
}

PTR(Expr) parse_if(std::istream &in){
    PTR(Expr) _if;
    PTR(Expr) _then;
    PTR(Expr) _else;
    _if = parse_expr(in);
    if(parse_keyword(in) == "_then")
        _then = parse_expr(in);
    else
        throw std::runtime_error("expecting _then");
    if(parse_keyword(in) == "_else")
        _else = parse_expr(in);
    else
        throw std::runtime_error("expecting _else");
    return NEW(IfExpr)(_if, _then, _else);
}

PTR(Expr) parse_fun(std::istream &in){
    PTR(Expr) formal_arg;
    PTR(Expr) body;
    skip_whitespace(in);
    int c = in.peek();
    if(c == '('){
        consume(in, '(');
         formal_arg = parse_var(in);
//    if(parse_keyword(in) == "(")
//        formal_arg = parse_var(in);
    }
    else
        throw std::runtime_error("expecting (");
    skip_whitespace(in);
    c = in.peek();
    if(c == ')'){
    //if(parse_keyword(in) == ")")
        consume(in, ')');
        body = parse_expr(in);
         
    }
    else
        throw std::runtime_error("expecting )");
     skip_whitespace(in);
    return NEW(FunExpr)(formal_arg->to_string(), body);
}

TEST_CASE ("parse_interp") {
    std::string addition = "24+8";
    std::istringstream in_stream(addition);
    CHECK(parse(in_stream)->interp()->equals(NEW(NumVal)(32)));
    CHECK(parse_str("1+4")->interp()->equals(NEW(NumVal)(5)));
    CHECK(parse_str("-1+4")->interp()->equals(NEW(NumVal)(3)));
    CHECK(parse_str("_let x=6 _in x*3")->interp()->equals(NEW(NumVal)(18)));
    CHECK(parse_str("(5*(_let x=5 _in (x+1)))")->interp()->equals(NEW(NumVal)(30)));
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "missing close parenthesis");
    CHECK_THROWS_WITH(parse_str("! % (3"), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_lets x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(parse_str("(_let x-5 _in ((_let y=3 _in (y+2))+x)")->interp(), "expecting =");
    CHECK_THROWS_WITH(parse_str("(_let x=5 _in ((_let y=3 _ins (y+2))+x)")->interp(), "expecting _in");
    CHECK_THROWS_WITH(parse_str("(_for x=5 _in ((_let y=3 _in (y+2))+x)")->interp(), "invalid input");
    CHECK_THROWS_WITH(consume(in_stream, '2'), "consume mismatch");
    CHECK(parse_str("8")->interp()->equals(NEW(NumVal)(8)));
    CHECK(parse_str("_let woof=6 _in woof*3")->interp()->equals(NEW(NumVal)(18)));
    CHECK_THROWS_WITH((parse_str("_let same = 1 = 2 _in _if 1 ==2 _then _false +5 _else 88"))->interp(),"invalid comparg");
    CHECK_THROWS_WITH((parse_str("_let same = 1 == 2 _in _if 1 ==2 _true _false +5 _else 88"))->interp(),"expecting _then");
    CHECK_THROWS_WITH((parse_str("_let same = 1 == 2 _in _if 1 ==2 _then _false +5 _elses 88"))->interp(),"expecting _else");
    CHECK(parse_str("(1+3) == 4")->interp()->equals(NEW(BoolVal)(true)));
}

TEST_CASE ("parse_interp_spacing") {
    CHECK(parse_str("   (    1  +   2     )    +   3    ")->interp()->equals(NEW(NumVal)(6)));
    CHECK(parse_str(" 3 + 7 + _let x = 3        _in  _let x = 5             _in  x + _let x = 5                      _in  x * 1")->interp()->equals(NEW(NumVal)(20)));
    CHECK(parse_str("   (    1  *   2     )    +  ( 3 *4  ) ")->interp()->equals(NEW(NumVal)(14)));
    CHECK(parse_str("     24")->interp()->equals(NEW(NumVal)(24)));
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
    
//    CHECK(parse_str("if 1978213523==895472277 _then 421926333 _else -262170344+_let s=1930413280 _in 535505750       +762586027")->to_pretty_string() == "");
    
    CHECK(parse_str("_let factrl = _fun (factrl)\n                _fun (x)\n                  _if x == 1\n                  _then 1\n                  _else x * factrl(factrl)(x + -1)\n_in  factrl(factrl)(10)")->to_pretty_string() =="_let factrl = _fun (factrl)\n                _fun (x)\n                  _if x == 1\n                  _then 1\n                  _else x * factrl(factrl)(x + -1)\n_in  factrl(factrl)(10)");
    
    
//    CHECK((parse_str("869624221+1806805595")->interp()->to_expr()->to_string())=="");
//    CHECK(parse_str(parse_str("869624221+1806805595")->to_string())->interp()->to_expr()->to_string()=="");
    
}


TEST_CASE ("interp parse_function") {
    CHECK(parse_str("_let f = _fun (x) x _in  f(2)")->interp()->equals(NEW(NumVal)(2)));
    CHECK(parse_str("_let f = _fun (x) x + 1 _in  f(10)")->interp()->equals(NEW(NumVal)(11)));
    CHECK_THROWS_WITH(parse_str("_let f = _fun x) x _in  f(2)")->to_string(), "expecting (");
    CHECK_THROWS_WITH(parse_str("_let f = _fun (x x _in  f(2)")->to_string(), "expecting )");
    CHECK(parse_str("_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in  factrl(factrl)(10)")->interp()->equals(NEW(NumVal)(3628800)));
}

TEST_CASE("Pretty Print Calls"){
    CHECK(parse_str("factrl(factrl)(x + -1)")->to_pretty_string() == "factrl(factrl)(x + -1)");
    
    CHECK(parse_str("(factrl)(factrl)(x + -1)")->to_pretty_string() == "factrl(factrl)(x + -1)");
}

TEST_CASE("More testing"){
    CHECK(parse_str("(_false)")->to_string() == "_false");
    CHECK(parse_str("(_false)")->to_pretty_string() == "_false");
//    CHECK(parse_str("905162409((_fun (i) mwFWMVKE))((_let f=-1873773093 _in 71330000))")->to_pretty_string() == "905162409(_fun (i)\n            mwFWMVKE)(_let f = -1873773093\n                      _in  71330000)");
}


