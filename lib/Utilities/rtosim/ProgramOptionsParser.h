#ifndef rtosim_ProgramOptionsParser_h
#define rtosim_ProgramOptionsParser_h

#include <string>
#include <vector>

namespace rtosim {

    class ProgramOptionsParser {
    public:
        ProgramOptionsParser(char **argv, int argc);
        bool exists(const std::string& option) const;
        //get the parameter that follows the option flag
        template<typename T>
        T getParameter(const std::string& option) const;
        std::string getParameter(const std::string& option) const;

    private:
        std::vector<std::string> po_;
    };
}

#endif