//
// Created by Inchan Hwang on 2019-11-07.
//

#include "gtest/gtest.h"
#include "finger_table.h"

TEST(finger_table_test,test1) {
    FingerTable tbl(10);

    tbl.setAddr(0, "one");
    tbl.setAddr(1, "two");

    EXPECT_EQ(tbl.getAddr(0), "one");
    EXPECT_EQ(tbl.getAddr(1), "two");

    EXPECT_ANY_THROW(
            tbl.setAddr(10, "huh");
    );
}
