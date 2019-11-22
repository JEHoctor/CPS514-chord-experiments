#include <utility>

//
// Created by Inchan Hwang on 2019-11-21.
//

#include "node.h"

Node::Node() {
    addr = "";
    id = 0;
    isValid = false;
}

Node::Node(const std::string &addr): addr(addr) {
    id = extractID(addr);
    isValid = true;
}

Node::Node(const chord::Node& proto) {
    if(proto.is_valid()) set(proto.addr());
    else isValid = false;
}

void Node::set(std::string addr_) {
    this->addr = std::move(addr_);
    this->id = extractID(addr);
    this->isValid = true;
}

std::string Node::getAddr() { return this->addr; }
uint32_t Node::getID() { return this->id; }
bool Node::getIsValid() { return this->isValid; }

chord::Node* Node::genProto() {
    auto* proto = new chord::Node();
    proto->set_addr(this->addr);
    proto->set_id(this->id);
    proto->set_is_valid(this->isValid);
    return proto;
}