//
// Created by Inchan Hwang on 2019-11-21.
//

#include "gtest/gtest.h"
#include "sha_wrapper.h"

TEST(sha_wrapper_test,test1) {
    std::string str = "localhost:8080";
    EXPECT_EQ(3301774746, extractID(str));
}
