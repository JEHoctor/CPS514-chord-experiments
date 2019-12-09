//
// Created by Inchan Hwang on 2019-12-08.
//

#ifndef CHORD_SUCCESSOR_LIST_H
#define CHORD_SUCCESSOR_LIST_H

#include <vector>
#include "node.h"

class SuccessorList {
private:
    int myID;
    std::vector<Node> list;

public:
    explicit SuccessorList(int myID);

    Node successor();
    void addNode(Node node, int predID);
    void removeNode(int id);
    int size() { return list.size(); }
};


#endif //CHORD_SUCCESSOR_LIST_H
