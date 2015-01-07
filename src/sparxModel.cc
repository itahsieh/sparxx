#include <iostream>
#include <fstream>
#include <unordered_map>
#include <utility>
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
  std::ifstream fs(name.c_str()); // Input file stream
  bool procTable = false; // Flag for indicating whether we're in the header or table section
  std::string line; // Temporary buffer for storing lines
  int cellId = 0;

  if(fs.is_open()) {
    while(std::getline(fs, line)) {
      trim(line);

      //Skip comments
      if(line[0] == '#') {
          continue;
      }

      //Load model
      if(!procTable) {
        if(line == "@") { // We've hit the header/table separator
          procTable = true;

          // Now's the time to figure out what the column indices are
          mapColumnIndex(std::string("id"));
          mapColumnIndex(std::string("ra"));
          mapColumnIndex(std::string("rb"));
          mapColumnIndex(std::string("nh"));
          mapColumnIndex(std::string("nm"));
          mapColumnIndex(std::string("tk"));
          mapColumnIndex(std::string("td"));
          mapColumnIndex(std::string("db"));
          mapColumnIndex(std::string("vr"));

          // Reset the cell index
          cellId = 0;
        }
        else { // We're in the header section, so read the key/val pairs.
          std::vector<std::string> strs; // Temporary buffer for storing tokens
          boost::split(strs, line, boost::is_any_of("=")); // Split key/val pair

          #ifdef DEBUG
          std::cout << "HEAD: " << line << "-->";
          for(std::vector<std::string>::const_iterator i=strs.begin(); i != strs.end(); ++i) {
              std::cout << (i == strs.begin() ? "" : " = ") << *i;
          }
          std::cout << std::endl;
          #endif

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
              boost::split(_columns, strs[1], boost::is_any_of(","));
          }
        }
      }
      else { // We're in the table section, read table according to columns spec
        std::vector<std::string> strs; // Temporary buffer for storing tokens
        boost::split(strs, line, boost::is_any_of(" \t"), token_compress_on); // Split key/val pair
        Cell cell;

        cell.id = boost::lexical_cast<int>(strs[getColIdx("id")]); // id starts at 1

        if(cell.id-1 != cellId++) {
          throw SparxModelBadCellId();
        }

        /* Format:
          *id     shell number
          *ra,rb  inner & outer radius (m)
          *za,zb  lower & upper height (m) (2D only)
          nh      density (cm-3) of main collision partner (usually H2)
          nm      density (cm-3) of molecule
          ne      density (cm-3) of second collision partner (e.g. electrons)
          tk      kinetic temperature (K)
          td      dust temperature (K)
          te      `electron' temperature (K)
          db      1/e half-width of line profile (Doppler b-parameter) (km s-1)
          vr      radial velocity (km s-1)
          vz      vertical velocity (km s-1) (2D only)
          vr      azimuthal velocity (km s-1) (2D only)
        */
        cell.ra = boost::lexical_cast<double>(strs[getColIdx("ra")]); // m
        cell.rb = boost::lexical_cast<double>(strs[getColIdx("rb")]); // m
        cell.nh = boost::lexical_cast<double>(strs[getColIdx("nh")]); // cm^-3
        cell.nm = boost::lexical_cast<double>(strs[getColIdx("nm")]); // cm^-3
        cell.tk = boost::lexical_cast<double>(strs[getColIdx("tk")]); // K
        cell.td = boost::lexical_cast<double>(strs[getColIdx("td")]); // K
        cell.db = boost::lexical_cast<double>(strs[getColIdx("db")]); // km s^-1
        cell.vr = boost::lexical_cast<double>(strs[getColIdx("vr")]); // km s^-1

        _cells.push_back(cell);

        #ifdef DEBUG
        std::cout << "TABL: " << line << std::endl;
        for(std::vector<std::string>::const_iterator i=strs.begin(); i != strs.end(); ++i) {
            std::cout << (i == strs.begin() ? "" : ", ") << *i;
        }
        std::cout << std::endl;
        #endif

      }
    }
    fs.close();
  }

  return;
}



int SparxModel::findColumnIndex(std::string name)
// Find index of name within columns
{
  int idx = -1; // What to return when name isn't found
  std::vector<std::string>::iterator find_posn; // iterator for std::find results

  // At this point we should've loaded _columns already, so check
  // if size is positive
  if(_columns.size() == 0) {
    throw SparxModelNoColumns();
  }

  // Find index of name
  find_posn = std::find(_columns.begin(), _columns.end(), name);

  // Set idx to position if find_posn isn't end()
  if(find_posn != _columns.end()) {
    idx = find_posn - _columns.begin();
  }

  return idx;
}



void SparxModel::mapColumnIndex(std::string name)
// Map column indices to the _colIdx hash map
{
  int idx = findColumnIndex(name);

  if(idx >= 0) {
    _colIdx.insert(std::make_pair(name, idx));
    #ifdef DEBUG
    std::cout << name << "=" << _colIdx[name] << std::endl;
    #endif
  }
  else {
    throw SparxModelMissingColumn(name.c_str());
  }

  return;
}



