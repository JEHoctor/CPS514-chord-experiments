// Behold the power of gradware!

#include <iostream>
#include <string>
#include "server.h"
#include "chord_client.h"
#include "chord_impl.h"

using namespace std;


//----- Data structure to hold arguments for client mode operation.
enum ClientAction { client_get_info,
                    client_find_succ,
                    client_find_pred,
                    client_get_closest_finger,
                    client_notify };

struct ClientArguments
{
    string otherHost;
    string otherPort;
    //string myHost;
    //string myPort;
    ClientAction action;
};
//-----


//----- Data structure to hold arguments for server mode operation.
struct ServerArguments
{
    string myHost;
    string myPort;
    bool other_node_indicated;
    string otherHost;
    string otherPort;
};
//-----


//----- Top level data structure to hold arguments of either mode, and an enum to facilitate interpretation.
enum RunMode { run_as_client, run_as_server };

struct ParsedArguments
{
    RunMode run_mode;

    union
    {
        ClientArguments *client_arguments;
        ServerArguments *server_arguments;
    };
};
//-----


//----- Functions for interpreting the command line arguments
int print_usage(char **argv) {
    cout << "usage: " << argv[0] << " client <client_action> <server_host>:<server_port>" << endl;
    cout << "   or: " << argv[0] << " server <my_host>:<my_port> [-t <other_host>:<other_port>]" << endl << endl;

    cout << "Where <client_action> is one of:" << endl;
    cout << "   get-info" << endl;
    cout << "   find-successor" << endl;
    cout << "   find-predecessor" << endl;
    cout << "   find-closest-finger" << endl;
    cout << "   notify" << endl;

    return EXIT_FAILURE;
}

int parse_host_colon_port(const string& input, string& host, string& port) {
    size_t colon_loc = input.find(':');
    if(colon_loc == string::npos) { // colon character not found
        cout << "Problem parsing <host>:<port>" << endl;
        cout << "No colon character found." << endl;
        return 1; // failure
    }
    // TODO: Validate these strings.
    host = input.substr(0, colon_loc);
    port = input.substr(colon_loc + 1);
    return 0; //success
}

int parse_arguments(ParsedArguments& dest, int argc, char **argv) {
    int status = 0;
    if(argc < 2) {
        cout << "Not enough command line arguments." << endl;
        return print_usage(argv);
    }
    else {
        string mode_string(argv[1]);
        if(mode_string == "client") {
            dest.run_mode = run_as_client;
            dest.client_arguments = new ClientArguments();
            if(argc != 4) {
                cout << "Wrong number of command line arguments for use with client operating mode." << endl;
                return print_usage(argv);
            }
            string client_action_string(argv[2]);
            if(client_action_string == "get-info") {
                dest.client_arguments->action = client_get_info;
            }
            else if(client_action_string == "find-successor") {
                dest.client_arguments->action = client_find_succ;
            }
            else if(client_action_string == "find-predecessor") {
                dest.client_arguments->action = client_find_pred;
            }
            else if(client_action_string == "find-closest-finger") {
                dest.client_arguments->action = client_get_closest_finger;
            }
            else if(client_action_string == "notify") {
                dest.client_arguments->action = client_notify;
            }
            else {
                cout << "Invalid <client_action>." << endl;
                return print_usage(argv);
            }
            string server_host_colon_port_string(argv[3]);
            status = parse_host_colon_port(server_host_colon_port_string, dest.client_arguments->otherHost, dest.client_arguments->otherPort);
            if(1 == status) {
                cout << "The problem was encountered in <server_host>:<server_port>" << endl;
                return print_usage(argv);
            }
        }
        else if (mode_string == "server") {
            dest.run_mode = run_as_server;
            dest.server_arguments = new ServerArguments();
            dest.server_arguments->other_node_indicated = false;
            string dash_t_string;
            string existing_server_host_colon_port_string;
            string my_host_colon_port_string;
            switch(argc) {
                case 5:
                        dash_t_string = argv[3];
                        if(dash_t_string != "-t") {
                            cout << "When specifiying an existing chord server, use -t." << endl;
                            return print_usage(argv);
                        }
                        existing_server_host_colon_port_string = argv[4];
                        status = parse_host_colon_port(existing_server_host_colon_port_string, dest.server_arguments->otherHost, dest.server_arguments->otherPort);
                        if(1 == status) {
                            cout << "The problem was encountered in <other_host>:<other_port>" << endl;
                            return print_usage(argv);
                        }
                        dest.server_arguments->other_node_indicated = true;
                case 3:
                        my_host_colon_port_string = argv[2];
                        status = parse_host_colon_port(my_host_colon_port_string, dest.server_arguments->myHost, dest.server_arguments->myPort);
                        if(1 == status) {
                            cout << "The problem was encountered in <my_host>:<my_port>" << endl;
                            return print_usage(argv);
                        }
                        break;
                default:
                        cout << "Wrong number of command line arguments for use with server operating mode." << endl;
                        return print_usage(argv);
            }
        }
        else {
            cout << "Invalid operating mode '" << mode_string << "'." << endl;
            return print_usage(argv);
        }
    }
    return 0;
}
//-----


