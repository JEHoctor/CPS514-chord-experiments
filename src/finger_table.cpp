//
// Created by Inchan Hwang on 2019-11-07.
//

#include <string>
#include <vector>
#include "finger_table.h"

using namespace std;

FingerTable::FingerTable(int size_): size(size_) {
    tbl.reserve(size);
    for(int i = 0 ; i < size ; i ++) {
        tbl.emplace_back("");
    }
}

string FingerTable::getAddr(int idx) {
    return tbl[idx];
}

void FingerTable::setAddr(int idx, string addr) {
    tbl.at(idx) = move(addr);
}
