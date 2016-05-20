#include "rtosim/ProgramOptionsParser.h"
#include <string>
#include <vector>
#include <sstream>

namespace rtosim {

    ProgramOptionsParser::ProgramOptionsParser(int argc, char **argv) {

        char** it = argv;
        char** end = argv + argc;
        for (it; it != end; ++it)
            po_.push_back(*it);
    }

    bool ProgramOptionsParser::exists(const std::string& option) const {

        return std::find(po_.begin(), po_.end(), option) != po_.end();
    }

    std::string ProgramOptionsParser::getParameter(const std::string& option) const {

        return getParameter<std::string>(option);
    }
}