//----- Specialized main functions for client and server mode.
int server_main(ServerArguments &server_arguments) {
    std::srand(std::time(nullptr)); // use current time as seed for random generator

    Node me(server_arguments.myHost+":"+server_arguments.myPort);

    Context myContext(me);

    if(server_arguments.other_node_indicated) {
        Node other(server_arguments.otherHost+":"+server_arguments.otherPort);
        ChordClient client;
        Node succ;
        uint32_t key = me.getID();
        bool success = client.findSucc(other, key, &succ);
        if(!success) {
            cout << "Failed to use other node to find successor of this new node. Exiting." << endl;
            return EXIT_FAILURE;
        }
        myContext.setSucc(succ);
    }

    ChordImpl chord(myContext);

    startServer(server_arguments.myPort, &chord);

    return EXIT_SUCCESS;
}

int client_main(ClientArguments &client_arguments) {
    ChordClient client;
    Node other(client_arguments.otherHost+":"+client_arguments.otherPort);

    if(client_arguments.action == client_get_info) {
        chord::NodeInfo info;
        bool status = client.getInfo(other, &info);
        if(!status) {
            cout << "Failed to get get the status of that chord server." << endl;
            return EXIT_FAILURE;
        }

        if(info.has_self()) {
            cout << "The reponse contains a self node:" << endl;
            const chord::Node &others_self_node = info.self();
            cout << "       addr:" << others_self_node.addr() << endl;
            cout << "         id:" << others_self_node.id() << endl;
            cout << "   is_valid:" << others_self_node.is_valid() << endl;
        }
        else {
            cout << "The reponse contains no self node." << endl;
        }

        if(info.has_succ()) {
            cout << "The reponse contains a successor node:" << endl;
            const chord::Node &others_succ_node = info.succ();
            cout << "       addr:" << others_succ_node.addr() << endl;
            cout << "         id:" << others_succ_node.id() << endl;
            cout << "   is_valid:" << others_succ_node.is_valid() << endl;
        }
        else {
            cout << "The reponse contains no sucessor node." << endl;
        }

        if(info.has_pred()) {
            cout << "The reponse contains a predecessor node:" << endl;
            const chord::Node &others_pred_node = info.pred();
            cout << "       addr:" << others_pred_node.addr() << endl;
            cout << "         id:" << others_pred_node.id() << endl;
            cout << "   is_valid:" << others_pred_node.is_valid() << endl;
        }
        else {
            cout << "The reponse contains no predecessor node." << endl;
        }
    }
    else if(client_arguments.action == client_find_succ) {
        cout << "Error. Not implemented." << endl;
        return EXIT_FAILURE;
    }
    else if(client_arguments.action == client_find_pred) {
        cout << "Error. Not implemented." << endl;
        return EXIT_FAILURE;
    }
    else if(client_arguments.action == client_get_closest_finger) {
        cout << "Error. Not implemented." << endl;
        return EXIT_FAILURE;
    }
    else if(client_arguments.action == client_notify) {
        cout << "Error. Not implemented." << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
//-----


int main(int argc, char **argv) {
    ParsedArguments parsed_arguments;

    int status = parse_arguments(parsed_arguments, argc, argv);
    if(status) {
        return status;
    }

    if(parsed_arguments.run_mode == run_as_client) {
        return client_main( *(parsed_arguments.client_arguments) );
    }
    else /*if(parsed_arguments.run_mode == run_as_server)*/ {
        return server_main( *(parsed_arguments.server_arguments) );
    }
}
