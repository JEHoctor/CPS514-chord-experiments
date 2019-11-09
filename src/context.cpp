#include <utility>

//
// Created by Inchan Hwang on 2019-11-07.
//

#include <string>
#include "context.h"

using namespace std;

Context::Context(int m, string addr, string port): m(m), addr(std::move(addr)), port(std::move(port)) {}

string Context::getAddr() { return addr; }

string Context::getPort() { return port; }

string Context::getSucc() { return succ; }
void Context::setSucc(string succ_) { succ = move(succ_); }

string Context::getPred() { return pred; }
void Context::setPred(string pred_) { pred = move(pred_); }

chord::Node Context::dumpProto() {
    chord::Node node;
    node.set_addr(addr + ":" + port);
    node.set_m(m);
    node.set_pred(pred);
    node.set_succ(succ);
    return node;
}
