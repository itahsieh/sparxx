#include <iostream>
#include <fstream>
#include <memory>
#include <exception>
#include "lamdaMolec.h"
#include "gtest/gtest.h"

TEST(LamdaMolec, loadDat) {
    //Pointer for handling model
    std::auto_ptr<LamdaMolec> molec(new LamdaMolec);

    try {
      //Load molec from .dat file
      molec->loadDat("hco+.dat");
      EXPECT_EQ(21, molec->nlev);
      EXPECT_EQ(20, molec->nlin);
      EXPECT_EQ(1, molec->ncoll);
      ASSERT_EQ(1, static_cast<int>(molec->collData.size()));
      EXPECT_EQ(12, molec->collData[0].nTemps);
      EXPECT_EQ(210, molec->collData[0].nTrans);
    }
    catch(std::exception &e) {
      std::cerr << "Exception raised: " << e.what() << std::endl;
    }

    //Test molec contents
}
