//
// Created by Inchan Hwang on 2019-11-07.
//

#include "gtest/gtest.h"
#include "context.h"
#include "node.h"

TEST(context_test,test1) {
    Node me("localhost:8080");
    Node succ("localhost:8081");
    Node pred("localhost:8082");

    Context ctx(me);
    ctx.setSucc(succ);
    ctx.setPred(pred);

    ASSERT_EQ("localhost:8080", ctx.genProto()->mutable_self()->addr());
    ASSERT_EQ("localhost:8081", ctx.genProto()->mutable_succ()->addr());
    ASSERT_EQ("localhost:8082", ctx.genProto()->mutable_pred()->addr());
}
