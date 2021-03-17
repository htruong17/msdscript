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

Val* NumVal::call(Val *actual_arg){
    throw std::runtime_error("call error for NumVal");
}

BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}

Expr* BoolVal::to_expr(){
    return new BoolExpr(this->rep);
}

Val* BoolVal::add_to(Val* other_val){
    throw std::runtime_error("cannot add with boolean");
}

Val* BoolVal::mult_by(Val* other_val){
    throw std::runtime_error("cannot multiply with boolean");
}

bool BoolVal::equals(Val* other){
    BoolVal *other_boolval = dynamic_cast<BoolVal*>(other);
    if (other_boolval == NULL)
        return false;
    else
        return (this->rep == other_boolval->rep);
}

Val* BoolVal::call(Val *actual_arg){
    throw std::runtime_error("call error for BoolVal");
}

FunVal::FunVal(std::string formal_arg, Expr *body){
    this->formal_arg = formal_arg;
    this->body = body;
}

Expr* FunVal::to_expr(){
    return new FunExpr(this->formal_arg, this->body);
}

Val* FunVal::add_to(Val* other_val){
    throw std::runtime_error("add error for FunVal");
}

Val* FunVal::mult_by(Val* other_val){
    throw std::runtime_error("mult error for FunVal");
}

bool FunVal::equals(Val* other){
    FunVal *other_funval = dynamic_cast<FunVal*>(other);
    if (other_funval == NULL)
        return false;
    else
        return (this->formal_arg == other_funval->formal_arg && this->body->equals(other_funval->body));
}

Val* FunVal::call(Val *actual_arg){
//    FunVal *other_fun = dynamic_cast<FunVal*>(actual_arg);
//    if (other_fun == NULL)
//        throw std::runtime_error("Call error for FunVal");
    return this->body->subst(this->formal_arg, actual_arg->to_expr())->interp();
    //return this->body
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


TEST_CASE ("Bool Val Equals") {
    CHECK((new BoolVal(true))->equals(NULL) == false);
}

TEST_CASE ("FunVal Tests") {
    CHECK_THROWS_WITH((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(5))))->add_to((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(5))))),"add error for FunVal");
    CHECK_THROWS_WITH((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(5))))->mult_by((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(5))))),"mult error for FunVal");
    CHECK((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(5))))->equals((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(5))))));
    CHECK((new FunVal("x", new AddExpr(new VarExpr("x"), new NumExpr(5))))->equals(NULL) == false);
}

TEST_CASE("Call Errors"){
    CHECK_THROWS_WITH((new NumVal(3))->call(new NumVal(5)),"call error for NumVal");
    CHECK_THROWS_WITH((new BoolVal(true))->call(new NumVal(5)),"call error for BoolVal");
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

