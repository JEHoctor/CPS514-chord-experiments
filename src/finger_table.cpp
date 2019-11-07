//
// Created by Inchan Hwang on 2019-11-07.
//

#include "finger_table.h"
#include <string>
#include <vector>

using namespace std;

FingerTable::FingerTable(int size) {
    this->size = size;
    tbl.reserve(size);
    for(int i = 0 ; i < size ; i ++) {
        tbl.emplace_back("");
    }
}
