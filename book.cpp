//#include <iostream>
#include <stack>
#include"poeple.hpp"
//#include<queue>
#include<vector>
#include<fstream>
#include "program.hpp"
//#include "book.hpp"
#include "string"
using namespace std;
vector<people>people_system;//用户系统//可以直接存people//等下可以修改
//由于技术问题，使用log系统的时候要比对vector,因为删除、修改密码等操作
//不对劲，stack里面存的是指针，所以当它所指被delete掉之后，它就是野指针了
//这个问题怎么解决？
//干脆直接不delete了？
//这不好吧
//stack<people*> log_system;
//解决了，非法的
stack<string>log_system;//登录系统
//stack<string>aaa;
stack<double>finance_stack;//存收入、收出//存入文件时不做特殊处理，因为读入的时候还会再颠倒一次
double finance_total_zhen=0;
double finance_total_fu=0;
//const int sqn=200;
int main() {//先改vector,再改strcpy(char*,string...)的锅
    //const char* b=" ";
    //people user_test("root","root","k");
   // ofstream ff;
    program pro;
    //cout<<pro.get_token_num("su user0 user0_password ");
    //cout<<pro.get_token("su user0 user0_password ",3)<<"nima";
   pro.run();
   //fstream ff;
   //ff.open("people_system.txt",ios::binary|ios::in|ios::out);
   //people mid;
   //ff.read(reinterpret_cast<char*>(&mid),sizeof(mid));
   //cout<<mid.;
}
