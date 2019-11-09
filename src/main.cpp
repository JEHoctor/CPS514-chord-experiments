#include <iostream>
#include <string>
#include "server.h"
#include "chord_impl.h"

int main(int argc, char **argv) {
    if(argc != 5) {
        printf(
                "Requires four arguments - m / host / port / buddy's host:port\n"
                "ex) ./chord 10 localhost 8080 localhost:8081\n"
        );
        return 1;
    }

    int m = stoi(argv[1]);
    string myHost = argv[2];
    string myPort = argv[3];
    string buddyHostPort = argv[4];

    printf("m=%d myHost=%s myPort=%s buddyHostPort=%s\n",
            m, myHost.c_str(), myPort.c_str(), buddyHostPort.c_str());

    Context myContext(m, myHost, myPort);

    ChordImpl chord(myContext);

    startServer(m, myContext, buddyHostPort, &chord);

    return 0;
}