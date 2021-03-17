//
//  testing.cpp
//  msdscript
//
//  Created by Harold Truong on 3/3/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

//#include <stdio.h>
#include "expr.h"
#include "catch.h"
#include <stdexcept>
#include <iostream>
#include <sstream>
#include "val.h"

// Different Exprs for testing purposes
NumExpr *null = NULL;
NumExpr *num1 = new NumExpr(1);
NumExpr *num2 = new NumExpr(2);
NumExpr *num3 = new NumExpr(3);
NumExpr *num4 = new NumExpr(4);
VarExpr *varx = new VarExpr("x");
VarExpr *vary = new VarExpr("y");


TEST_CASE ("equals") {
    CHECK( (new NumExpr(7))->equals(new NumExpr(7)) == true);
    CHECK( (new NumExpr(7))->equals(new NumExpr(6)) == false);
    CHECK( (new VarExpr("x"))->equals(new VarExpr("x")) == true);
    CHECK( (new VarExpr("x"))->equals(new VarExpr("y")) == false);
    CHECK( (new AddExpr(num1,num4))->equals(new AddExpr(num2,num3)) == false);
    CHECK( (new AddExpr(num1,num2))->equals(new AddExpr(num1,num2)) == true);
    CHECK( (new MultExpr(num1,num4))->equals(new MultExpr(num2,num3)) == false);
    CHECK( (new MultExpr(num1,num2))->equals(new MultExpr(num1,num2)) == true);
    CHECK( (new AddExpr(num1,varx))->equals(new AddExpr(num2,varx)) == false);
    CHECK( (new AddExpr(num1,vary))->equals(new AddExpr(num1,vary)) == true);
    CHECK( (new NumExpr(7))->equals(null) == false);
    CHECK( (new AddExpr(num1,num4))->equals(null) == false);
    CHECK( (new VarExpr("x"))->equals(null) == false);
    CHECK( (new MultExpr(num1,num4))->equals(null) == false);
    CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->equals((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))) == true);
    CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->equals((new MultExpr(new NumExpr(5), new LetExpr("y", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))) == false);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->equals(null) == false);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->equals(new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == true);
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->equals(new LetExpr("y", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))) == false);
    
}

TEST_CASE ("interp") {
    CHECK( (new NumExpr(7))->interp()->equals(new NumVal(7)));
    CHECK( (new AddExpr(num1,num2))->interp()->equals(new NumVal(3)));
    CHECK( (new MultExpr(num1,num2))->interp()->equals(new NumVal(2)));
    CHECK_THROWS_WITH( (new VarExpr("x"))->interp(), "no value for variable");
    
    CHECK((new AddExpr(new NumExpr(3),(new MultExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->interp()->equals(new NumVal(73)));
    
    CHECK((new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->interp()->equals(new NumVal(20)));
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new VarExpr("y"), new NumExpr(2) )), new VarExpr("x"))))->interp()->equals(new NumVal(10)));
     
     
     CHECK((new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x")))), new NumExpr(1)))->interp()->equals(new NumVal(26)));
  
     
     CHECK((new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->interp() ->equals(new NumVal(6)));

     
     CHECK((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->interp()->equals(new NumVal(30)));
 
     
     CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->interp()->equals(new NumVal(6)));
    
    CHECK((new LetExpr("x", new NumExpr(5), new AddExpr(new NumExpr(11), new NumExpr(1))))->interp()->equals(new NumVal(12)));
    
    CHECK_THROWS_WITH((new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("y")))), new NumExpr(1)))->interp(), "no value for variable");
    
    CHECK_THROWS_WITH((new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("z"), new NumExpr(1))))->interp(), "no value for variable");
  
}


