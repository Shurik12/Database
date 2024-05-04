// Convert byte order:
// 1. htonl - host to network long 32
// 2. htons - host to network short 16
// 3. ntohl - network to host long 32
// 4. ntohs - network to host short 16
// 5. inet_pton - strptr(presentation) to addrptr(numeric)
// 6. inet_ntop - addrptr(numeric) to strptr(presentation)


#include "server.h"

using namespace std;

Server::Server(list* l_) {
    l = l_;
    listenfd = socket(AF_INET, SOCK_STREAM, 0); // socket file descriptor
    // fill server_addr structure by 0's
    memset(&server_sockaddr, 0, sizeof(server_sockaddr));
    server_sockaddr.sin_family = AF_INET; // default value (always)
    server_sockaddr.sin_port = htons(SERV_PORT); // htons - host-to-network
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); // fix constant
}

Server::~Server() {
    close(listenfd);
    delete l;
}

int main() {

    struct      sockaddr_in client_addr;
    int         listenfd, connfd; // listen and connection file descriptors
    pid_t       childpid; // child process id type
    char        buffer[1024];
    ifstream    fp ("b.txt");
    int         res {0};
    
    list* l = new list();

    if (!(fp.is_open())) {
	    cout << "Cannot open file\n";
	    return -1;
	}
    if (!(l -> read(fp))) {
		fp.close();
        cout << "Cannot read file\n";
        return -2;
	}
    fp.close();
    cout << "Read from file.\n";


    Server server(l);
    if (bind(server.listenfd, (struct sockaddr *) &server.server_sockaddr, sizeof(server.server_sockaddr)) == -1)
    {
        perror("bind");
        cout << "Bind exception.";
        return -1;
    }

    if (listen(server.listenfd, LISTEN_QUEUE) == -1)
    {
        perror("listen");
        cout << "Bind exception.";
        return -1;
    }

    for ( ; ; )
    {

        socklen_t client_length = sizeof(client_addr);
        connfd = accept(server.listenfd, (struct sockaddr*) &client_addr, &client_length);
        if (connfd < 0)
        {
            perror("connect");
            continue;
        }

        if (auto child_pid = fork(); child_pid == 0) /* child process */
        {
            close(listenfd); // close listening socket
            // str_echo(connfd); process the request
            while (true)
            {
                memset(buffer, 0, sizeof(buffer)); // fill memory with zeros
                int len = recv(connfd, buffer, sizeof(buffer), 0); // get data from socket and put them to buffer
                for (int j=0; buffer[j]; j++) {
                  if (buffer[j] == '\n') {
                    cout << "\n";
                    buffer[j] = 0;
                    break;
                  }
                }
                cout << "Got command: " << buffer << "\n";

                command *c = new command(); // initialize new object: command
                if ((c -> init(buffer)) < 0) {
                    cout << "Syntax error!\n";
                }
                res = c -> apply( l );
                delete c;

                if (res) break;
                send(connfd, buffer, len , 0);
            }
            exit (0);
        }
    }
    close(connfd); // parent closes connectes socket
    return 0;
}
