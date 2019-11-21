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
    bool isValid;

public:
    Node();
    explicit Node(const std::string& addr);
    explicit Node(const chord::Node& proto);

    void set(std::string addr_);
    std::string getAddr();
    uint32_t getID();
    bool getIsValid();

    chord::Node* genProto();
};


#endif //CHORD_NODE_H
