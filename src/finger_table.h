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
    string getAddr(int idx);
    void setAddr(int idx, string addr);
};

#endif //CHORD_FINGER_TABLE_H
