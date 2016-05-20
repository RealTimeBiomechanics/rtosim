#ifndef rtosim_ProgramOptionsParser_h
#define rtosim_ProgramOptionsParser_h

#include <string>
#include <vector>
#include <algorithm>

namespace rtosim {

    class ProgramOptionsParser {
    public:
        ProgramOptionsParser(int argc, char **argv);
        bool exists(const std::string& option) const;
        //get the parameter that follows the option flag
        template<typename T>
        T getParameter(const std::string& option) const;
        std::string getParameter(const std::string& option) const;

    private:
        std::vector<std::string> po_;
    };

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
}

#endif