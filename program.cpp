#include"program.hpp"
#include "sstream"
//命令
//su user-id passwd    su root sjtu
//logout
//useradd user-id passwd privilege name   #3
//register user-id passwd name
//delete user-id
//passwd user-id old-passwd new-passwd
//passwd user-id new-passwd
//select ISBN
//modify -ISBN="ISBN" -name="name" -author="author" -keyword="keyword" -price="price"
//文件   books.txt   ISBN.txt  NAME.txt   AUTHOR.txt KEYWORD.txt people_system.txt
//关闭程序时记得将vector写回去//写回去的不是vector,是用户信息
  block_list ISBN_block("ISBN.txt");
 block_list NAME_block("NAME.txt");
 block_list AUTHOR_block("AUTHOR.txt");
 block_list KEYWORD_block("KEYWORD.txt");
extern vector<people>people_system;
extern stack<string>log_system;
extern stack<double>finance_stack;//存收入、收出//存入文件时不做特殊处理，因为读入的时候还会再颠倒一次
extern double finance_total_zhen;
extern double finance_total_fu;//都是正的
void program::init() {
    ofstream ff;
    ff.open("finance.txt",ios::binary);
    ff.close();
    ff.open("books.txt",ios::binary);
    ff.close();
    ff.open("ISBN.txt",ios::binary);
    ff.close();
    ff.open("NAME.txt",ios::binary);
    ff.close();
    ff.open("AUTHOR.txt",ios::binary);
    ff.close();
    ff.open("KEYWORD.txt",ios::binary);
    ff.close();
    ff.open("people_system.txt",ios::binary);
    ff.close();
    ff.open("people_system.txt",ios::binary|ios::in|ios::out);
    //people* people_mid=new people("root","root","sjtu");
    people  people_mid("root","root","sjtu");
    people_mid.set_privilege(7);
    people_mid.set_people(people_system);
    //people_mid->login(log_system,people_system,"root","sjtu");//这里可能出问题
    //ff.write(reinterpret_cast<char*>(&people_system),sizeof(people_system));//读入//读入指针没什么用，读入用户
    ff.write(reinterpret_cast<char*>(&people_mid),sizeof(people));//
    ff.close();//用完就关闭，养成好习惯
    finance_total_zhen=0;finance_total_fu=0;
}

void program::read_in()
{
    people_system.clear();
    //cout<<"nima";
    fstream ff;
    ff.open("people_system.txt",ios::binary|ios::in|ios::out);
    ff.seekg(0,ios::end);//沃日，（iOS：：end）==2,(0,ios::end)==128,NB
    int offset_=ff.tellg();
    ff.seekg(0,ios::beg);
      while(ff.tellg()!=offset_)//??文件的末尾为什么是2
    {
        people user_mid;
        ff.read(reinterpret_cast<char*>(&user_mid),sizeof(user_mid));
        user_mid.select_offset=-1;//已改。。。。。
        people_system.push_back(user_mid);
    }
    ff.close();
    ff.open("finance.txt",ios::binary|ios::in|ios::out);
    ff.seekg(0,ios::end);
    offset_=ff.tellg();
    ff.seekg(0,ios::beg);
    stack<double>finance_stack_mid;
    while(ff.tellg()!=offset_)
    {
        double mid;
        ff.read(reinterpret_cast<char*>(&mid),sizeof(mid));
        finance_stack_mid.push(mid);
    }
    ff.close();
    while(!finance_stack_mid.empty()){
        double mid=finance_stack_mid.top();finance_stack_mid.pop();
        finance_stack.push(mid);
    }
}

void program::write_out()
{
    fstream ff;
    ff.open("people_system.txt",ios::binary|ios::trunc|ios::out);
    vector<people>::iterator itr;//itr_mid;
    itr=people_system.begin();
    //ff.seekp(0,ios::end);
    while(itr!=people_system.end())
    {
        ff.write(reinterpret_cast<char*>(&*itr),sizeof(people));
        itr++;
    }
    ff.close();
}

