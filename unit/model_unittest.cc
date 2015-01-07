#include <iostream>
#include <fstream>
#include <memory>
#include "sparxModel.h"
#include "gtest/gtest.h"

TEST(SparxModel, loadMdl) {
    //Write out test file
    std::fstream fs;
    fs.open("example.mdl", std::fstream::out);
    fs << "rmax=1.197E+15\n"
          "ncell=16\n"
          "tcmb=2.728\n"
          "columns=id,ra,rb,nh,nm,tk,td,db,vr\n"
          "gas:dust=100\n"
          "@\n";
    for(int i=0; i<16; i++) {
        fs << std::setw(4) << i+1
             << "  0.10272E+14  0.14106E+14  0.11775E+08  0.23550E-01  0.81791E+02  0.81791E+02  0.12012E+00 -0.26418E+01"
             << std::endl;
    }
    fs.close();

    //Pointer for handling model
    std::auto_ptr<SparxModel> model(new SparxModel);

    //Load model from .mdl file
    model->loadMdl("example.mdl");

    //Test model contents
    EXPECT_FLOAT_EQ(1.197E+15, model->_rMax);
    EXPECT_EQ(16, model->_nCell);
    EXPECT_FLOAT_EQ(2.728, model->_tCmb);
    EXPECT_FLOAT_EQ(100, model->_gasToDust);
    ASSERT_EQ(model->_nCell, model->_cells.size());
    EXPECT_EQ(0, model->getColIdx(std::string("id")));
    EXPECT_EQ(1, model->getColIdx(std::string("ra")));
    EXPECT_EQ(2, model->getColIdx(std::string("rb")));
    EXPECT_EQ(3, model->getColIdx(std::string("nh")));
    EXPECT_EQ(4, model->getColIdx(std::string("nm")));
    EXPECT_EQ(5, model->getColIdx(std::string("tk")));
    EXPECT_EQ(6, model->getColIdx(std::string("td")));
    EXPECT_EQ(7, model->getColIdx(std::string("db")));
    EXPECT_EQ(8, model->getColIdx(std::string("vr")));
    for(int i=0; i<model->_nCell; i++) {
        EXPECT_EQ(i+1, model->_cells[i].id);
        EXPECT_FLOAT_EQ( 0.10272E+14, model->_cells[i].ra);
        EXPECT_FLOAT_EQ( 0.14106E+14, model->_cells[i].rb);
        EXPECT_FLOAT_EQ( 0.11775E+08, model->_cells[i].nh);
        EXPECT_FLOAT_EQ( 0.23550E-01, model->_cells[i].nm);
        EXPECT_FLOAT_EQ( 0.81791E+02, model->_cells[i].tk);
        EXPECT_FLOAT_EQ( 0.12012E+00, model->_cells[i].db);
        EXPECT_FLOAT_EQ(-0.26418E+01, model->_cells[i].vr);
    }
}
