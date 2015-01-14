#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_map>

class Cell {
  public:
    int id;
    double ra; //m
    double rb; //m
    double nh; //m^-3
    double nm; //m^-3
    double tk; //K
    double td; //K
    double db; //km s^-1
    double vr; //km s^-1
};

class SparxModel {
  public:
    SparxModel();
    void loadMdl(std::string name);
    int getColIdx(std::string name) { return _colIdx[name]; };
    int getColIdx(const char *name) { return _colIdx[std::string(name)]; };

    double _rMax; // Max radius of model
    int _nCell; // Number of cells in model
    double _tCmb; // Cosmic microwave background temperature (K)
    double _gasToDust; // Gas to dust ratio
    std::vector<Cell> _cells; // Vector of zones

  private:
    int findColumnIndex(std::string name);
    void mapColumnIndex(std::string name);

    std::vector<std::string> _columns; // Columns spec of table
    std::unordered_map<std::string, int> _colIdx; // Mapping of column name to index
};

//
// Exceptions
//
class SparxModelNoColumns : std::runtime_error {
  public:
    SparxModelNoColumns() : std::runtime_error("Columns undefined") {}
};

class SparxModelMissingColumn : std::runtime_error {
  public:
    SparxModelMissingColumn(const char *colName) : std::runtime_error(colName) {}
};

class SparxModelBadCellId : std::runtime_error {
  public:
    SparxModelBadCellId() : std::runtime_error("Cell id inconsistent with row number") {}
};
