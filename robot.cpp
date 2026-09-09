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
#include "Robot.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;
//define
Robot R1;Robot R2;bool sendinfo=false;bool sendinfo2=false;
mutex robotmutex;
//declaration
void sendrobotinfo1(bool &sendinfo);
void recvcommand1(bool &sendinfo);
int main(){
//define
int id,id2 ;float x,x2,y,y2,velocity2,velocity;string state,state2;
//window definition
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
//intialisation of robot1
cout<<"enter the robot id ";cin>>id;
cout<<"enter information of robot "<<id<<" :\n";
cout<<"enter the robot position x ";cin>>x;
cout<<"enter the robot position y ";cin>>y;
cout<<"enter the robot velocity ";cin>>velocity;
cout<<"enter the robot state ";cin.ignore();getline(cin,state);
R1.sett(id,x,y,velocity,state);
//thread
thread recv1(recvcommand1,ref(sendinfo));
thread sen1(sendrobotinfo1,ref(sendinfo));
recv1.join();
sen1.join();


    return 0;
}
//functions defintions
void sendrobotinfo1(bool &sendinfo){ string robotinfo;
//creating socket
SOCKET robotSocket1 = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in station{};

    station.sin_family = AF_INET;
    station.sin_port = htons(5001);
    inet_pton(AF_INET, "172.20.10.4", &station.sin_addr);

while(true){
if(sendinfo){
this_thread::sleep_for(chrono::seconds(1));
//sending robot info
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
void recvcommand1(bool &sendinfo){
//creating socket
 SOCKET clientSocket1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in client{};

    client.sin_family = AF_INET;
    client.sin_port = htons(5000);
    inet_pton(AF_INET, "172.20.10.4", &client.sin_addr);

    connect(clientSocket1,(sockaddr*)&client ,sizeof(client));


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
//modify
if(mg=="done"){break;}
lock_guard<mutex> lock(robotmutex);
if(commands.front()=="demarrer"){R1.state="EN-MARCHE";}
if(commands.front()=="arreter"){R1.state="ARRET";}
if(commands.front()=="changer la vitesse"){R1.velocity=30;}
sendinfo=true;
commands.pop();
}

}

commands.pop();
}

}*/
