//
// Created by Inchan Hwang on 2019-11-07.
//

#ifndef CHORD_CONTEXT_H
#define CHORD_CONTEXT_H

#include <string>
#include "generated/chord.pb.h"

using namespace std;

class Context {
private:
    int m;
    string addr;
    string port;
    string succ = "";
    string pred = "";

public:
    Context(int m, string addr, string port);

    string getAddr();

    string getPort();

    string getSucc();
    void setSucc(string succ_);

    string getPred();
    void setPred(string pred_);

    chord::Node dumpProto();
};

#endif //CHORD_CONTEXT_H
