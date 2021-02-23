#ifndef BOOK
#define BOOK
#include <iostream>
#include<fstream>
#include<vector>
#include "poeple.hpp"
/*
  struct node {
  node* nxt;
  int size;
  char d[(sqn << 1) + 5];
  node() { size = 0, nxt = NULL, memset(d, 0, sizeof(d)); }//全部用0代替
  void pb(char c) { d[size++] = c; }
};
 */
//extern const int n;
using namespace std;
struct book{
    char ISBN[80];//这里可能要初始化
    char name[80];
    char author[80];
    char keyword[80];
    double price=0.0;
    int quantity=0;
    book(){
        memset(ISBN,'\0',sizeof(ISBN));
        memset(name,'\0',sizeof(name));
        memset(author,'\0',sizeof(author));
        memset(keyword,'\0',sizeof(keyword));
    }

   /* book(book other){
        strcpy(ISBN,other.ISBN);
        strcpy(name,other.name);
        strcpy(author,other.author);
        strcpy(keyword,other.keyword);
        price=other.price;
        quantity=other.quantity;
    }*/

  /*  void operator=(book other){
        strcpy(ISBN,other.ISBN);
        strcpy(name,other.name);
        strcpy(author,other.author);
        strcpy(keyword,other.keyword);
        price=other.price;
        quantity=other.quantity;
        //return *this;
    }*/
};
class node{
public:
    int offset = -1;
    char key[70];
    node(){
        memset(key,'\0',sizeof(key));
    }
    node(int offset_, const string& key_){
        memset(key,'\0',sizeof(key));
        offset=offset_;strcpy(key,key_.c_str());
    }
    node(const node &a){
        memset(key,'\0',sizeof(key));
      offset=a.offset;
      strcpy(key,a.key);
    }
    node &operator = (const node &a){
        if(this==&a)return *this;
        offset=a.offset;
        strcpy(key,a.key);
        return *this;
    }
};
class block{
public:
    int next_offset = -1;
    int back_offset = -1;//这是什么？
    int length = 0;
    node nodes[400];
    //block();
};
//length为1，char【0】=..
class block_list{
public:
    char filename[70]={'\0'};
    explicit block_list(const string& a){
        memset(filename,'\0',sizeof(filename));
        strcpy(filename,a.c_str());
    }
    int get_next_block(int offset);
    void addnode(node &a);
    //void addnode_0(node &a);
    void deletenode(node &a);
    //void deletenode_0(node &a);
    void findnode(const string& key,vector<int>&offsets);
    //void findnode_0(const string& key,vector<int>&offsets);
    void merge(int cur1,int cur2);
    void split(int cur);
    //void split_0(int cur);
};
#endif
