//
// Created by Inchan Hwang on 2019-12-08.
//

#include "successor_list.h"

SuccessorList::SuccessorList(int myID): myID(myID) { }

Node SuccessorList::successor() {
    return list.empty() ? Node() : list[0];
}

void SuccessorList::addNode(Node node, int predID) {
    if(!node.getIsValid()) {
        list.erase(list.begin());
        return;
    }

    bool exists = false;
    for(auto& i : list) {
        exists |= i.getID() == node.getID();
    }

    if(exists) return;

    if(predID == myID) {
        list.insert(list.begin(), node);
    } else {
        for(int i = 0 ; i < list.size() ; i ++) {
            if(list[i].getID() == predID) {
                list.insert(list.begin()+i+1, node);
                break;
            }
        }
    }

    while(list.size() > 32) list.pop_back();
}

void SuccessorList::removeNode(int id) {
    for(int i = 0 ; i < list.size() ; i ++) {
        if(list[i].getID() == id) {
            list.erase(list.begin()+i);
            i --;
        }
    }

}
