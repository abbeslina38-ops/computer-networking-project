#ifndef ROBOT_H
#define ROBOT_H
#include <string>
using namespace std;

class Robot{
public:
string  state;
float x,y,velocity;
int id;
Robot();
void sett(int i,float xx,float yy ,float v,string s);
~Robot();
};
#endif // ROBOT_H
