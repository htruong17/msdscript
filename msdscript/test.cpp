//
//  testing.cpp
//  msdscript
//
//  Created by Harold Truong on 3/3/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

//#include <stdio.h>
#include "pointer.h"
#include "expr.h"
#include "catch.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "val.h"
#include "env.h"

// Different Exprs for testing purposes
PTR(NumExpr) null = NULL;
PTR(NumExpr) num1 = NEW(NumExpr)(1);
PTR(NumExpr) num2 = NEW(NumExpr)(2);
PTR(NumExpr) num3 = NEW(NumExpr)(3);
PTR(NumExpr) num4 = NEW(NumExpr)(4);
PTR(VarExpr) varx = NEW(VarExpr)("x");
PTR(VarExpr) vary = NEW(VarExpr)("y");


TEST_CASE ("equals") {
    CHECK( (NEW(NumExpr)(7))->equals(NEW(NumExpr)(7)) == true);
    CHECK( (NEW(NumExpr)(7))->equals(NEW(NumExpr)(6)) == false);
    CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("x")) == true);
    CHECK( (NEW(VarExpr)("x"))->equals(NEW(VarExpr)("y")) == false);
    CHECK( (NEW(AddExpr)(num1,num4))->equals(NEW(AddExpr)(num2,num3)) == false);
    CHECK( (NEW(AddExpr)(num1,num2))->equals(NEW(AddExpr)(num1,num2)) == true);
    CHECK( (NEW(MultExpr)(num1,num4))->equals(NEW(MultExpr)(num2,num3)) == false);
    CHECK( (NEW(MultExpr)(num1,num2))->equals(NEW(MultExpr)(num1,num2)) == true);
    CHECK( (NEW(AddExpr)(num1,varx))->equals(NEW(AddExpr)(num2,varx)) == false);
    CHECK( (NEW(AddExpr)(num1,vary))->equals(NEW(AddExpr)(num1,vary)) == true);
    CHECK( (NEW(NumExpr)(7))->equals(null) == false);
    CHECK( (NEW(AddExpr)(num1,num4))->equals(null) == false);
    CHECK( (NEW(VarExpr)("x"))->equals(null) == false);
    CHECK( (NEW(MultExpr)(num1,num4))->equals(null) == false);
    CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->equals((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))) == true);
    CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->equals((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("y", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))) == false);
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->equals(null) == false);
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->equals(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == true);
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->equals(NEW(LetExpr)("y", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))) == false);

}

TEST_CASE ("interp") {
    CHECK( (NEW(NumExpr)(7))->interp(Env::empty)->equals(NEW(NumVal)(7)));
    CHECK( (NEW(AddExpr)(num1,num2))->interp(Env::empty)->equals(NEW(NumVal)(3)));
    CHECK( (NEW(MultExpr)(num1,num2))->interp(Env::empty)->equals(NEW(NumVal)(2)));
    CHECK_THROWS_WITH( (NEW(VarExpr)("x"))->interp(Env::empty), "free variable: x");

    CHECK((NEW(AddExpr)(NEW(NumExpr)(3),(NEW(MultExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->interp(Env::empty)->equals(NEW(NumVal)(73)));

    CHECK((NEW(AddExpr)(NEW(NumExpr)(3),(NEW(AddExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->interp(Env::empty)->equals(NEW(NumVal)(20)));
    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)( NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)( NEW(VarExpr)("y"), NEW(NumExpr)(2) )), NEW(VarExpr)("x"))))->interp(Env::empty)->equals(NEW(NumVal)(10)));


     CHECK((NEW(AddExpr)((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")))), NEW(NumExpr)(1)))->interp(Env::empty)->equals(NEW(NumVal)(26)));


     CHECK((NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))->interp(Env::empty) ->equals(NEW(NumVal)(6)));


     CHECK((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->interp(Env::empty)->equals(NEW(NumVal)(30)));


     CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->interp(Env::empty)->equals(NEW(NumVal)(6)));

    CHECK((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(NumExpr)(11), NEW(NumExpr)(1))))->interp(Env::empty)->equals(NEW(NumVal)(12)));

    CHECK_THROWS_WITH((NEW(AddExpr)((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("y")))), NEW(NumExpr)(1)))->interp(Env::empty), "free variable: y");

    CHECK_THROWS_WITH((NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("z"), NEW(NumExpr)(1))))->interp(Env::empty), "free variable: z");

}


