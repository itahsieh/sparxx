#include <vector>
#include <string>

class Zone {
    public:
        int id;
        double ra;
        double rb;
        double nh;
        double nm;
        double tk;
        double td;
        double db;
        double vr;
};

class SparxModel {
    public:
        SparxModel();
        void loadMdl(std::string name);

        double _rMax;
        int _nCell;
        double _tCmb;
        double _gasToDust;
        std::vector<Zone> _zones;
};
