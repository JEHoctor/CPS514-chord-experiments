#include <utility>

//
// Created by Inchan Hwang on 2019-11-07.
//

#include <string>
#include <vector>
#include "finger_table.h"

using namespace std;

FingerTable::FingerTable() = default;

bool FingerTable::getNode(int idx, Node* dst) {
    if(tbl.find(idx) != tbl.end()) {
        dst->set(tbl[idx].getAddr());
        return true;
    }
    return false;
}

void FingerTable::setNode(int idx, Node node) {
    tbl[idx] = std::move(node);
}
