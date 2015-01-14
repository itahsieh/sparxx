#include <iostream>
#include <fstream>
#include <unordered_set>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "lamdaMolec.h" 

using namespace boost::algorithm;

/*
File format for LAMDA molecular data files:
% Lines 1-2: molecule (or atom) name 
% Lines 3-4: molecular (or atomic) weight (a.m.u.) 
% Lines 5-6: number of energy levels (NLEV) 
% Lines 7-7+NLEV: level number, level energy (cm-1), statistical weight. These numbers may be followed by additional info such as the quantum numbers, which are however not used by the program. The levels must be listed in order of increasing energy. 
% Lines 8+NLEV-9+NLEV: number of radiative transitions (NLIN) 
% Lines 10+NLEV-10+NLEV+NLIN: transition number, upper level, lower level, spontaneous decay rate (s-1). These numbers may be followed by additional info such as the line frequency, which is however not used by the program. 
% Lines 11+NLEV+NLIN-12+NLEV+NLIN: number of collision partners 
% Lines 13+NLEV+NLIN-14+NLEV+NLIN: collision partner ID and reference. Valid identifications are: 1=H2, 2=para-H2, 3=ortho-H2, 4=electrons, 5=H, 6=He. 
% Lines 15+NLEV+NLIN-16+NLEV+NLIN: number of transitions for which collisional data exist (NCOL) 
% Lines 17+NLEV+NLIN-18+NLEV+NLIN: number of temperatures for which collisional data exist 
% Lines 19+NLEV+NLIN-20+NLEV+NLIN: values of temperatures for which collisional data exist 
% Lines 21+NLEV+NLIN-21+NLEV+NLIN+NCOL: transition number, upper level, lower level; rate coefficients (cm3s-1) at each temperature. The program interpolates between rate coefficients in the specified temperature range. Outside this range, it assumes the collisional de-excitation rate coefficients are constant with T, i.e., it uses rate coefficients specified at the highest T (400 K in this case)also for higher temperatures, and similarly at temperatures below the lowest value (10 K in this case) for which rate coefficients were specified. 
*/
static void readLine(std::ifstream &fs, std::string &line, int nReads, bool resetCount);

static void readLine(std::ifstream &fs, std::string &line, int nReads = 1, bool resetCount = false)
// Helper function for reading and trimming lines
{
  static int lineCount = 0;

  if(resetCount) {
    lineCount = 1;
  }

  // Silently correct nReads if we got something bad
  nReads = nReads <= 0 ? 1 : nReads;

  // Loop nReads times and keep only the last line
  for(int i = 0; i < nReads; i++) {
    lineCount++;
    std::getline(fs, line);
    std::cout << "Line " << lineCount << ": " << line << std::endl;
  }

  // Trim the line before returning
  trim(line);

  return;
}

