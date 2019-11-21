//
// Created by Inchan Hwang on 2019-11-21.
//

#ifndef CHORD_NODE_H
#define CHORD_NODE_H

#include <cstdint>
#include <string>
#include <sha_wrapper.h>
#include "generated/chord.grpc.pb.h"

class Node {
private:
    std::string addr;
    uint32_t id;

public:
    explicit Node(const std::string& addr);

    chord::Node* genProto();
};


#endif //CHORD_NODE_H