TEST_CASE ("subst") {
    CHECK( (new VarExpr("x"))->subst("x", new VarExpr("b"))->equals(new VarExpr("b")) == true);
    CHECK( (new NumExpr(7))->subst("x", new NumExpr(8))->equals(new NumExpr(7)) == true);
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new NumExpr(8))->interp()->equals(new NumVal(15)));
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new VarExpr("y"))->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))));
    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new NumExpr(8))->equals(new AddExpr(new NumExpr(8), new NumExpr(7))) == true);
    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new NumExpr(8))->equals(new MultExpr(new NumExpr(8), new NumExpr(7))) == true);
    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))->subst("x", new VarExpr("y"))->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))) == true);
    CHECK( (new VarExpr("x"))->subst("y", new VarExpr("b"))->equals(new VarExpr("x")) == true);
    CHECK((new LetExpr("x", new AddExpr(new NumExpr(4), new VarExpr("x")), new VarExpr("x")))->subst("x", new VarExpr("y"))->equals(new LetExpr("x", new AddExpr(new NumExpr(4), new VarExpr("y")),new VarExpr("x"))));
    CHECK((new LetExpr("y", new NumExpr(4), new VarExpr("x")))->subst("x", new VarExpr("y"))->equals(new LetExpr("y",  new NumExpr(4) ,new VarExpr("y"))));
    CHECK((new LetExpr("x", new NumExpr(4), new VarExpr("x")))->subst("x", new VarExpr("y"))->equals(new LetExpr("x",  new NumExpr(4) ,new VarExpr("x"))));
    CHECK( (new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new VarExpr("y"))))->subst("y", new NumExpr(8)) ->equals(new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new NumExpr(8)))) );
    CHECK( (new LetExpr("x", new VarExpr("y"), new AddExpr(new VarExpr("x"), new NumExpr(2)))) ->subst("y", new NumExpr(8)) ->equals(new LetExpr("x", new NumExpr(8), new AddExpr(new VarExpr("x"), new NumExpr(2)))) );
    CHECK( (new LetExpr("y", new VarExpr("x"), new VarExpr("x")))->subst("x", new NumExpr(3))->equals(new LetExpr("y", new NumExpr(3), new NumExpr(3))) );
}

