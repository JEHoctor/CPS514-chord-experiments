//
// Created by Inchan Hwang on 2019-11-07.
//

#include "gtest/gtest.h"
#include "context.h"

TEST(context_test,test1) {
    Context ctx(10, "localhost", "8080");

    ctx.setSucc("localhost:8081");
    ctx.setPred("localhost:8082");

    chord::Node node = ctx.dumpProto();

    EXPECT_EQ(node.addr(), "localhost:8080");
    EXPECT_EQ(node.m(), 10);
    EXPECT_EQ(node.succ(), "localhost:8081");
    EXPECT_EQ(node.pred(), "localhost:8082");
}