TEST_CASE ("print") {
    std::ostringstream ss;

    (NEW(AddExpr)(num1,NEW(AddExpr)(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1+(2+3))");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(NEW(AddExpr)(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)+3)");
    ss.str("");
    ss.clear();

    CHECK( (NEW(AddExpr)(num4,NEW(AddExpr)(num1,num3)))->to_string() == "(4+(1+3))");
    CHECK( (NEW(AddExpr)((NEW(AddExpr)(num2,num2)),NEW(AddExpr)(num1,num3)))->to_string() == "((2+2)+(1+3))");

    (NEW(MultExpr)(num1,NEW(MultExpr)(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1*(2*3))");
    ss.str("");
    ss.clear();

    (NEW(MultExpr)(NEW(AddExpr)(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)*3)");
    ss.str("");
    ss.clear();

    CHECK( (NEW(MultExpr)(num4,NEW(AddExpr)(num1,num3)))->to_string() == "(4*(1+3))");
    CHECK( (NEW(MultExpr)((NEW(MultExpr)(num2,num4)),NEW(AddExpr)(num1,num3)))->to_string() == "((2*4)*(1+3))");

    (NEW(MultExpr)(NEW(AddExpr)(num1,num4),NEW(MultExpr)(num3,num4)))->print(ss);
    CHECK(ss.str() == "((1+4)*(3*4))");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(NEW(AddExpr)(num1,varx),num3))->print(ss);
    CHECK(ss.str() == "((1+x)+3)");
    ss.str("");
    ss.clear();

    (NEW(MultExpr)(NEW(AddExpr)(num1,varx),NEW(MultExpr)(vary,num4)))->print(ss);
    CHECK(ss.str() == "((1+x)*(y*4))");
    ss.str("");
    ss.clear();

    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)( NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)( NEW(VarExpr)("y"), NEW(NumExpr)(2) )), NEW(VarExpr)("x"))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
     ss.str("");
     ss.clear();

     (NEW(AddExpr)((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")))), NEW(NumExpr)(1)))->print(ss);
     CHECK(ss.str() == "((5*(_let x=5 _in x))+1)");
     ss.str("");
     ss.clear();

     (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))->print(ss);
     CHECK(ss.str() == "((_let x=5 _in x)+1)");
     ss.str("");
     ss.clear();

    (NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->print(ss);
     CHECK(ss.str() == "(5*(_let x=5 _in (x+1)))");
     ss.str("");
     ss.clear();

     (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in (x+1))");
     ss.str("");
     ss.clear();

    (NEW(AddExpr)(NEW(NumExpr)(3),(NEW(MultExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7*(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(NEW(NumExpr)(3),(NEW(AddExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7+(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();
}

TEST_CASE ("pretty_print") {
    std::ostringstream ss;


    CHECK((NEW(VarExpr)("x"))->to_pretty_string() == "x");
    CHECK((NEW(NumExpr)(5))->to_pretty_string() == "5");
    CHECK((NEW(AddExpr)(num1,NEW(AddExpr)(num2,num3)))->to_pretty_string() == "1 + 2 + 3");

    (NEW(AddExpr)(NEW(AddExpr)(num1,num2),num3))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 2) + 3");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(num1,NEW(MultExpr)(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * 3");
    ss.str("");
    ss.clear();

    (NEW(MultExpr)(num1,NEW(AddExpr)(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 * (2 + 3)");
    ss.str("");
    ss.clear();

    (NEW(MultExpr)(NEW(MultExpr)(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "(2 * 3) * 4");
    ss.str("");
    ss.clear();

    (NEW(MultExpr)(num2,NEW(MultExpr)(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 * 4");
    ss.str("");
    ss.clear();

    (NEW(MultExpr)(NEW(AddExpr)(num1,num4),NEW(MultExpr)(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 4) * 3 * 4");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(num1,NEW(MultExpr)(num2,vary)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * y");
    ss.str("");
    ss.clear();


    (NEW(MultExpr)(NEW(AddExpr)(varx,num4),NEW(MultExpr)(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(x + 4) * 3 * 4");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(NEW(MultExpr)(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 + 4");
    ss.str("");
    ss.clear();

    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)( NEW(LetExpr)("y", NEW(NumExpr)(3), NEW(AddExpr)( NEW(VarExpr)("y"), NEW(NumExpr)(2) )), NEW(VarExpr)("x"))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)((NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")))), NEW(NumExpr)(1)))->pretty_print(ss);
    CHECK(ss.str() == "5 * (_let x = 5\n     _in  x) + 1");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(VarExpr)("x")), NEW(NumExpr)(1)))->pretty_print(ss);
    CHECK(ss.str() == "(_let x = 5\n _in  x) + 1");
    ss.str("");
    ss.clear();

   (NEW(MultExpr)(NEW(NumExpr)(5), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1)))))->pretty_print(ss);
    CHECK(ss.str() == "5 * _let x = 5\n    _in  x + 1");
    ss.str("");
    ss.clear();

    (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  x + 1");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(NEW(NumExpr)(3),(NEW(MultExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->pretty_print(ss);
    CHECK(ss.str() == "3 + 7 * _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1");
    ss.str("");
    ss.clear();

    (NEW(AddExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))->pretty_print(ss);
    CHECK(ss.str() == "7 + _let x = 3\n    _in  _let x = 5\n         _in  x + _let x = 5\n                  _in  x * 1");

    ss.str("");
    ss.clear();


    (NEW(AddExpr)(NEW(NumExpr)(3),(NEW(AddExpr)(NEW(NumExpr)(7),(NEW(LetExpr)("x", NEW(NumExpr)(3), (NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(AddExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("x", NEW(NumExpr)(5), NEW(MultExpr)(NEW(VarExpr)("x"), NEW(NumExpr)(1))))))))))))->pretty_print(ss);
    CHECK(ss.str() == "3 + 7 + _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1");
    ss.str("");
    ss.clear();
}


TEST_CASE ("boolean") {
    CHECK((NEW(BoolExpr)(false))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK((NEW(BoolExpr)(true))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(BoolExpr)(true))->equals(NEW(BoolExpr)(true)));
    CHECK((NEW(BoolExpr)(true))->equals(NULL) == false);
    CHECK((NEW(BoolExpr)(true))->to_string() == "_true");
    CHECK((NEW(BoolExpr)(false))->to_string() == "_false");

    CHECK((NEW(BoolExpr)(true))->to_pretty_string() == "_true");
    CHECK((NEW(BoolExpr)(false))->to_pretty_string() == "_false");

    CHECK_THROWS_WITH((NEW(AddExpr)(NEW(NumExpr)(6),NEW(BoolExpr)(true)))->interp(Env::empty), "add of non-number");

    CHECK_THROWS_WITH((NEW(MultExpr)(NEW(NumExpr)(6),NEW(BoolExpr)(true)))->interp(Env::empty), "mult of non-number");

    CHECK_THROWS_WITH((NEW(AddExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(6)))->interp(Env::empty), "cannot add with boolean");
    CHECK_THROWS_WITH((NEW(MultExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(6)))->interp(Env::empty), "cannot multiply with boolean");




};

TEST_CASE ("equal interp") {
    CHECK((NEW(EqExpr)(NEW(NumExpr)(8),NEW(NumExpr)(8)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(NumExpr)(24),NEW(NumExpr)(8)))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(true),NEW(BoolExpr)(true)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(false),NEW(BoolExpr)(true)))->interp(Env::empty)->equals(NEW(BoolVal)(false)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(false),NEW(BoolExpr)(false)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));
    CHECK((NEW(EqExpr)(NEW(BoolExpr)(false),NEW(BoolExpr)(false)))->interp(Env::empty)->equals(NEW(BoolVal)(true)));

};

TEST_CASE ("eqExpr") {
    CHECK((NEW(EqExpr)(NEW(NumExpr)(8),NEW(NumExpr)(8)))->equals((NEW(EqExpr)(NEW(NumExpr)(8),NEW(NumExpr)(8)))));
    CHECK((NEW(EqExpr)(NEW(NumExpr)(8),NEW(NumExpr)(8)))->equals(NULL) == false);

};

TEST_CASE ("IfExp") {


    std::ostringstream ss;

    CHECK((NEW(LetExpr)("same", NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88)  )))->interp(Env::empty)->equals(NEW(NumVal)(88)));
    CHECK((NEW(LetExpr)("same", NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5)),NEW(NumExpr)(88)  )))->interp(Env::empty)->equals(NEW(NumVal)(88)));
    //CHECK((NEW(BoolExpr)(true))->interp(Env::empty)->equals(NEW(BoolVal)(true)));

    CHECK((NEW(LetExpr)("same", NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88)  )))->to_string() == "(_let same=(1==2) _in (_if (1==2) _then (_false+5) _else 88))");
    ss.str("");
    ss.clear();

    CHECK((NEW(LetExpr)("same", NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88)  )))->to_pretty_string() == "_let same = 1 == 2\n_in  _if 1 == 2\n     _then _false + 5\n     _else 88");
    ss.str("");
    ss.clear();

    CHECK((NEW(AddExpr)((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88))), NEW(NumExpr)(2)))->to_pretty_string() == "(_if 1 == 2\n _then _false + 5\n _else 88) + 2");

    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88)))->to_pretty_string() == "_if 1 == 2\n_then _false + 5\n_else 88");

    CHECK((NEW(MultExpr)((NEW(IfExpr)(NEW(AddExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(NumExpr)(24)), NEW(AddExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5)),NEW(NumExpr)(88))), NEW(NumExpr)(2)))->to_pretty_string() == "(_if (1 == 2) + 24\n _then _true + 5\n _else 88) * 2");


    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88)))->equals((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88)))));

    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5)),NEW(NumExpr)(88)))->equals((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(false), NEW(NumExpr)(5)),NEW(NumExpr)(88)))) == false);

    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(2)), NEW(AddExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(5)),NEW(NumExpr)(88)))->equals(NULL) == false);

    CHECK((NEW(IfExpr)(NEW(EqExpr)(NEW(NumExpr)(1),NEW(NumExpr)(1)), NEW(NumExpr)(5),NEW(NumExpr)(88)))->interp(Env::empty)->equals(NEW(NumVal)(5)));

    CHECK_THROWS_WITH(((NEW(IfExpr)(NEW(AddExpr)(NEW(NumExpr)(1),NEW(NumExpr)(1)), NEW(NumExpr)(5),NEW(NumExpr)(88)))->interp(Env::empty)->equals(NEW(NumVal)(5))), "If is a non-boolean value");

    //std::cout << "Hello" << std::endl;

    CHECK((NEW(IfExpr)(NEW(VarExpr)("x"), NEW(BoolExpr)(true), NEW(LetExpr)("Y", NEW(LetExpr)("F", NEW(NumExpr)(17), NEW(EqExpr)(NEW(MultExpr)(NEW(VarExpr)("S"), NEW(NumExpr)(24)), NEW(NumExpr)(17))), NEW(BoolExpr)(true))))->to_pretty_string() == "_if x\n_then _true\n_else _let Y = _let F = 17\n               _in  S * 24 == 17\n      _in  _true");

    CHECK((NEW(IfExpr)(NEW(VarExpr)("x"), NEW(IfExpr)(NEW(VarExpr)("x"), NEW(VarExpr)("y"), NEW(MultExpr)(NEW(NumExpr)(17), NEW(NumExpr)(24))), NEW(EqExpr)(NEW(IfExpr)(NEW(EqExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(17)), NEW(IfExpr)(NEW(VarExpr)("x"), NEW(LetExpr)("c", NEW(NumExpr)(17), NEW(MultExpr)(NEW(NumExpr)(17), NEW(NumExpr)(24))), NEW(IfExpr)(NEW(NumExpr)(17), NEW(MultExpr)(NEW(VarExpr)("G"), NEW(NumExpr)(24)), NEW(BoolExpr)(true))), NEW(AddExpr)(NEW(BoolExpr)(true), NEW(NumExpr)(24))), NEW(VarExpr)("y"))))->to_pretty_string() == "_if x\n_then _if x\n      _then y\n      _else 17 * 24\n_else (_if _true == 17\n       _then _if x\n             _then _let c = 17\n                   _in  17 * 24\n             _else _if 17\n                   _then G * 24\n                   _else _true\n       _else _true + 24) == y");
};