void LamdaMolec::loadDat(std::string name)
{
  std::ifstream fs;
  std::string line;
  MolecCollData collDataTmp;
  
  //fs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fs.open(name.c_str());

  readLine(fs, line, 2); // Line 2: Molecule name
  name = std::string(line);

  readLine(fs, line, 2); // Line 4: Molecular weight
  molecularWeight = boost::lexical_cast<double>(line);

  readLine(fs, line, 2); // Line 6: Number of levels
  nlev = boost::lexical_cast<int>(line);
  if(nlev <= 0) {
    throw LamdaMolecBadNlev();
  }

  // Lines 7-7+NLEV: Level data
  readLine(fs, line);
  for(int i=0; i<nlev; i++) {
    readLine(fs, line); // Read 1 row of data
    std::vector<std::string> strs; // For token storage
    boost::split(strs, line, boost::is_any_of(" \t"), token_compress_on); // Split line into columns
    MolecLevel lev;
    lev.level = boost::lexical_cast<int>(strs[0]);
    lev.energy = boost::lexical_cast<double>(strs[1]);
    lev.weight = boost::lexical_cast<double>(strs[2]);
    lev.j = boost::lexical_cast<int>(strs[3]);
    _levTab.insert(std::pair<int, MolecLevel>(lev.level, lev));
  }

  // Line 8+NLEV - 9+NLEV: NLIN
  readLine(fs, line, 2);
  nlin = boost::lexical_cast<int>(line);
  if(nlev <= 0) {
    throw LamdaMolecBadNlin();
  }

  // Lines 10+NLEV - 10+NLEV+NLIN: line data
  readLine(fs, line);
  for(int i=0; i<nlin; i++) {
    readLine(fs, line);
    std::vector<std::string> strs; // For token storage
    boost::split(strs, line, boost::is_any_of(" \t"), token_compress_on); // Split line into columns
    MolecLine line;
    line.trans = boost::lexical_cast<int>(strs[0]);
    line.upperLev = boost::lexical_cast<int>(strs[1]);
    line.lowerLev = boost::lexical_cast<int>(strs[2]);
    line.einsteinA = boost::lexical_cast<double>(strs[3]);
    line.freq = boost::lexical_cast<double>(strs[4]);
    line.eU = boost::lexical_cast<double>(strs[5]);
    _lineData.insert(std::pair<int, MolecLine>(line.trans, line));
  }

  // Lines 11+NLEV+NLIN - 12+NLEV+NLIN: number of collission partners
  readLine(fs, line, 2);
  ncoll = boost::lexical_cast<int>(line);
  if(nlev <= 0) {
    throw LamdaMolecBadNcol();
  }

  // Loop through ncoll sections of collisional partner data
  for(int i = 0; i < ncoll; i++) {
    MolecCollData collDataTmp; // Temporary storage for collisional data

    // Lines 13+NLEV+NLIN-14+NLEV+NLIN: collision partner ID and reference. Valid identifications are: 1=H2, 2=para-H2, 3=ortho-H2, 4=electrons, 5=H, 6=He.
    readLine(fs, line, 2);
    std::vector<std::string> strs; // For token storage
    boost::split(strs, line, boost::is_any_of(" \t"), token_compress_on); // Split line into columns

    int partnerId = boost::lexical_cast<int>(strs[0]); // Partner ID
    switch(partnerId) {
      case 1: collDataTmp.partnerId = CollPartner::H2; break;
      case 2: collDataTmp.partnerId = CollPartner::paraH2; break;
      case 3: collDataTmp.partnerId = CollPartner::orthoH2; break;
      case 4: collDataTmp.partnerId = CollPartner::electrons; break;
      case 5: collDataTmp.partnerId = CollPartner::H; break;
      case 6: collDataTmp.partnerId = CollPartner::He; break;
      default: throw LamdaMolecBadPartner();
    }

    collDataTmp.partnerRef = strs[1]; // Partner reference

    // Lines 15+NLEV+NLIN-16+NLEV+NLIN: number of transitions for which collisional data exist (NCOL) 
    readLine(fs, line, 2);
    collDataTmp.nTrans = boost::lexical_cast<int>(line);

    if(collDataTmp.nTrans <= 0) {
      throw LamdaMolecBadTemps();
    }

    // Lines 17+NLEV+NLIN-18+NLEV+NLIN: number of temperatures for which collisional data exist
    readLine(fs, line, 2);
    collDataTmp.nTemps = boost::lexical_cast<int>(line);

    if(collDataTmp.nTemps <= 0) {
      throw LamdaMolecBadTemps();
    }

    // Lines 19+NLEV+NLIN-20+NLEV+NLIN: values of temperatures for which collisional data exist
    readLine(fs, line, 2);
    boost::split(strs, line, boost::is_any_of(" \t"), token_compress_on); // Split line into temps

    if(static_cast<int>(strs.size()) != collDataTmp.nTemps) {
      throw LamdaMolecBadTemps();
    }

    for(int j = 0; j < collDataTmp.nTemps; j++) {
      trim(strs[j]);
      collDataTmp.temps.push_back(boost::lexical_cast<double>(strs[j]));
    }

    // Lines 21+NLEV+NLIN-21+NLEV+NLIN+NCOL: transition number, upper level, lower level; rate coefficients (cm3s-1) at each temperature. The program interpolates between rate coefficients in the specified temperature range. Outside this range, it assumes the collisional de-excitation rate coefficients are constant with T, i.e., it uses rate coefficients specified at the highest T (400 K in this case)also for higher temperatures, and similarly at temperatures below the lowest value (10 K in this case) for which rate coefficients were specified. 
    readLine(fs, line);
    for(int j = 0; j < collDataTmp.nTrans; j++) {
      readLine(fs, line);
    }

    // Lines 21+NLEV+NLIN+NCOL - 22+NLEV+NLIN+NCOL: notes
    readLine(fs, line, 2);
  }
  fs.close();

  return;
}
