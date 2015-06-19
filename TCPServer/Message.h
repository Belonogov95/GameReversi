//
// Created by vanya on 18.06.15.
//
#include <bits/stdc++.h>

#ifndef HWW2_MESSAGE_H
#define HWW2_MESSAGE_H

using std::string;
using std::vector;

class Message {
public:
    string type;
    string URL;
    vector < string > body;

    string get(string property);
};


#endif //HWW2_MESSAGE_H
