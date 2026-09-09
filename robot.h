#include "Robot.h"

Robot::Robot()
{

}
void Robot::sett(int i,float xx,float yy ,float v,string s){
id=i;
velocity=v;
state=s;
x=xx;
y=yy;
}
Robot::~Robot()
{
    //dtor
}
