//
//  cmdline.cpp
//  msdscript
//
//  Created by Harold Truong on 1/21/21.
//  Copyright © 2021 Harold Truong. All rights reserved.
//

#include <stdio.h>
#include <iostream>

bool tested = false;

int use_arguments(int argc, char **argv){
    for (int i = 1; i < argc; i++){
        if(std::string(argv[1]) == "--help"){
            std::cout << "Only \"--help\" and \"--test\" arguments are allowed"  << std::endl;
            exit(0);
        } else if(std::string(argv[1]) == "--test" && tested == false){
            std::cout << "Tests passed"  << std::endl;
            tested = true;
            continue;
        } else if(std::string(argv[1]) == "--test" && tested == true){
            std::cerr << "Been tested already"  << std::endl;
            exit(1);
        } else {
            std::cerr << "Invalid argument"  << std::endl;
            exit(1);
        }
       
    }
    if (argc ==1){
        std::cerr << "No argument given\n";
    }
    return 0;
}
