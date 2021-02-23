#ifndef PEOPLE
#define PEOPLE
#include<iostream>
#include<stack>
#include<string>
#include<vector>
#include "cstring"
using namespace std;//其实这里不应该搞继承的，直接万物平等更方便
class people{
private:
   char id[40];
   char name[40];
   char passwd[40];
public:
    int privilege=0;//迫不得已
    int select_offset=-1;
    people(const string& id_,const string& name_,const string& passwd_ )//这里可能有bug//为什么不能用char*？
   {
        memset(id,'\0',sizeof(id));
        memset(name,'\0',sizeof(name));
        memset(passwd,'\0',sizeof(passwd));
       strcpy(id,id_.c_str());
       strcpy(name,name_.c_str());
       strcpy(passwd,passwd_.c_str());
       privilege=0;
   }
   people(){
       memset(id,'\0',sizeof(id));
       memset(name,'\0',sizeof(name));
       memset(passwd,'\0',sizeof(passwd));
    }
   ~people(){};//这个要写吗？我忘了，艹
   void set_privilege(int n);

    people& get_people(stack<string>&stack_,vector<people>&people_system_);

    bool
    get_people_2(stack<string>&stack_,vector<people>&people_system_);

   void login(stack<string>&stack_,vector<people>&people_system_,const  string& id_,const string& passwd_);

    void login(stack<string>&stack_,vector<people>&people_system_,const string& id_);

   void logout(stack<string>&stack_);

    bool set_people(vector<people>&people_system_);//新添加的用这个函数

    //useradd [user-id] [passwd] [7/3/1] [name] #3:增加一个指定权限的用户，只能创建小于自己权限的账户
    //add 和 register 需要new,因为vector和stack里面存储的是people*指针，不是people
    void user_add( const string& id_, const string& passwd_,int privilege_, const string& name_,vector<people>&people_system_);

   //register [user-id] [passwd] [name] #0:注册一个带有这些信息的权限1用户
   void user_register(const string& id_,const string& passwd_,const string& name_,vector<people>&people_system_);
    //stack也要进行相应改变，对吗？
    //很抱歉，我不会
    //只能在使用stack的时候再处理了
   void user_delete( const string& id_,vector<people>&people_system_,stack<string>satck_);
   //修改密码
   //passwd [user-id] [old-passwd(if not root)] [new-passwd] #1:root不需要填写旧密码，其余账户需要
   void user_passwd( const string& id_,const string& old_passwd,const string& new_passwd,vector<people>&people_system_);
   //root用
   void user_passwd(const string& id_, const string& new_passwd,vector<people>&people_system_);

   void set_select(int select,vector<people>&people_system_);
};
/*
class root: public people
{
public:
    root( char* id_, char* name_, char* passwd_): people(  id_,  name_,  passwd_ ){privilege=7;};

    root(): people( "root","root","sjtu" ){privilege=7;};
};
class worker: public  people//为什么不是public基类指针就不能指向它了？
{
public:
    worker( char* id_, char* name_, char* passwd_): people(  id_,  name_,  passwd_ ){privilege=3;};

};
class customer: public people
{
public:
    customer( char* id_, char* name_, char* passwd_): people(  id_,  name_,  passwd_ ){privilege=1;};

};
*/
#endif
