//
// Created by Inchan Hwang on 2019-11-07.
//

#ifndef CHORD_FINGER_TABLE_H
#define CHORD_FINGER_TABLE_H

#include <string>
#include <stdlib.h>
#include <vector>

using namespace std;

class FingerTable {
private:
    int size;
    vector<string> tbl;

public:
    explicit FingerTable(int size);

    // "" if the address of the node isn't found yet
    string getAddr(int idx) { return tbl[idx]; }

    void setAddr(int idx, const string& addr) {
        if(0 <= idx && idx < size) {
            tbl[idx] = addr;
        } else {
            throw("FingerTable index out of bounds\n");
        }
    }
};

#endif //CHORD_FINGER_TABLE_H