TEST_CASE ("Let Fun Interp") {
    CHECK((NEW(LetExpr)("f", NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x"))), NEW(CallExpr)(NEW(VarExpr)("f"), NEW(NumExpr)(2))))->interp(Env::empty)->equals(NEW(NumVal)(4)));
}

TEST_CASE ("FunExpr Tests") {
    CHECK((NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x"))))->equals(NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x")))));

    CHECK((NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x"))))->equals(NULL) == false);


    CHECK((NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x"))))->to_string() == "(_fun (x) (x*x))");

    CHECK((NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x"))))->to_pretty_string() == "_fun (x)\n  x * x");

    CHECK((NEW(AddExpr)((NEW(FunExpr)("x", NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x")))), NEW(NumExpr)(2)))->to_pretty_string() == "(_fun (x)\n   x * x) + 2");





}

TEST_CASE ("CallExpr Tests") {

    CHECK((NEW(CallExpr)(NEW(VarExpr)("f"), NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x"))))->equals(NULL) == false);

    CHECK((NEW(CallExpr)(NEW(VarExpr)("f"), NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x"))))->equals(NEW(CallExpr)(NEW(VarExpr)("f"), NEW(MultExpr)(NEW(VarExpr)("x"),NEW(VarExpr)("x")))));

    CHECK((NEW(CallExpr)(NEW(VarExpr)("factrl"), NEW(NumExpr)(10)))->to_string() == "factrl(10)");

    CHECK((NEW(CallExpr)(NEW(VarExpr)("factrl"), NEW(NumExpr)(10)))->to_pretty_string() == "factrl(10)");

//    CHECK((NEW(CallExpr)((NEW(CallExpr)(NEW(VarExpr)("factrl"), NEW(NumExpr)(10))),NEW(VarExpr)("factrl") ))->to_pretty_string() == "factrl(10)");

}

