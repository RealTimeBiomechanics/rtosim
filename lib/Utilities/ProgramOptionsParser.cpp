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

    template<typename T>
    T ProgramOptionsParser::getParameter(const std::string& option) const {

        auto it(std::find(po_.begin(), po_.end(), option));
        T ans{ 0 };
        if (it != po_.end() && std::next(it) != po_.end()) {
            std::stringstream ss(*std::next(it));
            ss >> ans;
        }
        return ans;
    }

    std::string ProgramOptionsParser::getParameter(const std::string& option) const {

        return getParameter<std::string>(option);
    }
}