#include <utility>

//
// Created by Inchan Hwang on 2019-11-07.
//

#include <string>
#include <vector>
#include "finger_table.h"

using namespace std;

FingerTable::FingerTable() = default;

Node* FingerTable::getNode(int idx) {
    if(tbl.find(idx) == tbl.end()) {
        return nullptr;
    } else {
        return tbl[idx];
    }
}

void FingerTable::setNode(int idx, Node* node) {
    tbl[idx] = std::move(node);
}
