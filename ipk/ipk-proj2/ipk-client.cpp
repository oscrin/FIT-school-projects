/* Faculty of Information Technology, Brno University of Technology
*  Subject: IPK (Computer Communications and Networks) - Project 2
*  Description: Math client using IPv4 or IPv6
*  Date created: April 2017
*  Author: Jan Kubica
*  Login: xkubic39
*  Email: xkubic39@stud.fit.vutbr.cz
*/
#include <openssl/md5.h>
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <arpa/inet.h>
#include <sys/types.h> // connect()
#include <sys/stat.h> // stat()
#include <sys/socket.h> // connect(), send(), bind(), accept()
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <algorithm> // count()
#include <cctype>
#include <iomanip>   // setprecision()

#define LOGIN "xkubic39"
#define PORT "55555"
#define BUFF_LEN 1024

using namespace std;

struct Math {
   string  buffer;
   int   i1;
   int   i2;
   char  op;
};

string ip_address;
char md5_login[33];
char buffer[BUFF_LEN];
int c_socket;
struct addrinfo hints, *res;
long long byteCountSend, byteCountRead = 0;
string r_buffer;
string w_buffer;


char * login_to_hash() {
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)&LOGIN, strlen(LOGIN), (unsigned char*)&digest);    
    for (int i = 0; i < 16; i++)
         sprintf(&md5_login[i*2], "%02x", (unsigned int)digest[i]);
    return md5_login;
}
/* poslani result error */
void result_error () {
    w_buffer = "RESULT ERROR\n";
    byteCountSend = send(c_socket, w_buffer.c_str(), strlen(w_buffer.c_str()), 0);
    if (byteCountSend < 0) {
        cerr << "ERROR: SEND in RESULT ERROR" << endl;
        exit(1);
    }
}
/* vypocitej operand a operatory */
struct Math math_magic(const char buffer[]) {
    string i1c, i2c = "";
    char op = '\0';
    struct Math m;

    int j;
    for (j = 0;; j++) {
        if (op == '\0') {
            if ((i1c == "") & (buffer[j] == '-'))
                i1c = buffer[j];
            if (isdigit(buffer[j]))
                i1c += buffer[j];
            else {
                j++;
                op = buffer[j];
                j++;
            }   
        } else {
            if ((i2c == "") & (buffer[j] == '-'))
                i2c = buffer[j];
            if (isdigit(buffer[j]))
                i2c += buffer[j];
            else
                break;
        }
    }
    /* test na 00006 */
    if(((i1c.substr(0,1) == "0") && (i1c.length() > 1)) || ((i2c.substr(0,1) == "0") && (i2c.length() > 1)))
        result_error();

    m.i1 = atoi(i1c.c_str());
    m.i2 = atoi(i2c.c_str());
    m.op = op;
    m.buffer = string(buffer).erase(0,j);