void program::run() {//这个最后改
    string a;
    ifstream ff;
    ff.open("people_system.txt",ios::binary|ios::in);
    if(!ff){//尚未打开过
        init();
        //cout<<"INIT";
    } else   ff.close();
    fstream fff;
    people* user;
    people user_mid;
    read_in();
    while(getline(cin,a))
    {
        //if(log_system.empty()==false){user=&(user->get_people(log_system,people_system));}//已改...
         user=&user_mid;
        if(log_system.empty()==false){
            bool flag=user->get_people_2(log_system,people_system);
            if(flag==true)user=&(user->get_people(log_system,people_system));}
        string token=get_token(a,1);//记得delete
        if((token[0]=='s'&&token[1]=='u')||token[0]=='l'||token[0]=='u'||token[0]=='r'||token[0]=='d'||token[0]=='p')
        {users(a,*user);
        write_out();
        continue;}//bug
        if(strcmp(token.c_str(),"select")==0){
            string token_2=get_token(a,2);
            select(token_2,*user);
            write_out();continue;}
        if(strcmp(token.c_str(),"modify")==0){
            modify(a,*user);
            write_out();continue;}
        if(strcmp(token.c_str(),"import")==0){import(a,*user);write_out();continue;}
        if(strcmp(token.c_str(),"show")==0){show(a,*user);write_out();continue;}
        if(strcmp(token.c_str(),"buy")==0){
            string ISBN_=get_token(a,2);
            string token_3=get_token(a,3);
            int quantity_;//=int(token_3[0]);
            stringstream f(token_3);
            f>>quantity_;
            buy(ISBN_,quantity_,*user);
            write_out();continue;
        }
        if(strcmp(token.c_str(),"exit")==0||strcmp(token.c_str(),"quit")==0||strcmp(token.c_str(),"EOF")==0){quit();}
        //delete token;
        write_out();cout<<"Invalid"<<endl;
    }
}
//注意登录区为空的情况
void program::users(const string& aa,people& user) {//哲理绝对会内存泄漏，之后记得处理//改为string 之后就不会了  //所以就不用且不能delete了
   string token_1=get_token(aa,1);
    if(strcmp(token_1.c_str(),"su")==0)
    {
       if(get_token_num(aa)==2)
       {
           string token_2=get_token(aa,2);
           user.login(log_system,people_system,token_2);
       }
        if(get_token_num(aa)==3)
        {
            //cout<<3;
            string token_2=get_token(aa,2);
            string token_3=get_token(aa,3);
           // cout<<token_2<<"   "<<token_3<<endl;///日日日
            user.login(log_system,people_system,token_2,token_3);
        }
    }
    if(strcmp(token_1.c_str(),"logout")==0)
    {
        user.logout(log_system);
    }
    if(strcmp(token_1.c_str(),"useradd")==0)
    {
        //cout<<5;
        if(get_token_num(aa)!=5){cout<<"Invalid"<<endl;/*delete token_1*/;return;}
        int mid=get_token(aa,4)[0]-'0';//cout<<get_token(aa,4)[0];
        user.user_add(get_token(aa,2),get_token(aa,3),mid,get_token(aa,5),people_system);
    }
    if(strcmp(get_token(aa,1).c_str(),"register")==0)
    {
        if(get_token_num(aa)<4){cout<<"Invalid"<<endl;/*delete token_1*/;return;}//可能出问题
        user.user_register(get_token(aa,2),get_token(aa,3),get_token(aa,4),people_system);
    }
    if(strcmp(get_token(aa,1).c_str(),"delete")==0)
    {
        user.user_delete(get_token(aa,2),people_system,log_system);
        //delete token_2;
    }
    if(strcmp(get_token(aa,1).c_str(),"passwd")==0)
    {
        if(get_token_num(aa)==3){
            user.user_passwd(get_token(aa,2),get_token(aa,3),people_system);
            //delete token_2;delete token_3;
        }
        if(get_token_num(aa)==4){
            user.user_passwd(get_token(aa,2),get_token(aa,3),get_token(aa,4),people_system);
           // delete token_2;delete token_3;delete token_4;
        }
    }
    //delete token_1;
}

