#include <iostream>
#include <thread>
#include <chrono>
#include <winsock2.h>//socket
#include <ws2tcpip.h>
#include <windows.h>
#include <cstring>
#include <functional>
#include <string>
#pragma comment(lib, "ws2_32.lib")//window
 using namespace std ;bool sendInfo=false;;
void recvrobotinfo(bool &sendInfo){

 SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in udp{};// socket bind

    udp.sin_family = AF_INET;// socket bind
    udp.sin_port = htons(5001);// socket bind
    inet_pton(AF_INET, "192.168.100.34", &udp.sin_addr);

if (bind(udpSocket,(sockaddr*)&udp ,sizeof(udp)) != SOCKET_ERROR)

    std::cout << "UDP Bind successful!" << std::endl;



while (true){
if(sendInfo){
char message[500];

sockaddr_in station{};
int stationSize = sizeof(station);

int bytesReceived = recvfrom(udpSocket,message,sizeof(message),0,(sockaddr*)&station,&stationSize);
cout<<"Rbt info: ";
if (bytesReceived > 0){
    for (int i = 0; i < bytesReceived; i++)
    {
     std::cout << message[i];
    }
}
cout<<"\n";
sendInfo=false;}
}


}
void sendcommand(bool &sendInfo){
   SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//socket


    if (serverSocket != INVALID_SOCKET)

    std::cout << "TCP socket created successfully" << std::endl;

    sockaddr_in server{};// socket bind

    server.sin_family = AF_INET;// socket bind
    server.sin_port = htons(5000);// socket bind
    inet_pton(AF_INET, "192.168.100.34", &server.sin_addr);// socket bind

    if (bind(serverSocket,(sockaddr*)&server ,sizeof(server)) != SOCKET_ERROR)

    std::cout << "Bind successful!" << std::endl;
    // socket bind


    if (listen(serverSocket, 2) != SOCKET_ERROR)

    std::cout << "Server is listening..." << std::endl;

/*SOCKET robotSocket2 = accept(serverSocket, nullptr, nullptr);
cout << "Robot 2 connected!" << endl;*/

SOCKET robotSocket1 = accept(serverSocket, nullptr, nullptr);
cout << "Robot 1 connected!" << endl;

while(true){int id ; id=0;
int choice; choice=0;sendInfo=false;
cout << "=============MENU=============\n";
cout << "1-enter command \n";
cout << "2-exit\n";
cin>>choice;

if(choice==1){

  cout<<"enter the robot id ";cin>>id;
  this_thread::sleep_for(chrono::seconds(1));
  string message2;
  cout<<"enter the command ";cin.ignore();getline(cin,message2);
  send(robotSocket1,message2.c_str(),message2.size(),0);sendInfo=true;
  cout<<"\n";
  cout<<"command to Robot "<<id<<" sent  : "<<message2;
  cout<<"\n";
  this_thread::sleep_for(chrono::seconds(2));

}

else if(choice==2){

  const char*message3="done\n";
  send(robotSocket1,message3,strlen(message3),0);
  cout<<"\n";
  cout<<"command sent  : "<<message3;
  this_thread::sleep_for(chrono::seconds(1));
  cout<<"\n communication finished\n "; break;}
}

/*
const char*message1="demarrer\n";
  send(robotSocket2,message1,strlen(message1),0);
  cout<<"\n";
  cout<<"command to Robot 2 sent  : "<<message1;
  cout<<"\n";
 this_thread::sleep_for(chrono::seconds(1));
const char*message4="arreter\n";
  send(robotSocket1,message4,strlen(message4),0);
  cout<<"\n";
  cout<<"command to Robot 1 sent  : "<<message4;
  cout<<"\n";
  this_thread::sleep_for(chrono::seconds(1));
const char*message5="changer la vitesse\n";
  send(robotSocket2,message5,strlen(message5),0);
  cout<<"\n";
  cout<<"command to Robot 2 sent  : "<<message5;
  cout<<"\n";
  this_thread::sleep_for(chrono::seconds(1));
const char*message3="done\n";
  send(robotSocket1,message3,strlen(message3),0);
  send(robotSocket2,message3,strlen(message3),0);
  cout<<"\n";
  cout<<"command sent  : "<<message3;
  this_thread::sleep_for(chrono::seconds(1));
  cout<<"\n communication finished\n ";
*/
  }
int main(){
    std::cout << "Control Station starting..." << std::endl;

    WSADATA wsaData;//window
    WSAStartup(MAKEWORD(2, 2), &wsaData);//window


cout << "=================================\n";
cout << "     ROBOT CONTROL STATION\n";
cout << "=================================\n";


thread sen(sendcommand,ref(sendInfo));
thread rcv(recvrobotinfo,ref(sendInfo) );
sen.join();
rcv.join();

    return 0;
}
