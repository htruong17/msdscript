//
//  main.c
//  msdscript
//
//  Created by Harold Truong on 1/21/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

//#define CATCH_CONFIG_RUNNER

//#include "catch.h"

#include "cmdline.h"
#include "expr.h"
#include "parse.h"
#include <iostream>



int main(int argc, char **argv) {
    try {
        //Catch::Session().run(1, argv);
        //use_arguments(argc, argv);
        bool one = true;
        bool two = true;
        std::cout << one + two << std::endl;
        return 0;
    } catch (std::runtime_error exn) {
        std::cerr <<exn.what() << "\n";
        return 1;
    }

}
