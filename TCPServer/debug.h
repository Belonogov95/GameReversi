//
// Created by vanya on 20.06.15.
//

#ifndef HWW2_DEBUG_H
#define HWW2_DEBUG_H

#define db(x) cerr << #x << " = " << x << endl
#define db2(x, y) cerr << "(" << #x << ", " << #y << ") = (" << x << ", " << y << ")\n"

#define MY_ASSERT

#ifdef MY_ASSERT
#define assertMy(x) if (!(x)) { cerr << "assertion error file: " << __FILE__ << "    line: " \
    << __LINE__ << "       " << #x << endl; exit(1); }
#else
#define assertMy(x) x
#endif



//void assert(bool result);

#endif //HWW2_DEBUG_H
