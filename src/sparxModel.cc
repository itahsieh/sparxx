#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "sparxModel.h"

using namespace boost::algorithm;

SparxModel::SparxModel()
{
    _rMax = 0;
    _nCell = 0;
    _tCmb = 0;
    _gasToDust = 0;

    return;
}

void SparxModel::loadMdl(std::string name)
{
    std::string line;
    std::ifstream fs(name.c_str());
    bool procTable = false;
    std::vector<std::string> columns;

    if(fs.is_open()) {
        while(std::getline(fs, line)) {
            trim(line);

            //Skip comments
            if(line[0] == '#') {
                continue;
            }

            //Load model
            if(!procTable) {
                if(line == "@") {
                    procTable = true;
                }
                else {
                    std::vector<std::string> strs;
                    boost::split(strs, line, boost::is_any_of("="));
                    std::cout << "HEAD: " << line << "-->";
                    for(std::vector<std::string>::const_iterator i=strs.begin(); i != strs.end(); ++i) {
                        std::cout << "=" << *i;
                    }
                    std::cout << std::endl;
                    if(strs[0] == "rmax") {
                        _rMax = boost::lexical_cast<double>(strs[1]);
                    }
                    else if(strs[0] == "ncell") {
                        _nCell = boost::lexical_cast<int>(strs[1]);
                        std::cout << "_nCell=" << _nCell << std::endl;
                    }
                    else if(strs[0] == "tcmb") {
                        _tCmb = boost::lexical_cast<double>(strs[1]);
                    }
                    else if(strs[0] == "gas:dust") {
                        _gasToDust = boost::lexical_cast<double>(strs[1]);
                    }
                    else if(strs[0] == "columns") {
                        boost::split(columns, strs[1], boost::is_any_of(","));
                    }
                }
            }
            else {
                std::cout << "TABL: " << line << std::endl;
            }
        }
        fs.close();
    }

    return;
}
