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
        SparxModel() { return; }
        void loadMdl(std::string name);

        double rMax;
        long nCell;
        double tCmb;
        double gasToDust;
        std::vector<Zone> zones;
};
