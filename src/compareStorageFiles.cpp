#include <OpenSim/OpenSim.h>
using OpenSim::Array;
using OpenSim::Storage;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <algorithm>

double calculateRMSE(Array<double> arr1, Array<double> arr2) {

    double sum(0);
    size_t sizeArr(std::min(arr1.size(), arr2.size()));
    for (unsigned i(0); i < sizeArr; ++i)
        sum += (arr1[i] - arr2[i])*(arr1[i] - arr2[i]);
    return std::sqrt(sum / static_cast<double>(sizeArr));
}

void printTable(vector<string> names, vector<double> errors) {

    cout << endl;
    cout << std::setw(40) << "Dof" << std::setw(20) << "RMSE" << endl;
    cout << "          " << std::setfill('-') << std::setw(50) << '-' << endl;
    for (unsigned i(0); i < names.size(); ++i)
        cout << std::setfill(' ') << std::setw(40) << names[i] << std::setw(20) << errors[i] << endl;
    cout << endl;
}

void strip(string& name) {

    const vector<string> toStrip{ "_moment", "_angle" };
    for (auto& s : toStrip) {
        auto it(name.find(s));
        if (it != string::npos)
            name = name.substr(0, it);
    }
}

void strip(Array<string>& names) {

    for (int i(0); i < names.getSize(); ++i)
        strip(names[i]);
}

void compareSto(const string& f1, const string& f2) {

    Storage sto1(f1.c_str());
    Storage sto2(f2.c_str());
    vector<double> errors;
    vector<string> names;
    Array<string> names1(sto1.getColumnLabels());
    strip(names1);
    Array<string> names2(sto2.getColumnLabels());
    strip(names2);
    for (unsigned idx1(0); idx1 < names1.size(); ++idx1) {
        string currentName1(names1.get(idx1));
        int idx2(names2.findIndex(currentName1));
        //time column should be 0, and -1 is not found
        if (idx2 > 0) {
            Array<double> column1, column2;
            sto1.getDataColumn(idx1, column1);
            sto2.getDataColumn(idx2, column2);
            errors.emplace_back(calculateRMSE(column1, column2));
            names.emplace_back(currentName1);
        }
    }

    cout << "Comparing " << f1 << " and " << f2 << endl;
    printTable(names, errors);
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        cout << "Usage: compareStorageFiles filename1 filename2\n";
        exit(EXIT_FAILURE);
    }
    compareSto(argv[1], argv[2]);
    return 0;
}