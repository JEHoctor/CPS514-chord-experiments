//
// Created by Inchan Hwang on 2019-11-07.
//

#include "gtest/gtest.h"
#include "finger_table.h"

TEST(finger_table_test,test1) {
    FingerTable tbl;

    auto n0 = Node("localhost:8081");
    auto n2 = Node("localhost:8081");

    tbl.setNode(0, &n0);
    tbl.setNode(2, &n2);

    EXPECT_EQ(tbl.getNode(0)->genProto()->addr(), "localhost:8081");
    EXPECT_EQ(tbl.getNode(1), nullptr);
}
