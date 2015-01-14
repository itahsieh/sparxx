#include "physics.h"
#include "sparxMolec.h"

void SparxMolec::loadDat(std::string name)
// Convert to SI units and calculate derived quantities
{
  // Call parent loadDat to load raw data from file
  LamdaMolec::loadDat(name);

  this->molecularWeight *= PHYS_amu;

  // Calculate line frequency/Bul/Blu
  for(int i = 0; i < this->nlin; i++) {
    MolecLine line = this->lineData[i]; // Ref to line
    MolecLevel &upper = this->levData[line.upperLev]; // Ref to upper lev
    MolecLevel &lower = this->levData[line.lowerLev]; // Ref to lower lev
    SparxMolecLine lineTmp = static_cast<SparxMolecLine>(line);

    double freq = lineTmp.freq = (upper.energy - lower.energy) * PHYS_c; // Line frequency is (e1 - e0) * 100 * c
    double Bul = lineTmp.Bul = line.Aul * (PHYS_c / freq) * (PHYS_c / freq) / (PHYS_h * freq) / 2.0;
    lineTmp.Blu = upper.weight / lower.weight * Bul;

    this->lineData[i] = lineTmp;
  }

  return;
}
