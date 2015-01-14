#ifndef __LAMDAMOLEC_H__
#define __LAMDAMOLEC_H__

#include <string>
#include <vector>
#include <stdexcept>
#include <unordered_map>

typedef struct MolecLevel {
  int level;
  double energy; //cm^-1
  double weight; //a.m.u.
  int j;
} MolecLevel;

typedef struct MolecLine {
  int trans;
  int upperLev;
  int lowerLev;
  double einsteinA;
  double freq;
  double eU;
} MolecLine;

typedef struct MolecCollTrans {
  int trans;
  int upperLev;
  int lowerLev;
  std::vector<double> collRates;
} MolecCollTrans;

enum class CollPartner {
  H2 = 1,
  paraH2,
  orthoH2,
  electrons,
  H,
  He
};

typedef struct MolecCollData {
  CollPartner partnerId;
  std::string partnerRef;
  int nTrans;
  int nTemps;
  std::vector<double> temps;
  std::vector<MolecCollTrans> cxTab;
  std::string notes;
} MolecCollData;

class LamdaMolec {
  public:
    LamdaMolec() {}
    void loadDat(std::string name);

    std::string name;
    double molecularWeight;
    int nlev;
    int nlin;
    int ncoll;
    std::vector<MolecCollData> collData;

  private:
    std::unordered_map<int, MolecLevel> _levTab;
    std::unordered_map<int, MolecLine> _lineData;
};

class LamdaMolecBadNlev : std::runtime_error {
  public:
    LamdaMolecBadNlev() : std::runtime_error("Bad NLEV") {}
};

class LamdaMolecBadNlin : std::runtime_error {
  public:
    LamdaMolecBadNlin() : std::runtime_error("Bad NLIN") {}
};

class LamdaMolecBadNcol : std::runtime_error {
  public:
    LamdaMolecBadNcol() : std::runtime_error("Bad NCOL") {}
};

class LamdaMolecBadPartner : std::runtime_error {
  public:
    LamdaMolecBadPartner() : std::runtime_error("Bad collissonal partner") {}
};

class LamdaMolecBadTemps : std::runtime_error {
  public:
    LamdaMolecBadTemps() : std::runtime_error("Bad temperatures") {}
};

class LamdaMolecBadCX : std::runtime_error {
  public:
    LamdaMolecBadCX() : std::runtime_error("Bad collisional transition") {}
};
#endif
