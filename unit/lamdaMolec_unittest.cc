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
    }
    catch(std::exception &e) {
      std::cerr << "Exception raised: " << e.what() << std::endl;
    }

    //Test molec contents
}