void program::modify(const string &aa, people &user)
{
    if(user.privilege<3){cout<<"Invalid"<<endl;return;}
    if(user.select_offset==-1){cout<<"Invalid"<<endl;return;}
   // cout<<"lalala";
    int n=get_token_num(aa);
    for(int i=2;i<=n;i++)
    {
        string token=get_token(aa,i);//记得delete
        if(token[1]=='n')
        {
            string token_=get_token_2(token);
            modify_name(token_,user);
        }
        if(token[1]=='a')
        {
            string token_=get_token_2(token);
            modify_author(token_,user);
        }
        if(token[1]=='k')
        {
            string token_=get_token_2(token);
            modify_keywords(token_,user);
        }
        if(token[1]=='p'){
            string token_=get_token_3(token);
            stringstream f(token_);
            double d;
            f>>d;
            modify_price(d,user);
        }
        if(token[1]=='I'){
            string token_=get_token_3(token);
            modify_ISBN(token_,user);
        }
        //delete token;
    }
}//aa[0]==m

void program::select(const string &ISBN_,people& user)
{
    if(user.privilege<3){cout<<"Invalid"<<endl;return;}
    vector<int>offsets;
    ISBN_block.findnode(ISBN_,offsets);
    if(offsets.empty()){
       // cout<<"empty"<<endl;
        book book_mid;
        node node_mid;
        strcpy(node_mid.key,ISBN_.c_str());
        strcpy(book_mid.ISBN,ISBN_.c_str());
        fstream ff;
        ff.open("books.txt",ios::binary|ios::in|ios::out);
        ff.seekp(0,ios::end);
        int offset_=ff.tellp();
        //cout<<offset_<<endl;
        node_mid.offset=offset_;
        ff.write(reinterpret_cast<char*>(&book_mid),sizeof(book));
        ff.close();
        //cout<<book_mid.name<<"   kk   "<<endl;
        //cout<<book_mid.ISBN<<endl;
        ISBN_block.addnode(node_mid);
        user.select_offset=offset_;
        //cout<<66;
        return;//可能没写完
    }
    user.select_offset=offsets.front();
   // cout<<user.select_offset;
}

void program::modify_ISBN(const string &ISBN_, people &user)
{
    if(user.select_offset==-1){cout<<"Invalid"<<endl;return;}
    book book_mid;read_in_book(user.select_offset,book_mid);

    node node_tmp;vector<int>off_st;
    node_tmp.offset=user.select_offset;
    strcpy(node_tmp.key,ISBN_.c_str());
    ISBN_block.findnode(node_tmp.key,off_st);
    if(!off_st.empty()){cout<<"Invalid"<<endl;return;}

    if(book_mid.ISBN[0]=='\0')
    {
        node node_mid;
        node_mid.offset=user.select_offset;
        strcpy(node_mid.key,ISBN_.c_str());
        ISBN_block.addnode(node_mid);
    } else{//先删，后加
        node node_mid;
        node_mid.offset=user.select_offset;
        strcpy(node_mid.key,book_mid.ISBN);
        ISBN_block.deletenode(node_mid);
        strcpy(node_mid.key,ISBN_.c_str());
        ISBN_block.addnode(node_mid);
    }
    strcpy(book_mid.ISBN,ISBN_.c_str());
    read_out_book(user.select_offset,book_mid);
}

void program::modify_author(const string &author_, people &user)
{
    if(user.select_offset==-1){cout<<"Invalid"<<endl;return;}
    book book_mid;read_in_book(user.select_offset,book_mid);
    if(book_mid.author[0]=='\0')
    {
        node node_mid;
        node_mid.offset=user.select_offset;
        strcpy(node_mid.key,author_.c_str());
        AUTHOR_block.addnode(node_mid);//有可能有问题
    } else{//先删，后加
        node node_mid;
        node_mid.offset=user.select_offset;
        strcpy(node_mid.key,book_mid.author);
        AUTHOR_block.deletenode(node_mid);
        strcpy(node_mid.key,author_.c_str());
        AUTHOR_block.addnode(node_mid);
    }
    strcpy(book_mid.author,author_.c_str());
    read_out_book(user.select_offset,book_mid);
}

void program::modify_name(const string &NAME_, people &user)
{
    if(user.select_offset==-1){cout<<"Invalid"<<endl;return;}
    //cout<<"modi  name"<<endl;
    book book_mid;read_in_book(user.select_offset,book_mid);//cout<<book_mid.name;
    if(book_mid.name[0]=='\0')
    {
        node node_mid;
        node_mid.offset=user.select_offset;
        strcpy(node_mid.key,NAME_.c_str());
        NAME_block.addnode(node_mid);
    } else{//先删，后加
        node node_mid;
        node_mid.offset=user.select_offset;
        strcpy(node_mid.key,book_mid.name);
        NAME_block.deletenode(node_mid);
        strcpy(node_mid.key,NAME_.c_str());
        NAME_block.addnode(node_mid);
    }
    strcpy(book_mid.name,NAME_.c_str());
    read_out_book(user.select_offset,book_mid);
}

