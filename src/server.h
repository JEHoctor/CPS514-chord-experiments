//
// Created by Inchan Hwang on 2019-11-09.
//

#ifndef CHORD_SERVER_H
#define CHORD_SERVER_H

#include <string>
#include "context.h"
#include "generated/chord.grpc.pb.h"

using namespace std;
using chord::Chord;

void startServer(int m, Context myContext, const string& buddyAddrPort, Chord::Service* chordService);

#endif //CHORD_SERVER_H
