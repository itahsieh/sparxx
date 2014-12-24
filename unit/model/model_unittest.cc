#include <iostream>
#include <memory>
#include <fstream>
#include "sparxModel.h"
#include "gtest/gtest.h"

TEST(SparxModel, Load) {
    //Write out test file
    std::fstream fs;
    fs.open ("example.mdl", std::fstream::out);
    fs << "rmax=1.197E+15"
          "ncell=16"
          "tcmb=2.728"
          "columns=id,ra,rb,nh,nm,tk,td,db,vr"
          "gas:dust=100"
          "@";
    for(int i; i<16; i++) {
        fs << std::setw(4) << i+1
             << " 0.10272E+14  0.14106E+14  0.11775E+08  0.23550E-01  0.81791E+02  0.81791E+02  0.12012E+00 -0.26418E+01"
             << std::endl;
    }
    fs.close();

    //Pointer for handling model
    std::auto_ptr<SparxModel> model(new SparxModel);

    //Load model from .mdl file
    model->loadMdl("example.mdl");

    //Test model contents
    EXPECT_FLOAT_EQ(1.197E+15, model->rMax);
    EXPECT_EQ(16, model->nCell);
    EXPECT_FLOAT_EQ(2.728, model->tCmb);
    EXPECT_FLOAT_EQ(100, model->gasToDust);
    for(int i; i<model->nCell; i++) {
        EXPECT_EQ(i+1, model->zones[i].id);
        EXPECT_FLOAT_EQ( 0.10272E+14, model->zones[i].ra);
        EXPECT_FLOAT_EQ( 0.14106E+14, model->zones[i].rb);
        EXPECT_FLOAT_EQ( 0.11775E+08, model->zones[i].nh);
        EXPECT_FLOAT_EQ( 0.23550E-01, model->zones[i].nm);
        EXPECT_FLOAT_EQ( 0.81791E+02, model->zones[i].tk);
        EXPECT_FLOAT_EQ( 0.12012E+00, model->zones[i].db);
        EXPECT_FLOAT_EQ(-0.26418E+01, model->zones[i].vr);
    }
}