void program::modify_keyword(const string &keyword_, people &user) {
    node node_mid;
    node_mid.offset=user.select_offset;
    strcpy(node_mid.key,keyword_.c_str());
    KEYWORD_block.addnode(node_mid);
}

void program::modify_keywords(const string &keywords_, people &user) {//先删后加
    if(user.select_offset==-1){cout<<"Invalid"<<endl;return;}
    int n;

    n=get_keyword_num(keywords_);
    string key_words[15];
    for(int i=1;i<=n;i++){key_words[i]=get_keyword(keywords_,i);}
    for(int i=1;i<=n;i++)
        for(int j=i+1;j<=n;j++)
        {
            if(strcmp(key_words[i].c_str(),key_words[j].c_str())==0){cout<<"Invalid"<<endl;return;}
        }


    book book_mid;read_in_book(user.select_offset,book_mid);
     n=get_keyword_num(book_mid.keyword);
    for(int i=1;i<=n;i++)
    {
        string keyword_=get_keyword(book_mid.keyword,i);
        node node_mid;
        //node_mid.key=keyword_;
        strcpy(node_mid.key,keyword_.c_str());
        node_mid.offset=user.select_offset;
        KEYWORD_block.deletenode(node_mid);
        //delete keyword_;//应该是需要delete的
    }
    strcpy(book_mid.keyword,keywords_.c_str());
    read_out_book(user.select_offset,book_mid);
    n=get_keyword_num(keywords_);

    for(int i=1;i<=n;i++)
    {
        string keyword_=get_keyword(keywords_,i);
        modify_keyword(keyword_,user);//加入node
        //delete keyword_;//可能出错，如果没出错，吧=把之前的一个函数中需要delete的delete掉
    }
}

void program::modify_price(double price_, people &user)
{
    if(user.select_offset==-1){cout<<"Invalid"<<endl;return;}
    book book_mid;read_in_book(user.select_offset,book_mid);
    book_mid.price=price_;
    read_out_book(user.select_offset,book_mid);
}

void program::import(const string &aa, people &user)
{
    if(user.privilege<3){cout<<"Invalid"<<endl;return;}
    if(user.select_offset==-1){cout<<"Invalid"<<endl;return;}
    string quantity_=get_token(aa,2);
    string price_=get_token(aa,3);//记得delete
    if(quantity_[0]-'0'<0||quantity_[0]-'0'>9){cout<<"Invalid"<<endl;return;}
    if(price_[0]-'0'<0||price_[0]-'0'>9){cout<<"Invalid"<<endl;return;}
    stringstream f1(quantity_),f2(price_);
    int quantity;double price;
    f1>>quantity;f2>>price;
    book book_mid;read_in_book(user.select_offset,book_mid);
    book_mid.quantity+=quantity;
    read_out_book(user.select_offset,book_mid);
    finance_stack.push(-price);
    sum_finance();//不仅仅是为了计算，也为了动态修改文件，但是怕影响时间复杂度
}

void program::show_ISBN(const string& ISBN_, people &user) {
    //if(get_token_num())
    vector<int>offsets_;
    ISBN_block.findnode(ISBN_,offsets_);
    if(offsets_.empty()){cout<<endl;return;}
    book book_mid;
    read_in_book(offsets_.front(),book_mid);
    cout<<book_mid.ISBN<<'\t'<<book_mid.name<<'\t'<<book_mid.author<<'\t'<<book_mid.keyword<<'\t';
    printf("%0.2f",book_mid.price);
    cout<<'\t'<<book_mid.quantity<<endl;
}

