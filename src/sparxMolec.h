#ifndef __SPARXMOLEC_H__
#define __SPARXMOLEC_H__

#include <string>
#include <vector>
#include "lamdaMolec.h"


class SparxMolecLine {
  public:
    SparxMolecLine(MolecLine line);

    int trans;
    int upper;
    int lower;
    double freq;
    double Aul;
    double Bul;
    double Blu;
};

class SparxMolec : public LamdaMolec {
  public:
    SparxMolec() {};
    void loadDat(std::string name);

    std::vector<SparxMolecLine> lines;
};

#endif
