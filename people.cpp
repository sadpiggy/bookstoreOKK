#include"poeple.hpp"
people& people:: get_people(stack<string>&stack_,vector<people>&people_system_)
{
    auto itr=people_system_.begin();
    char id_[80];
    strcpy(id_,stack_.top().c_str());
    while(itr!=people_system_.end())
    {
        if(strcmp(id_,itr.operator*().id)==0){return itr.operator*();}//不能拿指针来比较
        else itr++;
    }

}

bool people::get_people_2(stack<string> &stack_, vector<people> &people_system_) {
    auto itr=people_system_.begin();
    char id_[80];
    strcpy(id_,stack_.top().c_str());
    while(itr!=people_system_.end())
    {
        if(strcmp(id_,itr.operator*().id)==0){return true;}//不能拿指针来比较
        else itr++;
    }
    return false;//就是没找到，就是已经删了
}

void people::login(stack<string>&stack_,vector<people>&people_system_,const string& id_,const string& passwd_){
   // cout<<"log_id  "<<id_<<endl;
    bool flag=false;
    auto itr=people_system_.begin();
    while(itr!=people_system_.end())
    {
        if(strcmp(id_.c_str(),itr.operator*().id)==0){flag=true;break;}//不能拿指针来比较//左小于右.返回负
        else itr++;
    }
    if(flag==false){cout<<"Invalid";return ;}
    else {
        //cout<<"jjj";
        if(strcmp(passwd_.c_str(),itr.operator*().passwd)!=0){cout<<"Invalid"<<endl;return;}
        else {
            string id_2;
            id_2=itr.operator*().id;
            stack_.push(id_2);
            //cout<<stack_.top()<<"  stacK"<<endl;//
            //cout<<"id  "<<id_2<<endl;//
        }
    }
    //stack_.push(this);
}

void people::login(stack<string>&stack_,vector<people>&people_system_,const string& id_)
{
    bool flag=false;
    auto itr=people_system_.begin();
    while(itr!=people_system_.end())
    {
        if(strcmp(id_.c_str(),itr.operator*().id)==0){flag=true;break;}//不能拿指针来比较
        else itr++;
    }
    if(flag==false){cout<<"Invalid"<<endl;return ;}
    else {
        if(privilege<=itr.operator*().privilege){cout<<"Invalid"<<endl;return;}
        else {
            string id_2;
            id_2=itr.operator*().id;
            stack_.push(id_2);
        }
    }
}

void people::logout(stack<string>&stack_){
    //cout<<id;
    if(stack_.empty()){cout<<"Invalid"<<endl;return;}
    else stack_.pop();
}

bool people::set_people(vector<people>&people_system_)//新添加的用这个函数
{
    //cout<<id<<"  id"<<endl;
    bool flag=true;
    auto itr=people_system_.begin();
    while(itr!=people_system_.end())
    {
        //  if(id==itr.operator*()->id){flag=false;break;}
        if(strcmp(id,itr.operator*().id)==0){flag=false;break;}//不能拿指针来比较
        else itr++;
    }
    if(flag){people_system_.push_back(*this);}
    else cout<<"Invalid"<<endl;

    return flag;
}

void people::set_privilege(int n)
{
    privilege=n;
}

void people:: user_add( const string& id_, const string& passwd_,int privilege_,const string& name_,vector<people>&people_system_){
    bool  flag;//cout<<"niam";
    if(privilege<=privilege_){cout<<"Invalid"<<endl;return;}
    if(privilege_==3){
        people other(id_,name_,passwd_);
        other.set_privilege(3);
        flag=other.set_people(people_system_);
    }
   if(privilege_==1)
   {
      // people *other = new people(id_, name_, passwd_);
       people other(id_,name_,passwd_);
       other.set_privilege(1);
       flag = other.set_people(people_system_);
   }
   //
    //people* other(id_,name_,passwd_);
}

void people::user_register(const string& id_,const string& passwd_,const string& name_,vector<people>&people_system_)
{
    //cout<<"register"<<endl;///
     //people* other=new people(id_,name_,passwd_);
    people other(id_,name_,passwd_);
     other.set_privilege(1);
     bool flag=other.set_people(people_system_);
     //if(flag==false){cout<<"Invalid"<<endl;/*delete other;*/return;}//输过了
}