    return m;
}
/* over platnost prikazu */
bool verify_command(const char* buffer) {
    bool solve = false;
    bool number1 = false;
    bool space1 = false;
    bool op = false;
    bool space2 = false;
    bool number2 = false;
    bool end = false;
    int i = 6;

    if (
        buffer[0] == 'S' and
        buffer[1] == 'O' and
        buffer[2] == 'L' and
        buffer[3] == 'V' and
        buffer[4] == 'E' and
        buffer[5] == ' ')
        solve = true;
    
    if (buffer[i] == '-')
        i++;
    do {
        if (isdigit(buffer[i]))
            number1 = true;
        else
            break;
        i++;
    } while (isdigit(buffer[i]));

    if (buffer[i] == ' ') {
        space1 = true;
        i++;
    }

    if (buffer[i] == '+' or buffer[i] == '-' or buffer[i] == '*' or buffer[i] == '/') 
        op = true;
    i++;
    if (buffer[i] == ' ') {
        space2 = true;
        i++;
    }

    if (buffer[i] == '-')
        i++;
    do {
        if (isdigit(buffer[i]))
            number2 = true;
        else
            break;
        i++;
    } while (isdigit(buffer[i]));

    if (buffer[i] == '\n')
        end = true;
    
    return (solve and number1 and space1 and op and space2 and number2 and end);
}
/* vypocet operace SOLVE */
string compile_solve(string r_buffer) {
    if (verify_command(r_buffer.c_str())) { // zjisti platnost prikazu
        r_buffer.erase(0,6); // odsekni 'SOLVE '
        struct Math m = math_magic(r_buffer.c_str()); // zjisti operandy
        long double result;
        int verify = true;
        r_buffer = m.buffer;

        if (m.i2 == 0 and m.op == '/') { // deleni nulou
            result_error();

        } else {
            switch(m.op) { // hledani operatoru
                case '+' : { result = m.i1 + m.i2; break; }
                case '-' : { result = m.i1 - m.i2; break; }
                case '*' : { result = m.i1 * m.i2; break; }
                case '/' : { result = m.i1 / float(m.i2); break; }
                default : verify = false;
            }

            long double pre_result = result * 100;
            int int_res = pre_result;
            result = int_res / 100.00;
            stringstream round_result;
            round_result << fixed << setprecision(2) << result;
//                cout << "cisla: " << m.i1 << " " << m.i2 << endl; // if aftersolve != \n
//                cout << "After solve: " << r_buffer.c_str() << endl; // if aftersolve != \n
//                cout << "Result: " << round_result.str() << endl;
            if (verify) {
                memset(buffer, 0, BUFF_LEN);
                w_buffer = "RESULT " + round_result.str() + "\n";
//                    cout << "w_buffer: " << w_buffer << endl;
                byteCountSend = send(c_socket, w_buffer.c_str(), strlen(w_buffer.c_str()), 0);
                if (byteCountSend < 0) {
                    cerr << "ERROR: SEND RESULT" << endl;
                    exit(1);
                }                        
//             cout << "Bytes send: " << byteCountSend << endl;              
            } // konec RESULT ok
        } // RESULT ERROR nebo ne
    } // validni SOLVE nebo ne
    int d;
    if ((r_buffer.substr(0,5)).compare("\nBYE ")) {
        w_buffer.erase(0,1);
        return r_buffer;
    }
    if ((d = r_buffer.find("\n") != string::npos))
        r_buffer.erase(0,d+1);
    else
        r_buffer = "";

    return r_buffer;
}


int main(int argc, char * argv[]) {
    
    if (argc > 2) {
        cerr << "TOO MANY ARGUMENTS" << endl;
        exit(1);
    } 
    ip_address = argv[1];
    char * md5_login = login_to_hash();
    string hello = "HELLO " + string(md5_login) + "\n";
//    cout << hello << endl;

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;
    getaddrinfo(argv[1], PORT, &hints, &res);

    if ((c_socket = socket(res->ai_family, res->ai_socktype, 0)) <= 0) {
        cerr << "ERROR: SOCKET" << endl;
        exit(1);
    }
    if (connect(c_socket, res->ai_addr, res->ai_addrlen) < 0) {
        cerr << "ERROR: CONNECT" << endl;
        exit(1);
    }

    memset(buffer, 0, BUFF_LEN);
    strcpy(buffer, hello.c_str());

    byteCountSend = send(c_socket, buffer, strlen(hello.c_str()), 0);
    if (byteCountSend < 0) {
        cerr << "ERROR: SEND" << endl;
        exit(1);
    }
//    cout << "Bytes send: " << byteCountSend << endl;

    while (1) {
        memset(buffer, 0, BUFF_LEN);
        r_buffer = "";
        w_buffer = "";

        byteCountRead = recv(c_socket, buffer, BUFF_LEN, 0);
//        cout << "Bytes recieved: " << byteCountRead << endl;
            r_buffer = buffer;
//            cout << "r_buffer: " << r_buffer << endl;

            // SOLVE
            while (1) {              
            if (((r_buffer.substr (0,5)).compare("SOLVE")) == 0)
                r_buffer = compile_solve(r_buffer);
//                cout << "Zbytek stringu: " << r_buffer << endl;
            else
                break;
            }

            // BYE
            if ((r_buffer.substr(0,4)).compare("BYE ") == 0)
                if (r_buffer.back() == '\n') {
                    r_buffer.erase(0,4); // odsekni 'BYE'
                    r_buffer = r_buffer.substr(0, r_buffer.size()-1);
                    cout << r_buffer << endl;
                    break;
                }
        }     
    return 0;
}