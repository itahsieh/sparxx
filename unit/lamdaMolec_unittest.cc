#include <iostream>
#include <fstream>
#include <memory>
#include <exception>
#include "lamdaMolec.h"
#include "gtest/gtest.h"

TEST(LamdaMolec, loadDat) {
    //Pointer for handling model
    std::auto_ptr<LamdaMolec> molec(new LamdaMolec);

    //Load molec from .dat file
    molec->loadDat("hco+.dat");
    EXPECT_EQ(2, molec->nlev);
    EXPECT_EQ(1, molec->nlin);
    EXPECT_EQ(1, molec->ncoll);
    ASSERT_EQ(1, static_cast<int>(molec->collData.size()));
    EXPECT_EQ(CollPartner::H2, molec->collData[0].partnerId);
    EXPECT_EQ(2, molec->collData[0].nTemps);
    EXPECT_EQ(10.0, molec->collData[0].temps[0]);
    EXPECT_EQ(20.0, molec->collData[0].temps[1]);
    EXPECT_EQ(1, molec->collData[0].nTrans);
    ASSERT_EQ(1, static_cast<int>(molec->collData[0].cxTab.size()));
    EXPECT_EQ(2, molec->collData[0].cxTab[0].upperLev);
    EXPECT_EQ(1, molec->collData[0].cxTab[0].lowerLev);
    EXPECT_EQ(2.6e-10, molec->collData[0].cxTab[0].collRates[0]);
    EXPECT_EQ(2.3e-10, molec->collData[0].cxTab[0].collRates[1]);
}
