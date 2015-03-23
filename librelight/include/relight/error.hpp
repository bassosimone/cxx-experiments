//
// This file is part of LibRelight.
//
// LibRelight is free software. See AUTHORS and LICENSE for more
// information on the copying conditions.
//

#pragma once

namespace relight {

class Error {

    int errorcode = 0;

  public:
    Error(int e) : errorcode(e) {}

    operator bool() { return errorcode != 0; }

    operator int() { return errorcode; }
};
}
