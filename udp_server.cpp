#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
// #include <pthread.h>
#include <thread>
#include <time.h>

const int buffer_size = 512;
const short portATP = 7001;
const short portATO = 7002;

void recvFromATP(void);
void recvFromATO(void);

sockaddr_in clientATPAddr;
sockaddr_in clientATOAddr;

int main()
{
    // pthread_t atpThread, atoThread;


    // thread join
    // auto atpID = pthread_create(&atpThread, NULL, recvFromATP, 0);
    // auto atoID = pthread_create(&atoThread, NULL, recvFromATO, 0);

    // pthread_join(atpThread, NULL);
    // pthread_join(atoThread, NULL);

    std::thread threadATP(recvFromATP);
    std::thread threadATO(recvFromATO);

    threadATP.join();
    threadATO.join();

    return 0;
}


// listen for ATP:7001
void recvFromATP(void)
{
    int sock;
    sockaddr_in serverAddr;
    int addrLen;
    int retval;
    char atpBuffer[buffer_size + 1];

    // socket init
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portATP);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(atpBuffer, buffer_size + 1);

    if(bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind error");
    }

    std::cout << "ATP UDP server start!" << std::endl;
    while (true)
    {
        // listen MSG
        // std::cout << "received from ATP" << std::endl;
        // sleep(1);

        // count++;
        // if(count > limit)
        //     break;
        retval = recvfrom(sock, atpBuffer, buffer_size, 0, (sockaddr*)&clientATPAddr, (socklen_t*)&addrLen);
        atpBuffer[retval] = '\0';
        std::cout << "[UDP server- " << inet_ntoa(clientATPAddr.sin_addr) << ":" << ntohs(clientATPAddr.sin_port) << "]" << atpBuffer << std::endl;
    }
}


// listen for ATO:7002
void recvFromATO(void)
{
    int sock;
    int retval;
    sockaddr_in serverAddr;
    char atoBuffer[buffer_size+1];
    int addrLen;

    // socket init
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portATO);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if(bind(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("bind error");
    }

    std::cout << "ATO UDP server start!" << std::endl;
    while (true)
    {
        // listen MSG
        // std::cout << "received from ATP" << std::endl;
        // sleep(1);

        // count++;
        // if(count > limit)
        //     break;
        retval = recvfrom(sock, atoBuffer, buffer_size, 0, (sockaddr*)&clientATPAddr, (socklen_t*)&addrLen);
        atoBuffer[retval] = '\0';
        std::cout << "[UDP server- " << inet_ntoa(clientATOAddr.sin_addr) << ":" << ntohs(clientATOAddr.sin_port) << "]" << atoBuffer << std::endl;
    }
}