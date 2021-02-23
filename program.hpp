#ifndef PROGRAM
#define PROGRAM
//文件   books.txt   ISBN.txt  NAME.txt   AUTHOR.txt KEYWORD.txt people_system.txt
//文件   finance.txt
//get_token一族用到都要delete
#include"poeple.hpp"
#include"book.hpp"
#include <map>
#include "string"
#define ISBNFILE ("ISBN.txt")
//do.txt
//block_list book_block("books.txt");//这个应该无用

//block_list people_system_block("people_system.txt");//这个应该也无用
class program{//记得优先级的判断呀
public:

    void init();

    void run();

    void select(const string& ISBN_,people& user);

    void modify_ISBN(const string& ISBN_,people&user);

    void modify_name(const string &NAME_,people&user);

    void modify_author(const string &author_,people&user);

    void modify_price(double price_,people&user);

    void modify_keyword(const string &keyword_,people&user);//这里应该与其他的不太一样//大体一样，只是keywords要划分成小块，然后一块一块处理

    void modify_keywords(const string &keywords_,people&user);//这个是大块

    void users(const string& aa,people& user);

    void modify(const string& aa,people& user);

    void show(const string &aa,people& user);

    void show_all_books(people& user);

    void show_ISBN(const string &ISBN_,people&user);

    void show_name(const string &NAME_,people&user);

    void show_author(const string &author_,people&user);

    void show_keyword(const string &keyword_,people&user);

    void show_finance( people& user,int n=0);

    void import(const string &aa,people& user);

    void buy(const string &ISBN_,int quantity,people& user);
    //工具
    void read_in();//people.txt,用之前read_in,之后write_out,加了个新功能，read_in  finance.txt

    void write_out();

    string get_token(const string &aa,int n);//用完之后记得delete

    int get_token_num(const string &aa);

    int get_keyword_num(const string &aa);

    string get_keyword(const string &aa,int n);

    string get_token_2(const string &aa);//把双引号之间的内容取出来

    string get_token_3(const string &aa);//把等号后的内容取出来//注：get_token函数的复用性应该更强的，没写好

    void read_in_book(int offset_,book&book_mid);//把一本书读入程序

    void read_out_book(int offset_,book book_mid);//吧一本书读入文件

    void sum_finance(int n=0);//计算最近n次的finance

    bool sum_time(int n);

    void quit();

    int char_to_int(const string &aa);

    void show_do();
};


/*class  show_do
{
public:
    string name[100];
    string a[100];
    int length=0;
};*/
#endif
