//
// Created by Inchan Hwang on 2019-11-21.
//

#ifndef CHORD_CHORD_CLIENT_H
#define CHORD_CHORD_CLIENT_H


#include <grpcpp/grpcpp.h>
#include "generated/chord.grpc.pb.h"
#include "node.h"

using grpc::Channel;
using chord::Chord;


class ChordClient {
public:
    ChordClient() = default;

    // remote handlers
    bool getInfo(Node target, chord::NodeInfo* dst);
    bool findSucc(Node target, uint32_t key, Node* dst);
    bool findPred(Node target, uint32_t key, Node* dst);
    void getClosestFinger(Node target, uint32_t key, Node* dst);
};


#endif //CHORD_CHORD_CLIENT_H