void program::show_author(const string&author_, people &user) {
    vector<int>offsets_;
    AUTHOR_block.findnode(author_,offsets_);
    vector<int>::iterator itr_vec=offsets_.begin();
    if(offsets_.empty()){cout<<endl;return;}
    map<string ,int>map_mid;//ISBN,offsets
    while (itr_vec!=offsets_.end())
    {
        int mid=*itr_vec;
        book book_mid;read_in_book(mid,book_mid);
        string ISBN_(book_mid.ISBN);
        //ISBN_.copy(book_mid.ISBN);
        //strcpy(ISBN_,book_mid.ISBN);
        map_mid.insert(make_pair(ISBN_,mid));
       // map_mid.insert(ISBN_,mid);
        //offsets_.pop_back();//是不是删第一个？
        itr_vec++;
    }
    map<string ,int>::iterator itr=map_mid.begin();
    while(itr!=map_mid.end())
    {
        book book_mid;
        read_in_book(itr->second,book_mid);
        cout<<book_mid.ISBN<<'\t'<<book_mid.name<<'\t'<<book_mid.author<<'\t'<<book_mid.keyword<<'\t';
        printf("%0.2f",book_mid.price);
        cout<<'\t'<<book_mid.quantity<<endl;
        itr++;
    }
}

void program::show_keyword(const string &keyword_, people &user) {
    vector<int>offsets_;
    KEYWORD_block.findnode(keyword_,offsets_);
    vector<int>::iterator itr_vec=offsets_.begin();
    if(offsets_.empty()){cout<<endl;return;}
    map<string ,int>map_mid;//ISBN,offsets
    while (itr_vec!=offsets_.end())
    {
        int mid=*itr_vec;
        book book_mid;read_in_book(mid,book_mid);
        string ISBN_(book_mid.ISBN);
        //ISBN_.copy(book_mid.ISBN);
        //strcpy(ISBN_,book_mid.ISBN);
        map_mid.insert(make_pair(ISBN_,mid));
        // map_mid.insert(ISBN_,mid);
        //offsets_.pop_back();//是不是删第一个？
        itr_vec++;
    }
    map<string ,int>::iterator itr=map_mid.begin();
    while(itr!=map_mid.end())
    {
        book book_mid;
        book_mid;read_in_book(itr->second,book_mid);
        cout<<book_mid.ISBN<<'\t'<<book_mid.name<<'\t'<<book_mid.author<<'\t'<<book_mid.keyword<<'\t';
        printf("%0.2f",book_mid.price);
        cout<<'\t'<<book_mid.quantity<<endl;
        itr++;
    }
}

void program::show_name(const string &NAME_, people &user) {
    vector<int>offsets_;
    NAME_block.findnode(NAME_,offsets_);
    vector<int>::iterator itr_vec=offsets_.begin();
    if(offsets_.empty()){cout<<endl;return;}
    map<string ,int>map_mid;//ISBN,offsets
    while (itr_vec!=offsets_.end())
    {
        int mid=*itr_vec;
        book book_mid;read_in_book(mid,book_mid);
        string ISBN_(book_mid.ISBN);
        map_mid.insert(make_pair(ISBN_,mid));
        itr_vec++;
    }
    map<string ,int>::iterator itr=map_mid.begin();
    while(itr!=map_mid.end())
    {
        book book_mid;
        read_in_book(itr->second,book_mid);
        cout<<book_mid.ISBN<<'\t'<<book_mid.name<<'\t'<<book_mid.author<<'\t'<<book_mid.keyword<<'\t';
        printf("%0.2f",book_mid.price);
        cout<<'\t'<<book_mid.quantity<<endl;
        itr++;
    }
}

void program::show_all_books(people &user) {
    map<string ,int>map_mid;//ISBN,offsets
    fstream ff;
    ff.open("ISBN.txt",ios::binary|ios::out|ios::in);
    ff.seekp(0,ios::end);
    int end_offset=ff.tellp();
    ff.seekg(0,ios::beg);
    if(ff.tellg()==end_offset){cout<<endl;return;}//无书空
    while (ff.tellg()!=end_offset)//可以·吗·？//不可以
    {
        block block_mid;
        ff.read(reinterpret_cast<char*>(&block_mid),sizeof(block));
      //  ff.seekg(sizeof(block),ios::cur);
        for(int i=1;i<=block_mid.length;i++)
        {
            string ISBN_(block_mid.nodes[i-1].key);
            int offset_=block_mid.nodes[i-1].offset;
            map_mid.insert(make_pair(ISBN_,offset_));
        }
    }
    map<string ,int>::iterator itr=map_mid.begin();
    while(itr!=map_mid.end())
    {
        book book_mid;
        read_in_book(itr->second,book_mid);
        cout<<book_mid.ISBN<<"\t"<<book_mid.name<<"\t"<<book_mid.author<<"\t"<<book_mid.keyword<<"\t";
        printf("%0.2f",book_mid.price);
        cout<<"\t"<<book_mid.quantity<<endl;
        itr++;
    }
}

