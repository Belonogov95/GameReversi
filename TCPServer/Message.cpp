//
// Created by vanya on 18.06.15.
//

#include "Message.h"
#include "HttpWorker.h"
#include "Tools.h"
#include "debug.h"

string Message::get(string property) const {
    for (auto x: body) {
        auto currentString = tools::split(x, '=');
        if (currentString[0] == property)
            return tools::trim(currentString[1]);
    }
    myAssert(false);
}




