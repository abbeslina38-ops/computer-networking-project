#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <chrono>
#include <thread>
#include <functional>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

//CREATING A CLASS ROBOT
class Robot{
public:
string  state;
float x,y,velocity;
int id;

//CONSTRUCTOR FOR CLASS ROBOT
Robot(){}

//SETTER FOR CLASS ROBOT
void sett(int i,float xx,float yy ,float v,string s){
id=i;
velocity=v;
state=s;
x=xx;
y=yy;
}
};

Robot R1;
bool sendinfo=false;
mutex robotmutex; //TO PROTECT ROBOT INFORMATION

//CREATING A FUNCTION FOR SENDING ROBOT INFORMATION
void sendrobotinfo1(bool &sendinfo){ string robotinfo;

SOCKET robotSocket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in station{};

    station.sin_family = AF_INET;
    station.sin_port = htons(5001);
    station.sin_addr.s_addr = INADDR_ANY;

while(true){
if(sendinfo){
this_thread::sleep_for(chrono::seconds(1));

lock_guard<mutex> lock(robotmutex);
robotinfo =
    "id=" + to_string(R1.id) +
    ";x=" + to_string(R1.x) +
    ";y=" + to_string(R1.y) +
    ";velocity=" + to_string(R1.velocity) +
    ";state=" + R1.state;

    sendto(robotSocket1,robotinfo.c_str(),robotinfo.size(),0,(sockaddr*)&station,sizeof(station));
    sendinfo=false;
}}}

//CREATING A FUNCTION FOR .....
void recvcommand1(bool &sendinfo){
 SOCKET clientSocket1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket1 != INVALID_SOCKET)
    cout << "TCP socket created successfully" << endl;

    sockaddr_in client{};

    client.sin_family = AF_INET;
    client.sin_port = htons(5000);
    client.sin_addr.s_addr = INADDR_ANY;

    if (connect(clientSocket1,(sockaddr*)&client ,sizeof(client)) != SOCKET_ERROR)

    cout << "connected to the control station " << endl;


//receiving
queue<string> commands;
while (true){
char message1[100];
int bytrecvs=recv(clientSocket1,message1,sizeof(message1),0);
  string mg="";
   for(int i=0;i<bytrecvs;i++){
    if (message1[i]=='\n') break;
     mg+=message1[i];}

     commands.push(mg);

if(mg=="done"){break;}
lock_guard<mutex> lock(robotmutex);
if(commands.front()=="demarrer"){R1.state="EN-MARCHE";}
if(commands.front()=="arreter"){R1.state="ARRET";}
if(commands.front()=="changer la vitesse"){R1.velocity=30;}
sendinfo=true;
commands.pop();
}

}
int main(){int id ;float x,y,velocity;string state;
//window definition
WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
cout<<"enter the robot id ";cin>>id;
cout<<"enter the robot position x ";cin>>x;
cout<<"enter the robot position y ";cin>>y;
cout<<"enter the robot velocity ";cin>>velocity;
cout<<"enter the robot state ";cin.ignore();getline(cin,state);
R1.sett(id,x,y,velocity,state);
thread recv1(recvcommand1,ref(sendinfo));
thread sen1(sendrobotinfo1,ref(sendinfo));
recv1.join();
sen1.join();



    return 0;
}
