/* Faculty of Information Technology, Brno University of Technology
*  Subject: IPK (Computer Communications and Networks) - Project 1
*  Description: Client/Server socket communication
*  Date created: February 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: ftrest.cpp - client side
*/

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include <regex>
#include <sys/types.h> // connect()
#include <sys/stat.h> // stat()
#include <sys/socket.h> // connect(), send(), bind(), accept()
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>  // perror()
#include <ctime>
#include <iomanip>

#define BUFF_LEN 1024

using namespace std;

int getFileSize(string filename)
{
    FILE *p_file = NULL;
    p_file = fopen(filename.c_str(),"rb");
    if (fseek(p_file, 0, SEEK_END) == -1) {
            perror("ERROR: fseek");
            exit(EXIT_FAILURE);
    }
    int size = ftell(p_file);
    fclose(p_file);
    return size;
}

int main(int argc, char * argv[]) {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFF_LEN];

    string command;
    int commandNum;

    string hostname;
    int portNum = 6677;
    string userAccount;
    string remotePath;

    string localPath = "none";

    string head = "";
    long long byteCountSend, byteCountRead = 0;

    // localPath properites
    bool dirFlag = false;
    bool fileFlag = false;
    unsigned long size = 0;

    /* ------------------- ARGUMENT RECOGNITION -------------------- */
    
    string commands[] = {"del", "get", "put", "lst", "mkd", "rmd"};
    if (argc > 4) {
        perror("ERROR: Unsupported number of Arguments");
        exit(EXIT_FAILURE);
    } else if (argc > 2) {
        command = argv[1];
        bool valid = false;
        for (int i = 0; i < 6; i++) {
            if (strcmp(command.c_str(),commands[i].c_str()) == 0) {
                valid = true;
                commandNum = i;
            }
        }
        if (!valid) {
            perror("ERROR: Unsupported command");
            exit(EXIT_FAILURE);
        }
        remotePath = argv[2];

    } else {
        cout << "Program usage:     ftrest COMMAND REMOTE-PATH [LOCAL-PATH]" << endl << endl;
        cout << "                   - COMMAND        - One of [del|get|put|lst|mkd|rmd]." << endl;
        cout << "                   - REMOTE-PATH    - Try \"http://localhost:12345/foo/bar\" as example." << endl;
        cout << "                   - LOCAL-PATH     - Is neccesary for \"put\" command." << endl;
        return 0;
    }
    if (argc == 4) {
        localPath = argv[3];
        if (commandNum == 2 || commandNum == 1) {
            if (((access(localPath.c_str(), R_OK)) != 0)) {
                perror("ERROR: Local Path is not readable");
                exit(EXIT_FAILURE);   
            }
            struct stat s;
            if ( stat(localPath.c_str(),&s) == 0 )
            {
                if ( s.st_mode & S_IFDIR ) {
                    dirFlag = true;
                  //  perror("ERROR: Cannot put a folder to server, put command is used for files only");
                  //  exit(EXIT_FAILURE);                    
                }
                else if ( s.st_mode & S_IFREG ) {
                    fileFlag = true;
                    size = getFileSize(localPath);
                } 
                else {
                    perror("ERROR: Local Path is not a File nor Directory");
                    exit(EXIT_FAILURE); 
                }
            } 
            else {
                perror("ERROR: Unsupported Local Path");
                exit(EXIT_FAILURE);
            }
        }
    }

    /* -------------- REMOTE PATH PARSING --------------- */

   /* if (!regex_match (remotePath, regex("(http://)[:alpha:]+(:[:digit:]{4,5})*(.*)") )) {
            perror("ERROR: Unsuportable remote path");
            exit(EXIT_FAILURE); 
    } */
    bool noPort = false;
    remotePath.erase(0, 7); // remove "http://"
    /* set hostname */
    int n = remotePath.find(':');
    if (n == -1) {noPort = true; n = remotePath.find('/');}
    hostname = remotePath.substr(0, n);
    remotePath.erase(0, n+1); // remove hostname
    /* set portNum */
    n = remotePath.find('/');
    if (noPort == false) {
        portNum = atoi(remotePath.substr(0, n).c_str());
        remotePath.erase(0, n+1); // remove portNum
    }
    /* set userAccount */
    n = remotePath.find('/');
    userAccount = remotePath.substr(0, n);
    remotePath.erase(0, n); // remove userAccount

    /* resolve hostname */
    struct hostent * phostent;
    if ((phostent = gethostbyname(hostname.c_str()) ) == NULL ) {
        perror("ERROR: Domain addres not found");
        exit(EXIT_FAILURE); 
    }

    /* copy the network address to sockaddr_in structure */
    memcpy(&serverAddr.sin_addr, phostent->h_addr_list[0], phostent->h_length);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(portNum);
    
    /* extract main IP address */ 
    in_addr ip_addr;    
    ip_addr.s_addr = *(unsigned long*) phostent->h_addr_list[0]; 
 /*   
    cout << "userAccount: " << userAccount << endl; 
    cout << "remotePath: " << remotePath << endl;
    cout << "hostname: " << hostname << endl;
    cout << "portNum: " << portNum << endl;
    cout << "phostent: " << inet_ntoa(ip_addr) << endl;
    cout << "localPath: " << localPath << endl;
    cout << "File: " << fileFlag << endl;
    cout << "Directory: " << dirFlag << endl;
    cout << "Size: " << size << endl;
*/
    /* ------------------- FOLDER / FILE CHECK ------------------- */
    // string commands[] = {"del", "get", "put", "lst", "mkd", "rmd"};
    if (commandNum == 3 && dirFlag == true ) {
        perror("ERROR: Incompatible command and local path");
        exit(EXIT_FAILURE);
    }