/*void people::user_delete(const string& id_,vector<people>&people_system_,stack<string>stack_){//这里改动，可能出错。。。。。//Yg出错了
    if(privilege!=7){cout<<"Invalid"<<endl;return;}
    else {
        bool flag=false;
        auto itr=people_system_.begin();
        while(itr!=people_system_.end())
        {
           // if(id_==itr.operator*()->id){flag=true;break;}
            if(strcmp(id_.c_str(),itr.operator*().id)==0){flag=true;break;}//不能拿指针来比较
            else itr++;
        }
        if(flag==false){cout<<"Invalid"<<endl;return;}
        if(itr->privilege==7){cout<<"Invalid"<<endl;return;}
        people_system_.erase(itr);
        /*else{//听说，delete以及登录的用户，非法
            //stack<>//
            while (stack_.empty()==false)
            {
                if(strcmp(id_.c_str(),stack_.top().c_str())==0)
                {
                   cout<<"Invalid"<<endl;return;
                }
                else stack_.pop();
            }
         people_system_.erase(itr);
            //delete itr.operator*();
        }
    }
}*/

void people::user_delete(const string& id_,vector<people>&people_system_,stack<string>stack_){//这里改动，可能出错。。。。。//Yg出错了
    if(privilege!=7){cout<<"Invalid"<<endl;return;}
        bool flag=false;
        auto itr=people_system_.begin();
        while(itr!=people_system_.end())
        {
            // if(id_==itr.operator*()->id){flag=true;break;}
            if(strcmp(id_.c_str(),itr.operator*().id)==0){flag=true;break;}//不能拿指针来比较
            else itr++;
        }
        if(flag==false){cout<<"Invalid"<<endl;return;}

        //if(itr->privilege==7){cout<<"Invalid"<<endl;return;}

        //people_system_.erase(itr);
        //听说，delete以及登录的用户，非法
            //stack<>//
            while (stack_.empty()==false)
            {
                if(strcmp(id_.c_str(),stack_.top().c_str())==0)
                {
                   cout<<"Invalid"<<endl;return;
                }
                else stack_.pop();
            }
         people_system_.erase(itr);
            //delete itr.operator*();

}


void people::user_passwd( const string& id_, const string& old_passwd, const string& new_passwd,vector<people>&people_system_) {
    if(privilege==0){cout<<"Invalid"<<endl;return;}
    else{
        bool flag=false;
        auto itr=people_system_.begin();
        while(itr!=people_system_.end())
        {
            //if(id_==itr.operator*()->id){flag=true;break;}
            if(strcmp(id_.c_str(),itr.operator*().id)==0){flag=true;break;}//不能拿指针来比较
            else itr++;
        }
        if(flag==false){cout<<"Invalid"<<endl;return;}
        else{
            if(strcmp(itr.operator*().passwd,old_passwd.c_str())==0)//相同
            {
                strcpy(itr.operator*().passwd,new_passwd.c_str());
            }else{//不同
                cout<<"Invalid"<<endl;return;
            }
        }
    }
}

void people::user_passwd( const string& id_, const string& new_passwd,vector<people>&people_system_){
    if(privilege!=7){cout<<"Invalid"<<endl;return;}
    else{
        bool flag=false;
        auto itr=people_system_.begin();
        while(itr!=people_system_.end())
        {
           // if(id_==itr.operator*()->id){flag=true;break;}
            if(strcmp(id_.c_str(),itr.operator*().id)==0){flag=true;break;}//不能拿指针来比较
            else itr++;
        }
        if(flag==false){cout<<"Invalid"<<endl;return;}
        else{
            strcpy(itr.operator*().passwd,new_passwd.c_str());
        }
    }
}

void people::set_select(int select,vector<people>&people_system_){/////
    select_offset=select;
    auto itr=people_system_.begin();
    while(itr!=people_system_.end())
    {
        //  if(id==itr.operator*()->id){flag=false;break;}
        if(strcmp(id,itr.operator*().id)==0){break;}//不能拿指针来比较
        else itr++;
    }
    itr.operator*().select_offset=select;
}
