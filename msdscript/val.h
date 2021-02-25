//
//  val.hpp
//  msdscript
//
//  Created by Harold Truong on 2/24/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef val_h
#define val_h

#include <stdio.h>

class Expr;

class Val{
public:
//    std::string to_string();
//    std::string to_pretty_string();
//    virtual std::ostream& print(std::ostream& argument) = 0;
//    virtual std::ostream& pretty_print(std::ostream& argument) = 0;
    virtual Expr* to_expr() = 0;
    virtual Val* add_to(Val* other_val) = 0;
    virtual Val* mult_by(Val* other_val) = 0;
    virtual bool equals(Val* other) = 0;

};

class NumVal: public Val{
public:
    int rep;
    NumVal(int rep);
    virtual Expr* to_expr();
    virtual Val* add_to(Val* other_val);
    virtual Val* mult_by(Val* other_val);
    virtual bool equals(Val* other);
//    virtual std::ostream& print(std::ostream& argument);
//    virtual std::ostream& pretty_print(std::ostream& argument);
};

#endif /* val_hpp */