TEST_CASE ("print") {
    std::ostringstream ss;
    
    (new AddExpr(num1,new AddExpr(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1+(2+3))");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new AddExpr(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)+3)");
    ss.str("");
    ss.clear();
    
    CHECK( (new AddExpr(num4,new AddExpr(num1,num3)))->to_string() == "(4+(1+3))");
    CHECK( (new AddExpr((new AddExpr(num2,num2)),new AddExpr(num1,num3)))->to_string() == "((2+2)+(1+3))");
    
    (new MultExpr(num1,new MultExpr(num2,num3)))->print(ss);
    CHECK(ss.str() == "(1*(2*3))");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new AddExpr(num1,num2),num3))->print(ss);
    CHECK(ss.str() == "((1+2)*3)");
    ss.str("");
    ss.clear();
    
    CHECK( (new MultExpr(num4,new AddExpr(num1,num3)))->to_string() == "(4*(1+3))");
    CHECK( (new MultExpr((new MultExpr(num2,num4)),new AddExpr(num1,num3)))->to_string() == "((2*4)*(1+3))");
    
    (new MultExpr(new AddExpr(num1,num4),new MultExpr(num3,num4)))->print(ss);
    CHECK(ss.str() == "((1+4)*(3*4))");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new AddExpr(num1,varx),num3))->print(ss);
    CHECK(ss.str() == "((1+x)+3)");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new AddExpr(num1,varx),new MultExpr(vary,num4)))->print(ss);
    CHECK(ss.str() == "((1+x)*(y*4))");
    ss.str("");
    ss.clear();
    
    (new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new VarExpr("y"), new NumExpr(2) )), new VarExpr("x"))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");
     ss.str("");
     ss.clear();

     (new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x")))), new NumExpr(1)))->print(ss);
     CHECK(ss.str() == "((5*(_let x=5 _in x))+1)");
     ss.str("");
     ss.clear();

     (new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->print(ss);
     CHECK(ss.str() == "((_let x=5 _in x)+1)");
     ss.str("");
     ss.clear();

    (new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->print(ss);
     CHECK(ss.str() == "(5*(_let x=5 _in (x+1)))");
     ss.str("");
     ss.clear();

     (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->print(ss);
     CHECK(ss.str() == "(_let x=5 _in (x+1))");
     ss.str("");
     ss.clear();
    
    (new AddExpr(new NumExpr(3),(new MultExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7*(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->print(ss);
    CHECK(ss.str() == "(3+(7+(_let x=3 _in (_let x=5 _in (x+(_let x=5 _in (x*1)))))))");
    ss.str("");
    ss.clear();
}

TEST_CASE ("pretty_print") {
    std::ostringstream ss;
    
    
    CHECK((new VarExpr("x"))->to_pretty_string() == "x");
    CHECK((new NumExpr(5))->to_pretty_string() == "5");
    CHECK((new AddExpr(num1,new AddExpr(num2,num3)))->to_pretty_string() == "1 + 2 + 3");
    
    (new AddExpr(new AddExpr(num1,num2),num3))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 2) + 3");
    ss.str("");
    ss.clear();
    
    (new AddExpr(num1,new MultExpr(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * 3");
    ss.str("");
    ss.clear();
    
    (new MultExpr(num1,new AddExpr(num2,num3)))->pretty_print(ss);
    CHECK(ss.str() == "1 * (2 + 3)");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new MultExpr(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "(2 * 3) * 4");
    ss.str("");
    ss.clear();
    
    (new MultExpr(num2,new MultExpr(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new MultExpr(new AddExpr(num1,num4),new MultExpr(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(1 + 4) * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new AddExpr(num1,new MultExpr(num2,vary)))->pretty_print(ss);
    CHECK(ss.str() == "1 + 2 * y");
    ss.str("");
    ss.clear();
    
    
    (new MultExpr(new AddExpr(varx,num4),new MultExpr(num3,num4)))->pretty_print(ss);
    CHECK(ss.str() == "(x + 4) * 3 * 4");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new MultExpr(num2,num3),num4))->pretty_print(ss);
    CHECK(ss.str() == "2 * 3 + 4");
    ss.str("");
    ss.clear();
    
    (new LetExpr("x", new NumExpr(5), new AddExpr( new LetExpr("y", new NumExpr(3), new AddExpr( new VarExpr("y"), new NumExpr(2) )), new VarExpr("x"))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  (_let y = 3\n      _in  y + 2) + x");
    ss.str("");
    ss.clear();
    
    (new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x")))), new NumExpr(1)))->pretty_print(ss);
    CHECK(ss.str() == "5 * (_let x = 5\n     _in  x) + 1");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->pretty_print(ss);
    CHECK(ss.str() == "(_let x = 5\n _in  x) + 1");
    ss.str("");
    ss.clear();
    
   (new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1)))))->pretty_print(ss);
    CHECK(ss.str() == "5 * _let x = 5\n    _in  x + 1");
    ss.str("");
    ss.clear();
    
    (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))))->pretty_print(ss);
    CHECK(ss.str() == "_let x = 5\n_in  x + 1");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new NumExpr(3),(new MultExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->pretty_print(ss);
    CHECK(ss.str() == "3 + 7 * _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1");
    ss.str("");
    ss.clear();
    
    (new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))->pretty_print(ss);
    CHECK(ss.str() == "7 + _let x = 3\n    _in  _let x = 5\n         _in  x + _let x = 5\n                  _in  x * 1");

    ss.str("");
    ss.clear();

    
    (new AddExpr(new NumExpr(3),(new AddExpr(new NumExpr(7),(new LetExpr("x", new NumExpr(3), (new LetExpr("x", new NumExpr(5), new AddExpr(new VarExpr("x"), new LetExpr("x", new NumExpr(5), new MultExpr(new VarExpr("x"), new NumExpr(1))))))))))))->pretty_print(ss);
    CHECK(ss.str() == "3 + 7 + _let x = 3\n        _in  _let x = 5\n             _in  x + _let x = 5\n                      _in  x * 1");
    ss.str("");
    ss.clear();
}

TEST_CASE( "more subst" ) {
  // _let x = 1
  // _in  x + 2  -> subst("x", y+3)
  // =
  // _let x = 1
  // _in  x + 2
  Expr *let1 = (new LetExpr("x",
                        new NumExpr(1),
                        new AddExpr(new VarExpr("x"), new NumExpr(2))));
  CHECK( let1->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
        ->equals(let1) );

  // _let x = x
  // _in  x + 2  -> subst("x", y+3)
  // =
  // _let x = y+3
  // _in  x + 2
  Expr *let2 = (new LetExpr("x",
                        new VarExpr("x"),
                        new AddExpr(new VarExpr("x"), new NumExpr(2))));
  CHECK( let2->subst("x", new AddExpr(new VarExpr("y"), new NumExpr(3)))
        ->equals(new LetExpr("x",
                         new AddExpr(new VarExpr("y"), new NumExpr(3)),
                         new AddExpr(new VarExpr("x"), new NumExpr(2)))) );
  
  // _let x = y
  // _in  x + 2 -> subst("y", 8)
  // =
  // _let x = 8
  // _in  x + 2
  
  // _let x = 8
  // _in  x + 2 + y -> subst("y", 9)
  // =
  // _let x = 8
  // _in  x + 2 + 9

  // y + y -> subst("y", 8)
  // =
  // 8 + 8

  // _let x = y
  // _in  x + y -> subst("y", 8)
  // =
  // _let x = 8
  // _in  x + 8

  // _let z = x
  // _in  z + 32 -> subst("z", 0)
  // =
  // _let z = x
  // _in  z + 32
  
  // _let z = z
  // _in  z + 32 -> subst("z", 0)
  // =
  // _let z = 0
  // _in  z + 32

  // _let z = z + 2
  // _in  z + 32 -> subst("z", 0)
  // =
  // _let z = 0 + 2
  // _in  z + 32

}

TEST_CASE ("boolean") {
    CHECK((new BoolExpr(false))->interp()->equals(new BoolVal(false)));
    CHECK((new BoolExpr(true))->interp()->equals(new BoolVal(true)));
    CHECK((new BoolExpr(true))->equals(new BoolExpr(true)));
    CHECK((new BoolExpr(true))->equals(NULL) == false);
    CHECK((new BoolExpr(true))->subst("x", new BoolExpr(false)));
    CHECK((new BoolExpr(true))->to_string() == "_true");
    CHECK((new BoolExpr(false))->to_string() == "_false");
    
    CHECK((new BoolExpr(true))->to_pretty_string() == "_true");
    CHECK((new BoolExpr(false))->to_pretty_string() == "_false");
    
    CHECK_THROWS_WITH((new AddExpr(new NumExpr(6),new BoolExpr(true)))->interp(), "add of non-number");
    
    CHECK_THROWS_WITH((new MultExpr(new NumExpr(6),new BoolExpr(true)))->interp(), "mult of non-number");
    
    CHECK_THROWS_WITH((new AddExpr(new BoolExpr(true), new NumExpr(6)))->interp(), "cannot add with boolean");
    CHECK_THROWS_WITH((new MultExpr(new BoolExpr(true), new NumExpr(6)))->interp(), "cannot multiply with boolean");
   
    
    
    
};

TEST_CASE ("equal interp") {
    CHECK((new EqExpr(new NumExpr(8),new NumExpr(8)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new NumExpr(24),new NumExpr(8)))->interp()->equals(new BoolVal(false)));
    CHECK((new EqExpr(new BoolExpr(true),new BoolExpr(true)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new BoolExpr(false),new BoolExpr(true)))->interp()->equals(new BoolVal(false)));
    CHECK((new EqExpr(new BoolExpr(false),new BoolExpr(false)))->interp()->equals(new BoolVal(true)));
    CHECK((new EqExpr(new BoolExpr(false),new BoolExpr(false)))->interp()->equals(new BoolVal(true)));
    
};

TEST_CASE ("eqExpr") {
    CHECK((new EqExpr(new NumExpr(8),new NumExpr(8)))->equals((new EqExpr(new NumExpr(8),new NumExpr(8)))));
    CHECK((new EqExpr(new NumExpr(8),new NumExpr(8)))->equals(NULL) == false);
    CHECK((new EqExpr(new NumExpr(8),new NumExpr(8)))->subst("x", new NumExpr(2))->equals(new EqExpr(new NumExpr(8),new NumExpr(8))));
    
};

TEST_CASE ("IfExp") {
    
    
    std::ostringstream ss;
    
    CHECK((new LetExpr("same", new EqExpr(new NumExpr(1),new NumExpr(2)), new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88)  )))->interp()->equals(new NumVal(88)));
    CHECK((new LetExpr("same", new EqExpr(new NumExpr(1),new NumExpr(2)), new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(true), new NumExpr(5)),new NumExpr(88)  )))->interp()->equals(new NumVal(88)));
    //CHECK((new BoolExpr(true))->interp()->equals(new BoolVal(true)));
    
    CHECK((new LetExpr("same", new EqExpr(new NumExpr(1),new NumExpr(2)), new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88)  )))->to_string() == "(_let same=(1==2) _in (_if (1==2) _then (_false+5) _else 88))");
    ss.str("");
    ss.clear();
    
    CHECK((new LetExpr("same", new EqExpr(new NumExpr(1),new NumExpr(2)), new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88)  )))->to_pretty_string() == "_let same = 1 == 2\n_in  _if 1 == 2\n     _then _false + 5\n     _else 88");
    ss.str("");
    ss.clear();
    
    CHECK((new AddExpr((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88))), new NumExpr(2)))->to_pretty_string() == "(_if 1 == 2\n _then _false + 5\n _else 88) + 2");
    
    CHECK((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88)))->to_pretty_string() == "_if 1 == 2\n_then _false + 5\n_else 88");
    
    CHECK((new MultExpr((new IfExpr(new AddExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new NumExpr(24)), new AddExpr(new BoolExpr(true), new NumExpr(5)),new NumExpr(88))), new NumExpr(2)))->to_pretty_string() == "(_if (1 == 2) + 24\n _then _true + 5\n _else 88) * 2");
    
    
    CHECK((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88)))->equals((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88)))));
    
    CHECK((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(true), new NumExpr(5)),new NumExpr(88)))->equals((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(false), new NumExpr(5)),new NumExpr(88)))) == false);
    
    CHECK((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(2)), new AddExpr(new BoolExpr(true), new NumExpr(5)),new NumExpr(88)))->equals(NULL) == false);
    
    CHECK((new IfExpr(new EqExpr(new NumExpr(1),new NumExpr(1)), new NumExpr(5),new NumExpr(88)))->interp()->equals(new NumVal(5)));
    
    CHECK_THROWS_WITH(((new IfExpr(new AddExpr(new NumExpr(1),new NumExpr(1)), new NumExpr(5),new NumExpr(88)))->interp()->equals(new NumVal(5))), "If is a non-boolean value");
    
    //std::cout << "Hello" << std::endl;
    
    CHECK((new IfExpr(new VarExpr("x"), new BoolExpr(true), new LetExpr("Y", new LetExpr("F", new NumExpr(17), new EqExpr(new MultExpr(new VarExpr("S"), new NumExpr(24)), new NumExpr(17))), new BoolExpr(true))))->to_pretty_string() == "_if x\n_then _true\n_else _let Y = _let F = 17\n               _in  S * 24 == 17\n      _in  _true");
    
    CHECK((new IfExpr(new VarExpr("x"), new IfExpr(new VarExpr("x"), new VarExpr("y"), new MultExpr(new NumExpr(17), new NumExpr(24))), new EqExpr(new IfExpr(new EqExpr(new BoolExpr(true), new NumExpr(17)), new IfExpr(new VarExpr("x"), new LetExpr("c", new NumExpr(17), new MultExpr(new NumExpr(17), new NumExpr(24))), new IfExpr(new NumExpr(17), new MultExpr(new VarExpr("G"), new NumExpr(24)), new BoolExpr(true))), new AddExpr(new BoolExpr(true), new NumExpr(24))), new VarExpr("y"))))->to_pretty_string() == "_if x\n_then _if x\n      _then y\n      _else 17 * 24\n_else (_if _true == 17\n       _then _if x\n             _then _let c = 17\n                   _in  17 * 24\n             _else _if 17\n                   _then G * 24\n                   _else _true\n       _else _true + 24) == y");
};

