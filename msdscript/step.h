//
//  step.h
//  msdscript
//
//  Created by Harold Truong on 4/6/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#ifndef step_hpp
#define step_hpp

#include <stdio.h>
#include "pointer.h"

class Expr;
class Env;
class Val;
class Cont;

class Step {
public:
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;
    
    static mode_t mode;
    static PTR(Expr) expr;
    static PTR(Env) env;
    static PTR(Val) val;
    static PTR(Cont) cont;
    static PTR(Val) interp_by_steps(PTR(Expr) e);
};

#endif /* step_h */
