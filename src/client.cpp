#include "client.h"

using namespace std;

int main()
{
    int i, j, k, buff_len = 1024;
    ifstream fp ("commands2.txt");

    int sock_cli = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in servaddr {};
    memset(&servaddr, 0, sizeof(servaddr)); // fill servaddr 0`s
    servaddr.sin_family = AF_INET; // default value (always)
    servaddr.sin_port = htons(MYPORT);  /// Server Port
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // server ip

    //Connect to the server, successfully return 0, error return - 1
    if (connect(sock_cli, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(1);
    }

    char sendbuf[buff_len];
    char recvbuf[buff_len];

    // open file
    if (!(fp.is_open())) {
	    cout << "Cannot open file\n";
	    return -1;
	}

    // Read and execute commands from file
    while (fp.getline(sendbuf, buff_len)) 
    {
        for (j = 0; sendbuf[j]; j++) 
        {
            if (sendbuf[j] == '\n') 
            {
                sendbuf[j] = 0;
                break;
            }
        }
        cout << "Request to server: " << sendbuf << "\n";
        send(sock_cli, sendbuf, strlen(sendbuf), 0); /// Send
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0); /// Receiving
        cout << "Answer from server: " << recvbuf << "\n";

        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));        
    }

    // Read and process commands from standart input
    while (cin.getline(sendbuf, buff_len)) 
    {
        for (j = 0; sendbuf[j]; j++) 
        {
            if (sendbuf[j] == '\n') 
            {
                sendbuf[j] = 0;
                break;
            }
        }
        cout << "Request to server: " << sendbuf << "\n";
        send(sock_cli, sendbuf, strlen(sendbuf), 0); /// Send
        recv(sock_cli, recvbuf, sizeof(recvbuf), 0); /// Receiving
        cout << "Answer from server: " << recvbuf << "\n";

        if (recvbuf == "Bye!")
        {
            memset(sendbuf, 0, sizeof(sendbuf));
            memset(recvbuf, 0, sizeof(recvbuf));
            break;
        }
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    close(sock_cli);
    return 0;
}