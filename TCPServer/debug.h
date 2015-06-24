//
// Created by vanya on 20.06.15.
//

#ifndef HWW2_DEBUG_H
#define HWW2_DEBUG_H

#include <iostream>
#include <cassert>

#define db(x) cerr << #x << " = " << x << endl
#define db2(x, y) cerr << "(" << #x << ", " << #y << ") = (" << x << ", " << y << ")\n"

#define MY_RELEASE


#ifdef MY_RELEASE
    #define myAssert(x) assert(x);
    #define myCheck(x) assert(x);
#else
    #define myAssert(x) x
    #define myCheck(x)
#endif



#endif

//HWW2_DEBUG_H

//void assert(bool result);

//if (!(x)) {
//    std::cerr << "assertion error file: " << __FILE__ << "    line: "
//    << __LINE__ << "       " << #x << std::endl;
//    perror("");
//    exit(1);
//}


