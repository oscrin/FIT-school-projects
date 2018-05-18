/* Faculty of Information Technology, Brno University of Technology
*  Subject: IPK (Computer Communications and Networks) - Project 1
*  Description: Client/Server socket communication
*  Date created: February 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*  File: ftrestd.cpp - server side
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
#include <dirent.h>

#define IP_ADDRESS "127.0.0.1"
#define BUFF_LEN 1024

using namespace std;

/* "\n\n" - returns index of second '\n' */
int findMessageDelimiter(char* buffer) {
    for (int i = 0; i < BUFF_LEN; i++) {
        if (buffer[i] == '\n' && buffer[i-1] == '\n')
            return i;
    }
    return -1;
}

string createHead(char* buffer, int d) {
    stringstream headStream;
    for (int i = 0; i < d; i++) {
        headStream << buffer[i];
    }
    return headStream.str();
}

const char * createBinaryContent( char* buffer, int d) {
    const int size = BUFF_LEN - d;
    char binaryBuffer[size];
    int i = -1;
    for (d; d < BUFF_LEN; d++) {
        binaryBuffer[i++] = buffer[d];
    }
    return binaryBuffer;
}

string makeTime() {
    time_t rawtime;
    struct tm * timeinfo;
    char buffTime[100];

    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffTime, sizeof(buffTime),"%a, %d %b %Y %X CET", timeinfo);
    string str(buffTime);
    string t = "Date: ";
    return t.append(str);
}

