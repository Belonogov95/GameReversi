//
// Created by vanya on 19.06.15.
//

#include "Tools.h"


std::vector<std::string> tools::split(const std::string & s, char ch) {
    std::vector<std::string> res;
    for (int i = 0; i < (int) s.size(); i++) {
        for (; i < (int) s.size() && s[i] == ch; i++);
        int j = i;
        for (; i < (int) s.size() && s[i] != ch; i++);
        if (i - j > 0) {
            res.push_back(s.substr(j, i - j));
        }
    }
    return res;
}


std::string tools::trim(const std::string & s) {
    int l = 0;
    for (; l < (int) s.size() && isspace(s[l]); l++);
    int r = (int) s.size() - 1;
    for (; r > 0 && isspace(s[r]); r--);
    if (r < l) return std::string();
    return s.substr(l, (r - l) + 1);
}


