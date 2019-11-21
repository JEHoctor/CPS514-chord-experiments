#include <iostream>
#include <string>
#include "server.h"
#include "chord_impl.h"

int main(int argc, char **argv) {
    if(argc != 3) {
        printf(
                "Requires two arguments - host / port\n"
                "ex) ./chord localhost 8080\n"
        );
        return 1;
    }

    string myHost = argv[1];
    string myPort = argv[2];

    Node me(myHost+":"+myPort);

    Context myContext(&me);

    ChordImpl chord(&myContext);

    startServer(myPort, &chord);

    return 0;
}