int main(int argc, char * argv[]) {
    long long byteCountSend, byteCountRead;
    int welcomeSocket, serverSocket;
    char buffer[BUFF_LEN];
    int length;
    struct sockaddr_in serverAddr;

    string rootPath = get_current_dir_name();
    int portNum = 6677;
    string messageContent;

    string command;
    string type;
    string user;
    string head;
    stringstream headOut;
    string binaryContent;
    stringstream BinaryContentOut;
    string tim;
    unsigned long contentLen = 0;

    /* -------------- ARGUMENT RECOGNITION --------------- */
    int c;
    while ((c = getopt(argc, argv, "r:p:")) != -1 ) {
        switch (c) {
            case 'r':
                {   rootPath = optarg;
                    if ((access(rootPath.c_str(), W_OK)) != 0) {
                        perror("ERROR: Root Path");
                        exit(EXIT_FAILURE);   
                    } break;
                }
            case 'p':
                portNum = atoi(optarg);
                if (portNum < 1025 || portNum > 65535) {
                    perror("ERROR: Port Number");
                    exit(EXIT_FAILURE);              
                } break;     
        }
    }
    
    string savedRootPath = rootPath;

    /* -------------- SOCKET DEFINITION --------------- */
    /*---- Create the socket. The three arguments are: ----*/
    /* 1) Internet domain 2) Stream socket 3) Default protocol (TCP in this case) */
    welcomeSocket = socket(PF_INET, SOCK_STREAM, 0);
  
    /* -------------- ADDRESS STRUCT DEFINITION --------------- */
    /*---- Configure settings of the server address struct ----*/
    /* Set all bits of the padding field to 0 */
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);
    /* Address family = Internet */
    serverAddr.sin_family = AF_INET;
    /* Set port number, using htons function to use proper byte order */
    serverAddr.sin_port = htons(portNum);
    /* Set IP address to localhost */
    serverAddr.sin_addr.s_addr = inet_addr(IP_ADDRESS);

    struct sockaddr_in clientAddr = serverAddr;

    /* -------------- STRUCT - ADDRESS BINDING --------------- */
    /*---- Bind the address struct to the socket ----*/
    int bindCode;
    if ((bindCode = bind(welcomeSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))) != 0) {
        perror("ERROR: bind");
        exit(EXIT_FAILURE);
    }
    /*---- Listen on the socket, with 5 max connection requests queued ----*/
    if (listen(welcomeSocket, 5) < 0) {
        perror("ERROR: listen");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 0, BUFF_LEN);

    /*---- Accept call creates a new socket for the incoming connection ----*/
    socklen_t clientAddr_len = sizeof(clientAddr);
    while (1) {
        serverSocket = accept(welcomeSocket, (struct sockaddr *) &clientAddr, &clientAddr_len);
        if (serverSocket >= 0) {
            /*---- Send message to the socket of the incoming connection ----*/
            byteCountRead = recv(serverSocket, buffer, BUFF_LEN, 0);
            if (byteCountRead <= 0)
                break;
            else {


                int d = findMessageDelimiter(buffer);

                head = createHead(buffer, d);
                binaryContent = createBinaryContent(buffer, d);
                head.erase(head.length()-1,1);

                string parseHead = head;
                command = parseHead.substr(0,3);     // PUT, GET, ...
                int n = parseHead.find("?type=file HTTP/1.1");
                if (n == -1) {
                    n = parseHead.find("?type=folder HTTP/1.1");
                    type = "folder";
                    parseHead.erase(n,21);
                } else {
                    type = "file";
                    parseHead.erase(n,19);
                }
                n = parseHead.find('/'); // begin of path
                parseHead.erase(0,n+1);  // delete all before
                n = parseHead.find('\n');  // to the end of line
                string localPath = parseHead.substr(0,n); // localPath
                n = localPath.find('/');    // in local path separate user
                user = localPath.substr(0,n);
                localPath = localPath.erase(0,n);
                if (command == "PUT") {
                    n = parseHead.find("Content-Length: ");  // if PUT command
                    parseHead.erase(0, n+16);
                    char * pEnd;
                    contentLen = strtol(parseHead.c_str(), &pEnd, 10);
                }

                /* --------- LOCAL PATH SPEC ---------*/
                if (rootPath.at(rootPath.length()-1) != '/') {
                    rootPath.append("/");
                }
                
                rootPath.append(user);

                if ((access(rootPath.c_str(), W_OK) != 0)) {
                    headOut << "HTTP/1.1 401 Unauthorized" << endl;
                    tim = makeTime();
                    headOut << tim << endl;
                }

                rootPath.append(localPath);
                /* ---------------------------------- */
                
                cout << "Delimiter: " << d << endl;
                cout << "Command: " << command << endl;
                cout << "Type: " << type << endl;
                cout << "User " << user << endl;
                cout << "Path: " << localPath << endl;
                cout << "Length: " << contentLen << endl;

                cout << "rootPath: " << "|" << rootPath << "|" << endl;
                cout << "portNum: " << portNum << endl;

                cout << "Head:\n" << head << endl;
                cout << "BinaryContent:\n" << "|" << binaryContent << "|" << endl;

                
                // HTTP/1.1 200 OK
                // Date: Wed, 22 Feb 2017 15:50:01 CET
                /* -------------- rmd --------------- */
                if (command == "DEL" && type == "folder") {

                    struct stat s;
                    if ( stat(rootPath.c_str(),&s) == 0 )
                    {
                        if ( s.st_mode & S_IFDIR ) {
                            // je to slozka
                            if ((access(rootPath.c_str(), W_OK) == 0) && (rmdir(rootPath.c_str()) == 0)) {
                                // odesli uspech
                                headOut << "HTTP/1.1 200 OK" << endl;
                                tim = makeTime();
                                headOut << tim << endl;
                            }
                            else {
                            headOut << "HTTP/1.1 304 Not Modified" << endl;
                            tim = makeTime();
                            headOut << tim << endl;
                            }
                        }
                        else if ( s.st_mode & S_IFREG ) {
                            // je to soubor
                            headOut << "HTTP/1.1 400 Bad Request" << endl;
                            tim = makeTime();
                            headOut << tim << endl;
                        }
                    } 
                    else { 
                        // neplatna cesta
                        headOut << "HTTP/1.1 404 Not Found" << endl;
                        tim = makeTime();
                        headOut << tim << endl;
                    } 
                }
                /* -------------- del --------------- */
                if (command == "DEL" && type == "file") {

                    struct stat s;
                    if ( stat(rootPath.c_str(),&s) == 0 )
                    {
                        if ( s.st_mode & S_IFDIR ) {
                            // je to slozka
                            headOut << "HTTP/1.1 400 Bad Request" << endl;
                            tim = makeTime();
                            headOut << tim << endl;
                        }
                        else if ( s.st_mode & S_IFREG ) {
                            // je to soubor
                            if ((access(rootPath.c_str(), W_OK) == 0) && (remove(rootPath.c_str()) == 0)) {
                                // odesli uspech
                                headOut << "HTTP/1.1 200 OK" << endl;
                                tim = makeTime();
                                headOut << tim << endl;
                            }
                            else {
                            headOut << "HTTP/1.1 304 Not Modified" << endl;
                            tim = makeTime();
                            headOut << tim << endl;
                            }
                        }
                    } 
                    else { 
                        // neplatna cesta
                        headOut << "HTTP/1.1 404 Not Found" << endl;
                        tim = makeTime();
                        headOut << tim << endl;
                    }
                }
                /* -------------- lst --------------- */
                if (command == "GET" && type == "folder") {

                    struct stat s;
                    if ( stat(rootPath.c_str(),&s) == 0 )
                    {
                        if ( s.st_mode & S_IFDIR ) {
                            // je to slozka
                            DIR *dp;
                            struct dirent *ep;
                            stringstream lst;

                            dp = opendir ((rootPath.append("/")).c_str());
                            string f;
                            if (dp != NULL) {
                                while (ep = readdir (dp)) {
                                    f = ep->d_name;
                                    if (f.at(0) != '.')
                                        lst << f << "  ";
                                }
                                closedir(dp);
                                string ls = lst.str();
                                ls = ls.erase(ls.length()-2,ls.length());

                                headOut << "HTTP/1.1 200 OK" << endl;
                                tim = makeTime();
                                headOut << tim << endl;
                                headOut << "Content-Type: application/json" << endl;
                                headOut << "Content-Length: " << ls.length() << endl;
                                headOut << "Content-Encoding: identity" << endl;
                                headOut << endl;
                                headOut << ls;                     
                            }
                        }
                        else if ( s.st_mode & S_IFREG ) {
                            // je to soubor
                            headOut << "HTTP/1.1 400 Bad Request" << endl;
                            tim = makeTime();
                            headOut << tim << endl;
                        }
                    } 
                    else { 
                        // neplatna cesta
                        headOut << "HTTP/1.1 404 Not Found" << endl;
                        tim = makeTime();
                        headOut << tim << endl;
                    }          
                }
                /* -------------- get --------------- */
          /*      if (command == "GET" && type == "file") {

                }
                /* -------------- mkd --------------- */
                if (command == "PUT" && type == "folder") {
                    if (mkdir(rootPath.c_str(), 0755) == 0 ) {
                        // odesli uspech
                        headOut << "HTTP/1.1 200 OK" << endl;
                        tim = makeTime();
                        headOut << tim << endl;
                    } else {
                        // posli zpravu o neuspechu
                        headOut << "HTTP/1.1 304 Not Modified" << endl;
                        tim = makeTime();
                        headOut << tim << endl;
                    }
                }
                /* -------------- put --------------- */
          /*      if (command == "PUT" && type == "file") {

                } */

                rootPath = savedRootPath;
            }
      
            string respond = headOut.str();
            memset(buffer, 0, BUFF_LEN);
            strcpy(buffer, respond.c_str());
            byteCountSend = send(serverSocket, buffer, BUFF_LEN, 0);

            headOut.str("");
        }
    }

    return 0;
}