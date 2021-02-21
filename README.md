          图书管理系统——wky的文档
此程序主要由三个类组成：people block_list  program

people负责用户有关的操作
block_list块链
Program  负责整个程序的运行

people的组成：
private:char id[40];char name[40];char passwd[40];public:int privilege=0;    int select_offset=-1;
主要函数接口:void set_privilege(int n);//设置privilege

    people& get_people(stack<string>&stack_,vector<people>&people_system_);//用来得到登录系统的用户

    bool   get_people_2(stack<string>&stack_,vector<people>&people_system_);//用来判断登录系统中的用户是否已经被删去

   void login(stack<string>&stack_,vector<people>&people_system_,const  string& id_,const string& passwd_);

    void login(stack<string>&stack_,vector<people>&people_system_,const string& id_);

   void logout(stack<string>&stack_);

    bool set_people(vector<people>&people_system_);//将新用户录入系统

    void user_add( const string& id_, const string& passwd_,int privilege_, const string& name_,vector<people>&people_system_);//添加新用户

  
   void user_register(const string& id_,const string& passwd_,const string& name_,vector<people>&people_system_);//注册用户
    
   void user_delete( const string& id_,vector<people>&people_system_,stack<string>satck_);//删除用户
  
   //passwd [user-id] [old-passwd(if not root)] [new-passwd] #1:root不需要填写旧密码，其余账户需要
   void user_passwd( const string& id_,const string& old_passwd,const string& new_passwd,vector<people>&people_system_);
   
   void user_passwd(const string& id_, const string& new_passwd,vector<people>&people_system_);

   void set_select(int select,vector<people>&people_system_);//设置select


 block_list由block组成，block由node组成
 
 
 node的组成： int offset = -1（book在文件的位置）;  char key[70];
 
 block的组成：int next_offset = -1;//下一块block在文件中的位置
    int back_offset = -1;//上一块block在文件中的位置
    int length = 0;//block中node的数量
    node nodes[400];//一堆node
 
block_list的主要函数接口
    void addnode(node &a);//将一个node加入块链
   
    void deletenode(node &a);//删除一个node
    
    void findnode(const string& key,vector<int>&offsets);//找到一个key所对应的offset们
   
    void merge(int cur1,int cur2);//将块合并
    
    void split(int cur);//将块裂开
   
   
   program中没有变量，只有函数
   主要函数接口:
   
   void init();//进行初始化

    void run();//进行整个程序，起着最重要的作用

    void select(const string& ISBN_,people& user);//选择select
   
    void modify(const string& aa,people& user);//进行有关modify的操作
    
    void show(const string &aa,people& user);//进行有关show的操作
     
    void users(const string& aa,people& user);//进行有关用户系统的操作
    
    void import(const string &aa,people& user);//进书

    void buy(const string &ISBN_,int quantity,people& user);//买书
    
    还有一些辅助函数：
    void read_in();//将文件里的用户信息读入

    void write_out();//将用户信息读入文件

    string get_token(const string &aa,int n);//以空格划分string

    int get_token_num(const string &aa);//得到此划分的string数

    int get_keyword_num(const string &aa);//以|划分string

    string get_keyword(const string &aa,int n);得到此划分的string数

    string get_token_2(const string &aa);//把双引号之间的内容取出来

    string get_token_3(const string &aa);//把等号后的内容取出来//注：get_token函数的复用性应该更强的，没写好

    void read_in_book(int offset_,book&book_mid);//把一本书读入程序

    void read_out_book(int offset_,book book_mid);//吧一本书读入文件

    void sum_finance(int n=0);//计算最近n次的finance

    bool sum_time(int n);//计算time是否超出了finance的次数

    void quit();//离开程序

    int char_to_int(const string &aa);将char型的转换成int
