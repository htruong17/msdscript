//
//  val.cpp
//  msdscript
//
//  Created by Harold Truong on 2/24/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include "catch.h"
#include <stdexcept>
#include "val.h"
#include "expr.h"
//#include "parse.h"
//#include <sstream>



//std::string Val::to_string(){
//    std::ostringstream ss;
//    this->print(ss);
//    return ss.str();
//}
//
//// Converts Expr to pretty_pretty string
//std::string Val::to_pretty_string(){
//    std::ostringstream ss;
//    this->pretty_print(ss);
//    return ss.str();
//}
//
//std::ostream& NumVal::print(std::ostream& argument){
//    return argument << this->rep;
//}
//
//// Prints number value to ostream (for pretty print)
//std::ostream& NumVal::pretty_print(std::ostream& argument){
//    return argument << this->rep;
//}

NumVal::NumVal(int rep) {
    this->rep = rep;
}

Expr* NumVal::to_expr(){
    return new NumExpr(this->rep);
}

Val* NumVal::add_to(Val* other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL)
        throw std::runtime_error("add of non-number");
    return new NumVal(rep + other_num->rep);
}

Val* NumVal::mult_by(Val* other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL)
        throw std::runtime_error("mult of non-number");
    return new NumVal(rep * other_num->rep);
}

bool NumVal::equals(Val* other){
    NumVal *other_numval = dynamic_cast<NumVal*>(other);
    if (other_numval == NULL)
        return false;
    else
        return (this->rep == other_numval->rep);
}

BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}

Expr* BoolVal::to_expr(){
    return new BoolExpr(this->rep);
}

Val* BoolVal::add_to(Val* other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL)
        throw std::runtime_error("cannot add with boolean");
    return new NumVal(rep + other_num->rep);
}

Val* BoolVal::mult_by(Val* other_val){
    NumVal *other_num = dynamic_cast<NumVal*>(other_val);
    if (other_num == NULL)
        throw std::runtime_error("cannot multiply with boolean");
    return new NumVal(rep * other_num->rep);
}

bool BoolVal::equals(Val* other){
    BoolVal *other_boolval = dynamic_cast<BoolVal*>(other);
    if (other_boolval == NULL)
        return false;
    else
        return (this->rep == other_boolval->rep);
}



TEST_CASE ("NumVal equals") {
    CHECK((new AddExpr((new MultExpr(new NumExpr(5), new LetExpr("x", new NumExpr(5), new VarExpr("x")))), new NumExpr(1)))->interp()->equals(NULL) == false);
    CHECK((new AddExpr(new LetExpr("x", new NumExpr(5), new VarExpr("x")), new NumExpr(1)))->interp() ->equals(NULL) == false);
}

TEST_CASE ("NumVal add_to") {
    CHECK_THROWS_WITH( (new MultExpr(new NumExpr(5),new NumExpr(2)))->interp()->add_to(NULL),"add of non-number");
    CHECK_THROWS_WITH( (new AddExpr(new NumExpr(5),new NumExpr(2)))->interp()->mult_by(NULL),"mult of non-number");
    
    //CHECK((new MultExpr(new NumExpr(5), NULL))->interp());
}

//std::ostringstream ss;
//
//TEST_CASE ("Val Print") {
//    CHECK((parse_str("2+5*10")->interp()->to_string() == "52"));
//}
//
//TEST_CASE ("Val Pretty Print") {
//    CHECK((parse_str("_let x = 3 _in x + 5")->interp()->to_pretty_string() == "8"));
//
//}