void program::show_finance(people &user,int n) {
    if(user.privilege!=7){cout<<"Invalid"<<endl;return;}
    //sum_finance();
    if(n==0)sum_finance();
    else  {
        if(sum_time(n)==false){cout<<"Invalid"<<endl;return;}
        sum_finance(n);
    }
    cout<<'+'<<' ';
    printf("%0.2f",finance_total_zhen);
    cout<<' '<<'-'<<' ';
    printf("%0.2f",finance_total_fu);
    cout<<endl;
}


//show
////Output: ISBN2 name1 author1 keyword1 10.00 0
void program::show(const string &aa, people &user) {//我有有个大大的问题，show keyword=...的时候，万一有好几本书，怎么办，用map维护
    //if(user.select_offset==-1){cout<<"Invalid  show user.select_offset==-1"<<endl;return;}
    if(user.privilege==0){cout<<"Invalid"<<endl;return;}
    if(get_token_num(aa)>3){cout<<"Invalid"<<endl;return;}
    if(get_token_num(aa)==1){show_all_books(user);return;}
    string token=get_token(aa,2);//记得delete
    if(token[0]=='f'){
        if(get_token_num(aa)==2)
        {show_finance(user);/*delete token;*/return;}
        if(get_token_num(aa)==3)
        {
            string token_3=get_token(aa,3);
            int n;//=int(token_3[0]);
            stringstream f(token_3);
            f>>n;
            show_finance(user,n);
            //delete token_3;delete token;
            return;
        }
    }
    if(token[1]=='I'){
        string token_=get_token_3(token);
        show_ISBN(token_,user);
        //delete token;delete token_;
        return;
    }
    if(token[1]=='n'){
        string token_=get_token_2(token);
        show_name(token_,user);
        //cout<<token_<<" name"<<endl;////
        //delete token;delete token_;
        return;
    }
    if(token[1]=='a'){
        string token_=get_token_2(token);
        show_author(token_,user);
        //cout<<token_<<" author"<<endl;////
        //delete token;delete token_;
        return;
    }
    if(token[1]=='k'){
        string token_=get_token_2(token);
        show_keyword(token_,user);
        //delete token;delete token_;
        return;
    }
}

void program::buy(const string &ISBN_, int quantity, people &user)
{
    if(user.privilege==0){cout<<"Invalid"<<endl;return;}
    vector<int>offsets;
    ISBN_block.findnode(ISBN_,offsets);
    if(offsets.empty()){cout<<"Invalid"<<endl;return;}
    book book_mid;read_in_book(offsets.front(),book_mid);
    double cost=quantity*book_mid.price;
    if(book_mid.quantity<quantity){cout<<"Invalid"<<endl;return;}
    finance_stack.push(cost);
    book_mid.quantity-=quantity;
    read_out_book(offsets.front(),book_mid);
    printf("%0.2f",cost);cout<<endl;
}

void program::sum_finance(int n)
{
    finance_total_zhen=0.00;finance_total_fu=0.00;
    stack<double>finance_mid=finance_stack;
    //for(int i)
    if(n==0)
    {
        while(!finance_mid.empty()){
            double mid=finance_mid.top();finance_mid.pop();
            if(mid>0)finance_total_zhen+=mid;
            if(mid<0)finance_total_fu+=(-mid);
        }
    } else{
        for(int i=1;i<=n;i++)
        {
            double mid=finance_mid.top();finance_mid.pop();
            if(mid>0)finance_total_zhen+=mid;
            if(mid<0)finance_total_fu+=(-mid);
        }
    }
    fstream ff;
    ff.open("finance.txt",ios::binary|ios::trunc|ios::out);
    ff.seekp(0,ios::beg);
    while (!finance_mid.empty())
    {
        double mid=finance_mid.top();finance_mid.pop();
        ff.write(reinterpret_cast<char*>(&mid),sizeof(mid));
    }
    ff.close();
}

