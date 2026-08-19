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
 using namespace std ;
 bool sendInfo=false;
 //declaration
 void recvrobotinfo(bool &sendInfo);
 void sendcommand(bool &sendInfo);
int main(){
    std::cout << "Control Station starting..." << std::endl;
//window
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);//window


cout << "=================================\n";
cout << "     ROBOT CONTROL STATION\n";
cout << "=================================\n";

//thread
thread sen(sendcommand,ref(sendInfo));
thread rcv(recvrobotinfo,ref(sendInfo) );
sen.join();
rcv.join();

    return 0;
}
void recvrobotinfo(bool &sendInfo){
// socket
 SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in udp{};

    udp.sin_family = AF_INET;
    udp.sin_port = htons(5001);// socket bind
    inet_pton(AF_INET, "172.20.10.4", &udp.sin_addr);
// socket bind
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
    inet_pton(AF_INET, "172.20.10.x4", &server.sin_addr);// socket bind

    if (bind(serverSocket,(sockaddr*)&server ,sizeof(server)) != SOCKET_ERROR)

    std::cout << "Bind successful!" << std::endl;
    // socket bind


    if (listen(serverSocket, 2) != SOCKET_ERROR)

    std::cout << "Server is listening..." << std::endl;
SOCKET robotSocket[100];
//robot1
SOCKET robotSocket1 = accept(serverSocket, nullptr, nullptr);
if(robotSocket1!= SOCKET_ERROR)cout << "Robot 1 connected!" << endl;
robotSocket[0]=robotSocket1;
//robot2
SOCKET robotSocket2 = accept(serverSocket, nullptr, nullptr);
if(robotSocket2!= SOCKET_ERROR)cout << "Robot 2 connected!" << endl;
robotSocket[1]=robotSocket2;



while(true){
int id ; id=0;
int choice; choice=0;sendInfo=false;
cout << "=============MENU=============\n";
cout << "1-enter command \n";
cout << "2-exit\n";
cin>>choice;

if(choice==1){
//sending commands
  cout<<"enter the robot id ";cin>>id;
  this_thread::sleep_for(chrono::seconds(1));
  string message2;
  cout<<"enter the command ";cin.ignore();getline(cin,message2);
  send(robotSocket[id-1],message2.c_str(),message2.size(),0);
  this_thread::sleep_for(chrono::seconds(1));
  cout<<"\n";
  cout<<"command to Robot "<<id<<" sent  : "<<message2;
  cout<<"\n";sendInfo=true;
  this_thread::sleep_for(chrono::seconds(2));

}

else if(choice==2){
//quitting
  const char*message3="done\n";
  send(robotSocket1,message3,strlen(message3),0);
  send(robotSocket2,message3,strlen(message3),0);
  cout<<"\n";
  cout<<"command sent  : "<<message3;
  this_thread::sleep_for(chrono::seconds(1));
  cout<<"\n communication finished\n "; break;}
}

  }
