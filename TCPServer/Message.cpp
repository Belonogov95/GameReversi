//
// Created by vanya on 18.06.15.
//


#include "Message.h"
#include "HttpWorker.h"
#include "Tools.h"

string Message::get(string property) {
    for (auto x: body) {
        auto list = tools::split(x, '=');
        if (list[0] == property)
            return tools::trim(list[1]);
    }
    assert(false);
}



