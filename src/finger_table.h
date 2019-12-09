//
// Created by Inchan Hwang on 2019-11-07.
//

#ifndef CHORD_FINGER_TABLE_H
#define CHORD_FINGER_TABLE_H

#include <string>
#include <stdlib.h>
#include <node.h>
#include <unordered_map>

class FingerTable {
private:
    std::unordered_map<int, Node> tbl;

public:
    explicit FingerTable();

    bool getNode(int idx, Node* dst);
    void setNode(int idx, Node node);
    void delNode(int idx);
};

#endif //CHORD_FINGER_TABLE_H
