#include <stdio.h>
#include <winsock2.h>

#define MAX_MSG_SIZE 1024

int main() {
    WSADATA data;
    int result = WSAStartup(MAKEWORD(2, 0), &data);
    if (result < 0) {
        printf("WSAStartup error\n");
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket < 0) {
        printf("Socket creation error\n");
        WSACleanup();
        return 1;
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;

    // Get server address and port from user input
    char serverAddress[20];
    int port;
    printf("Enter server address: ");
    scanf("%s", serverAddress);
    printf("Enter port number: ");
    scanf("%d", &port);

    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress);

    // Connect to the server
    result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result < 0) {
        printf("Connection error\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Get player name from user input
    char playerName[1024];
    printf("Enter your name: ");
    scanf("%s", playerName);

    // Send player name to the server
    result = send(clientSocket, playerName, strlen(playerName), 0);
    if (result < 0) {
        printf("Send error\n");
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    char message[MAX_MSG_SIZE];

    while (1) {
        // Receive message from server
        result = recv(clientSocket, message, sizeof(message), 0);
        if (result <= 0) {
            printf("Receive error or connection closed\n");
            break;
        }

        // Display received message
        printf("Received message: %s\n", message);

        // Get user input for the next move
        printf("Enter your move (e.g., '4,5'): ");
        scanf("%s", message);

        // Send the move to the server
        result = send(clientSocket, message, strlen(message), 0);
        if (result < 0) {
            printf("Send error\n");
            break;
        }
    }

    // Close the socket and cleanup
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
