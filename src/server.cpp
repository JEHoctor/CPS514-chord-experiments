//
// Created by Inchan Hwang on 2019-11-09.
//

#include <utility>
#include <string>
#include <grpcpp/grpcpp.h>
#include "context.h"
#include "generated/chord.grpc.pb.h"

using namespace std;
using chord::Chord;

void startServer(int m, Context myContext, const string& buddyAddrPort, Chord::Service* chordService) {
    grpc::ServerBuilder builder;
    builder.AddListeningPort("0.0.0.0:"+myContext.getPort(), grpc::InsecureServerCredentials());
    builder.RegisterService(chordService);
    unique_ptr<grpc::Server> server(builder.BuildAndStart());
    cout << "Server listening on " << myContext.getPort() << endl;
    server->Wait();
}