bool program::sum_time(int n) {
    stack<double>finance_mid=finance_stack;
    for(int i=1;i<=n;i++)
    {
        if(finance_mid.empty())return false;
        finance_mid.pop();
    }
    return true;
}

string program::get_token(const string &aa, int n) {//出了点问题，最后一个词的最后的空格应该除去//本来就没读进来
    int k=1;int i=0;
    char b[80];
    for(int g=0;g<80;g++)b[g]='\0';
    while(k!=n)
    {
        if(aa[i]==' ')k++;
        i++;
    }
    if(n==1)
    {
        i=0;
        while(aa[i]!=' '&&aa[i]!='\0')
        {
            b[i]=aa[i];
            i++;
        }
        string c(b);
        return c;
    }
    int g=0;
    while(aa[i]!=' '&&aa[i]!='\0')
    {
        b[g]=aa[i];
        g++;
        i++;
    }
    string c(b);
    return c;
    //return b;
}

int program::get_token_num(const string &aa) {
    bool flag=false;
    int k=1;
    int i=0;
    while(aa[i]!='\0')
    {
        //if(aa[i]==' ')k++;
        if(aa[i]==' ')flag=true;
        if(flag==true&&(aa[i]!=' '&&aa[i]!='\0')){k++;flag=false;}
        i++;
    }
    return k;
}

int program::get_keyword_num(const string &aa) {
    int k=1;
    int i=0;
    while(aa[i]!='\0')
    {
        if(aa[i]=='|')k++;
        i++;
    }
    return k;
}

string program::get_keyword(const string &aa, int n) {
    int k=1;int i=0;
   // char* b=new char[70];
   char b[80];
    for(int g=0;g<80;g++)b[g]='\0';
    while(k!=n)
    {
        if(aa[i]=='|')k++;
        i++;
    }
    if(n==1)
    {
        i=0;
        while(aa[i]!='|'&&aa[i]!='\0')
        {
            b[i]=aa[i];
            i++;
        }
        string c(b);
        return c;
        //return b;
    }
    int g=0;
    while(aa[i]!='|'&&aa[i]!='\0')
    {
        b[g]=aa[i];
        g++;
        i++;
    }
    string c(b);
    return c;
   // return b;
}
//2_双引号
string program::get_token_2(const string &aa)
{
    bool flag=false;
    //char* token=new char[80];
    char token[80];
    for(int g=0;g<80;g++)token[g]='\0';
    int i=0,k=0;
    while(aa[i]!='\0')
    {
        if(aa[i]=='"'){flag=!flag;i++;continue;}
        if(flag==true)
        {
            token[k]=aa[i];
            k++;
        }
        i++;
    }
    string c(token);
    return c;
    //return token;
}

string program::get_token_3(const string &aa)
{
   // char* token=new char[80];
   char token[80];
    bool flag=false;
    for(int g=0;g<80;g++)token[g]='\0';
    int i=0,k=0;
    while(aa[i]!='\0')
    {
        if(aa[i]=='='){flag=true;i++;continue;}
        if(flag){
            token[k]=aa[i];
            k++;
        }
        i++;
    }
    string c(token);
    return c;
    //return token;
}

void program::quit() {
    sum_finance();
    write_out();
    exit(0);
}

void program::read_in_book(int offset_,book&book_mid)
{
    //book book_mid;
    fstream  ff;
    ff.open("books.txt",ios::binary|ios::in);
    ff.seekg(offset_,ios::beg);
    ff.read(reinterpret_cast<char*>(&book_mid),sizeof(book));
    //cout<<book_mid.ISBN;cout<<endl<<book_mid.name;
    ff.close();
}

void program::read_out_book(int offset_, book book_mid)
{
    fstream ff;
    ff.open("books.txt",ios::binary|ios::in|ios::out);
    ff.seekp(offset_,ios::beg);
    ff.write(reinterpret_cast<char*>(&book_mid),sizeof(book));
    ff.close();
}

int program::char_to_int(const string &aa)
{
    //int b;
    if(aa[0]=='1')return 1;
    if(aa[0]=='3')return 3;
    if(aa[0]=='7')return 7;
}
