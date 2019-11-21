//
// Created by Inchan Hwang on 2019-11-07.
//

#ifndef CHORD_CONTEXT_H
#define CHORD_CONTEXT_H

#include "node.h"
#include "finger_table.h"
#include "generated/chord.grpc.pb.h"

class Context {
private:
    Node me;
    Node succ;
    Node pred;
    FingerTable tbl;

public:
    explicit Context(Node me);

    Node getMe();
    Node getSucc();
    Node getPred();

    void setSucc(Node succ_);
    void setPred(Node pred_);
    void setFinger(int idx, Node node);
    bool getFinger(int idx, Node* dst);

    chord::NodeInfo* genProto();
};

#endif //CHORD_CONTEXT_H