TEST_CASE ("Let Fun Interp") {
    CHECK((new LetExpr("f", new FunExpr("x", new MultExpr(new VarExpr("x"),new VarExpr("x"))), new CallExpr(new VarExpr("f"), new NumExpr(2))))->interp()->equals(new NumVal(4)));
}

TEST_CASE ("FunExpr Tests") {
    CHECK((new FunExpr("x", new MultExpr(new VarExpr("x"),new VarExpr("x"))))->equals(new FunExpr("x", new MultExpr(new VarExpr("x"),new VarExpr("x")))));
    
    CHECK((new FunExpr("x", new MultExpr(new VarExpr("x"),new VarExpr("x"))))->equals(NULL) == false);
    
    CHECK((new FunExpr("x", new AddExpr(new VarExpr("x"),new VarExpr("2"))))->subst("x", new NumExpr(3))->equals(new FunExpr("x", new AddExpr(new VarExpr("x"),new VarExpr("2")))));
       
    CHECK((new FunExpr("x", new AddExpr(new VarExpr("x"),new VarExpr("y"))))->subst("y", new NumExpr(3))->equals(new FunExpr("x", new AddExpr(new VarExpr("x"),new NumExpr(3)))));
    
    CHECK((new FunExpr("x", new MultExpr(new VarExpr("x"),new VarExpr("x"))))->to_string() == "(_fun (x) (x*x))");
    
    CHECK((new FunExpr("x", new MultExpr(new VarExpr("x"),new VarExpr("x"))))->to_pretty_string() == "_fun (x)\n  x * x");
    
    CHECK((new AddExpr((new FunExpr("x", new MultExpr(new VarExpr("x"),new VarExpr("x")))), new NumExpr(2)))->to_pretty_string() == "(_fun (x)\n   x * x) + 2");
    
   

    
    
}

TEST_CASE ("CallExpr Tests") {
    
    CHECK((new CallExpr(new VarExpr("f"), new MultExpr(new VarExpr("x"),new VarExpr("x"))))->equals(NULL) == false);
    
    CHECK((new CallExpr(new VarExpr("f"), new MultExpr(new VarExpr("x"),new VarExpr("x"))))->equals(new CallExpr(new VarExpr("f"), new MultExpr(new VarExpr("x"),new VarExpr("x")))));
    
    CHECK((new CallExpr(new VarExpr("factrl"), new NumExpr(10)))->to_string() == "(factrl(10))");
    
    CHECK((new CallExpr(new VarExpr("factrl"), new NumExpr(10)))->to_pretty_string() == "factrl(10)");
    
//    CHECK((new CallExpr((new CallExpr(new VarExpr("factrl"), new NumExpr(10))),new VarExpr("factrl") ))->to_pretty_string() == "factrl(10)");
  
}
                                                        