/*    if (commandNum > 3 && fileFlag == true ) {
        perror("ERROR: Incompatible command and local path");
        exit(EXIT_FAILURE);
    }
*/
    /* ------------------- SOCKET DEFINITION -------------------- */
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    if ((clientSocket = socket(PF_INET, SOCK_STREAM, 0)) <= 0) {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
  
    /* -------------- ADDRESS STRUCT DEFINITION --------------- */ 
    /*---- Configure settings of the server address struct ----*/
    /* Address family = Internet */
//    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
//    serverAddr.sin_port = htons(portNum);
    /* Set IP address to localhost */
//    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);
    /* Set all bits of the padding field to 0 */
//    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero)); 

    /* -------------- STRUCT - ADDRESS CONNECTING --------------- */
    /*---- Connect the socket to the server using the address struct ----*/
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0) {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);
    }

    /* -------------------- CREATING HEADER -------------------- */
    // string commands[] = {"del", "get", "put", "lst", "mkd", "rmd"};
    stringstream headStream;
    switch (commandNum) {
        case 0: headStream << "DEL /" << userAccount << remotePath << "?type=" << "file" << " HTTP/1.1" << endl; break;
        case 1: headStream << "GET /" << userAccount << remotePath << "?type=" << "file" << " HTTP/1.1" << endl; break;
        case 2: headStream << "PUT /" << userAccount << remotePath << "?type=" << "file" << " HTTP/1.1" << endl; break;
        case 3: headStream << "GET /" << userAccount << remotePath << "?type=" << "folder" << " HTTP/1.1" << endl; break;
        case 4: headStream << "PUT /" << userAccount << remotePath << "?type=" << "folder" << " HTTP/1.1" << endl; break;
        case 5: headStream << "DEL /" << userAccount << remotePath << "?type=" << "folder" << " HTTP/1.1" << endl; break;
    }
    /*
    PUT /tonda/foo/bar/doc.pdf?type=file HTTP/1.1
    Host: somehost.org
    Date: Wed, 22 Feb 2017 15:50:00 CET
    Accept: application/json
    Accept-Encoding: identity
    Content-Type: application/octet-stream
    Content-Length: 21557
    */

    int length = 0;

    time_t rawtime;
    struct tm * timeinfo;
    char buffTime[100];

    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffTime, sizeof(buffTime),"%a, %d %b %Y %X CET", timeinfo);
    string str(buffTime);

    headStream << "Host: " << hostname << endl;
    headStream << "Date: " << str << endl;
    headStream << "Accept: application/json" << endl;
    headStream << "Accept-Encoding: identity" << endl;
    if (commandNum == 2) {
        headStream << "Content-Type: application/octet-stream" << endl; // TODO
        headStream << "Content-Length: " << size << endl << endl;
    } else {
        headStream << endl;
    }

 //   headStream << "010101011010101001111111111111111111111100";
    head = headStream.str();

    // cout << head << endl;


    /* -------------------- SENDING SOCKET -------------------- */

    memset(buffer, 0, BUFF_LEN);
    strcpy(buffer, head.c_str());

    byteCountSend = send(clientSocket, buffer, BUFF_LEN, 0);
    if (byteCountSend < 0)
        perror("ERROR: sendto");

    /* -------------------- RECEIVING SOCKET -------------------- */

    memset(buffer, 0, BUFF_LEN);

    /*---- Read the message from the server into the buffer ----*/
    byteCountRead = recv(clientSocket, buffer, BUFF_LEN, 0);
    if (byteCountRead < 0)
        perror("ERROR: recvfrom");

    /*---- Print the received message ----*/
    string receivedMessage = buffer;

    if (receivedMessage.find("HTTP/1.1 401 Unauthorized") == 0) {
        fprintf(stderr, "User Account Not Found\n");
        return(EXIT_FAILURE);
    }
    if (command == "lst" || command == "rmd") {
        int h;
        if (receivedMessage.find("HTTP/1.1 200 OK") == 0) {
            if (command == "lst") {
                for (int k = 0; k < 6; k++) {
                    h = receivedMessage.find("\n");
                    receivedMessage.erase(0, h+1);
                }
                cout << receivedMessage << endl;
            }
        }
        if (receivedMessage.find("HTTP/1.1 400 Bad Request") == 0) {
            fprintf(stderr, "Not a directory.\n");
            return(EXIT_FAILURE);
        }
        if (receivedMessage.find("HTTP/1.1 404 Not Found") == 0) {
            fprintf(stderr, "Directory not found.\n");
            return(EXIT_FAILURE);
        }
        if (receivedMessage.find("HTTP/1.1 304 Not Modified") == 0) {
            fprintf(stderr, "Directory not empty.\n");
            return(EXIT_FAILURE);
        }
    }
    if (command == "mkd") {
        if (receivedMessage.find("HTTP/1.1 304 Not Modified") == 0) {
        fprintf(stderr, "Already exists.\n");
        return(EXIT_FAILURE);
        }
    }
    if (command == "del") {
        if (receivedMessage.find("HTTP/1.1 400 Bad Request") == 0) {
            fprintf(stderr, "Not a file.\n");
            return(EXIT_FAILURE);
        }
        if (receivedMessage.find("HTTP/1.1 404 Not Found") == 0) {
            fprintf(stderr, "File not found.\n");
            return(EXIT_FAILURE);
        }
        if (receivedMessage.find("HTTP/1.1 304 Not Modified") == 0) {
            fprintf(stderr, "Acces not allowed.\n");
            return(EXIT_FAILURE);
        }   
    }
    close(clientSocket);

    return 0;
}
