#include "book.hpp"
using namespace std;
int block_list::get_next_block(int offset_) {
    fstream ff;
    ff.open(filename,ios::binary|ios::out|ios::in);
    ff.seekg(offset_);
    block block_mid;
    ff.read(reinterpret_cast<char*>(&block_mid),sizeof(block_mid));
    ff.close();
    return block_mid.next_offset;
}
void block_list::addnode_0(node &a) {
    fstream ff1;
    ff1.open(filename,ios::binary|ios::out|ios::in);
    if(!ff1){cout<<"little brother your program have some programs"<<endl;return;}
    ff1.seekg(0,ios::end);
    int cur=ff1.tellg();
    if(cur==0)//此文件中没有block//是吧？。。。。。。
    {
        block block_mid;
        block_mid.nodes[0]=a;
        ff1.write(reinterpret_cast<char*>(&block_mid),sizeof(block));
        ff1.close();
        return;
    }
    //cur!=0
     cur=0;
    int next=get_next_block(0);
    while(next!=-1)//找到一个块，其开头小于a，下一块的开头大于a
    {
        block mid;
        ff1.seekg(next);
        ff1.read(reinterpret_cast<char*>(&mid),sizeof(block));
        if(strcmp(a.key,mid.nodes[0].key)<0)break;
        cur=next;
        next=get_next_block(cur);
    }
    ff1.seekg(cur);
    block block_mid;
    ff1.read(reinterpret_cast<char*>(&block_mid),sizeof(block));
   int position=-1;
    bool flag=false;
    for(int i=0;i<block_mid.length;i++)
    {
        if(strcmp(a.key,block_mid.nodes[i].key)<=0){flag=true;position=i;break;}
    }
    if(!flag)
    {
        block_mid.nodes[block_mid.length]=a;
        block_mid.length++;
        ff1.seekp(cur);
        ff1.write(reinterpret_cast<char*>(&block_mid),sizeof(block));
        ff1.close();
        if(block_mid.length>=300)split(cur);
        return;
    }
    for(int k=block_mid.length;k>=position+1;k--)block_mid.nodes[k]=block_mid.nodes[k-1];
    block_mid.nodes[position]=a;
    block_mid.length++;
    ff1.seekp(cur);
    ff1.write(reinterpret_cast<char*>(&block_mid),sizeof(block));
    ff1.close();
    if(block_mid.length>300)split(cur);
}
void block_list::addnode(node &a){//2.0
    fstream fin,fout;
    fin.open(filename,ios::in | ios::binary);
    fout.open(filename,ios::in | ios::out | ios::binary);
    if(!fin || !fout)cout<<"Invalid"<<"can open"<<endl;
    fin.seekg(0,ios::end);
    int x = fin.tellg();
    if(x == 0){
        block tmp;
        fout.seekp(0);
        fout.write(reinterpret_cast<char*>(&tmp),sizeof(block));
    }
    int cur = 0;
    int next = get_next_block(cur);
    while(next != -1){
        block b;
        fin.seekg(next);
        fin.read(reinterpret_cast<char*>(&b),sizeof(block));
        if(strcmp(a.key,b.nodes[0].key) < 0)break;
        cur = next;
        next = get_next_block(cur);
    }
    int p = -1;
    bool flag = false;
    fin.seekg(cur);
    block s;
    fin.read(reinterpret_cast<char*>(&s),sizeof(block));
    for(int i = 0 ; i < s.length ; i++)
    {
        if(strcmp(a.key,s.nodes[i].key) < 0){
            p = i;
            flag = true;
            break;
        }
    }
    if(!flag)p = s.length;
    for(int i = s.length; i >= p + 1 ; i--)//递减（防止覆盖）
    {
        s.nodes[i] = s.nodes[i - 1];
    }
    s.nodes[p] = a;
    s.length++;
    fout.seekp(cur);
    fout.write(reinterpret_cast<char*>(&s),sizeof(block));
    fin.close();
    fout.close();
    if(s.length >= 290){
        split(cur);
    }
}

void block_list::deletenode_0(node &a) {
    fstream ff1;
    ff1.open(filename,ios::binary|ios::in|ios::out);
    if(!ff1){cout<<"little brother your program have some programs  delete"<<endl;return;}
    ff1.seekg(0,ios::end);
    int cur=ff1.tellg();
    if(cur==0)//此文件中没有block//是吧？。。。。。。
    {
        //cout<<"you have deleted air"<<endl;
        return;//删空气  要返回错误吗？//......
    }
    //cur!=0
    cur=0;
    int next=get_next_block(0);
    while(next!=-1)//找到一个块，其开头小于a，下一块的开头大于a
    {
        block mid;
        ff1.seekg(next,ios::beg);
        ff1.read(reinterpret_cast<char*>(&mid),sizeof(block));
        if(strcmp(a.key,mid.nodes[0].key)<0)break;
        cur=next;
        next=get_next_block(cur);
    }
    ff1.seekg(cur,ios::beg);
    block block_mid;
    ff1.read(reinterpret_cast<char*>(&block_mid),sizeof(block));
    int position=-1;
    bool flag=false;
    for(int i=0;i<block_mid.length;i++)
    {
        if(strcmp(a.key,block_mid.nodes[i].key)==0&&a.offset==block_mid.nodes[i].offset){flag=true;position=i;break;}
    }
    if(!flag)
    {
        //删空气
        return;
    }
    block_mid.length--;
    for(int k=position;k<=block_mid.length-1;k++)block_mid.nodes[k]=block_mid.nodes[k+1];
    ff1.seekp(cur,ios::beg);
    ff1.write(reinterpret_cast<char*>(&block_mid),sizeof(block));
   // ff1.close();////...
    block block_mid2;
    ff1.seekg(block_mid.next_offset);//
    ff1.read(reinterpret_cast<char*>(&block_mid2),sizeof(block));
    ff1.close();
    if(block_mid.length+block_mid2.length<200){merge(cur,block_mid.next_offset);}
}

void block_list::deletenode(node &a){//2.0
    fstream fin,fout;
    fin.open(filename,ios::in | ios::binary);
    fout.open(filename,ios::in | ios::out | ios::binary);
    if(!fin || !fout)cout<<"Invalid"<<"can open"<<endl;
    fin.seekg(0,ios::end);
    int x = fin.tellg();
    if(x == 0){
        return;
    }
    int cur = 0;
    int next = get_next_block(cur);
    while(next != -1){
        block b;
        fin.seekg(next);
        fin.read(reinterpret_cast<char*>(&b),sizeof(block));
        if(strcmp(a.key,b.nodes[0].key) < 0)break;
        cur = next;
        next = get_next_block(cur);
    }
    bool flag = true;
    int p ;
    fout.seekg(cur);
    block s;
    fout.read(reinterpret_cast<char*>(&s),sizeof(block));
    for(int i = 0 ; i < s.length ; i++)
    {
        if(strcmp(a.key,s.nodes[i].key) == 0 && a.offset == s.nodes[i].offset){
            p = i;
            flag = false;
            break;
        }
    }
    if(flag)return;
    for(int i = p ; i <= s.length - 2; i++)
    {
        s.nodes[i] = s.nodes[i + 1];
    }
    s.length--;
    fout.seekp(cur);
    fout.write(reinterpret_cast<char*>(&s),sizeof(block));
    fin.seekg(cur);
    block c;
    fin.read(reinterpret_cast<char*>(&c),sizeof(block));
    next = c.next_offset;
    int nextlen = 0;
    if(next != -1) {
        block az;
        fin.seekg(next);
        fin.read(reinterpret_cast<char *>(&az), sizeof(block));
        nextlen = az.length;
    }
    fin.close();
    fout.close();
    if(s.length + nextlen <= -1 && next != -1)merge(cur,next);//不想要merge了
}

void block_list::split_0(int cur) {
    fstream ff;
    block block_mid,block1,block2;
    ff.open(filename,ios::binary|ios::in|ios::out);
    if(!ff){cout<<"wenjie da bu kai"<<endl;return;}
    ff.seekg(cur,ios::beg);
    ff.write(reinterpret_cast<char*>(&block_mid),sizeof(block));
    block1.length=200;block2.length=block_mid.length-200;
    for(int i=0;i<200;i++)block1.nodes[i]=block_mid.nodes[i];
    for(int i=0;i<block2.length;i++)block2.nodes[i]=block_mid.nodes[i+200];
    ff.seekp(0,ios::end);
    int offset2=ff.tellp();
    if(block_mid.next_offset>0){
        ff.seekp(block_mid.next_offset+sizeof(int));
        ff.write(reinterpret_cast<char*>(offset2),sizeof(int));
    }
    block1.next_offset=offset2;
    block1.back_offset=block_mid.back_offset;
    block2.back_offset=cur;
    block2.next_offset=block_mid.next_offset;
    ff.seekp(cur,ios::beg);
    ff.write(reinterpret_cast<char*>(&block1),sizeof(block));
    ff.seekp(offset2);
    ff.write(reinterpret_cast<char*>(&block2),sizeof(int));
    ff.close();
}

void block_list::split(int cur){
    fstream fin,fout;
    fin.open(filename,ios::in | ios::binary);
    fout.open(filename,ios::out | ios :: in | ios ::binary);
    if(!fin || !fout){cout<<"Invalid"<<endl;return;}
    block square1,square2,square_tmp;
    fout.seekg(cur);
    fout.read(reinterpret_cast<char *>(&square_tmp),sizeof(block));
    for(int i = 0 ; i <square_tmp.length ; i++)
    {
        if(i < 150 ){
            square1.nodes[i] = square_tmp.nodes[i];
        }
        else{
            square2.nodes[i - 150] = square_tmp.nodes[i];
        }
    }
    square1.length = 150;
    square2.length = square_tmp.length - 150;
    fin.seekg(0,ios::end);
    int offset2 = fin.tellg();
    if(square_tmp.next_offset >= 0) {
        fout.seekp(square_tmp.next_offset + sizeof(int));
        fout.write(reinterpret_cast<char *>(&offset2), sizeof(int));
    }
    square2.next_offset = square_tmp.next_offset;
    square2.back_offset = cur;
    square1.next_offset = offset2;
    square1.back_offset = square_tmp.back_offset;
    fout.seekp(cur);
    fout.write(reinterpret_cast<char*>(&square1),sizeof(block));
    fout.seekp(offset2);
    fout.write(reinterpret_cast<char *>(&square2),sizeof(block));
    fin.close();
    fout.close();
}

void block_list::merge(int cur1, int cur2)  {
    fstream ff;
    block block1,block2;
    ff.open(filename,ios::binary|ios::in|ios::out);
    if(!ff){cout<<"wenjie da bu kai"<<endl;return;}
    ff.seekg(cur1,ios::beg);
    ff.read(reinterpret_cast<char*>(&block1),sizeof(block));
    ff.seekg(cur2,ios::beg);
    ff.read(reinterpret_cast<char*>(&block2),sizeof(block));
    block1.next_offset=block2.next_offset;
    for(int i = 0 ; i < block2.length ; i++){
        block1.nodes[i+block1.length]=block2.nodes[i];
    }
    if(block2.next_offset>0)//cur2的下一块的上一块调成cur1
    {
        ff.seekp(block2.next_offset+sizeof(int));
        ff.write(reinterpret_cast<char*>(&cur1),sizeof(int));
    }
    ff.seekp(cur1,ios::beg);
    ff.write(reinterpret_cast<char*>(&block1),sizeof(block));
    ff.close();
}

void block_list::findnode_0(const string &key, vector<int> &offsets) {
    fstream ff1;
    ff1.open(filename,ios::binary|ios::out|ios::in);
    if(!ff1){cout<<"little brother your program have some programs find"<<endl;return;}
    ff1.seekg(0,ios::end);
    int cur=ff1.tellg();
    if(cur==0)//此文件中没有block//是吧？。。。。。。
    {
        //cout<<"ni zhao bu dao"<<endl;
        return;//删空气  要返回错误吗？//......
    }
    //cur!=0
    cur=0;
    int next=get_next_block(0);
    while(next!=-1)//找到一个块，其开头小于a，下一块的开头大于a
    {
        block mid;
        ff1.seekg(next,ios::beg);
        ff1.read(reinterpret_cast<char*>(&mid),sizeof(block));
        if(strcmp(key.c_str(),mid.nodes[0].key)<=0)break;
        cur=next;
        next=get_next_block(cur);//可能前后两块都有？
    }
    ff1.seekg(cur,ios::beg);
    block block_mid;
    ff1.read(reinterpret_cast<char*>(&block_mid),sizeof(block));
    for(int i=0;i<block_mid.length;i++)
    {
        if(strcmp(key.c_str(),block_mid.nodes[i].key)==0){
            offsets.push_back(block_mid.nodes[i].offset);
        }
    }
    ff1.seekg(block_mid.next_offset);
    ff1.read(reinterpret_cast<char*>(&block_mid),sizeof(block));
    for(int i=0;i<block_mid.length;i++)
    {
        if(strcmp(key.c_str(),block_mid.nodes[i].key)==0){
            offsets.push_back(block_mid.nodes[i].offset);
        }
    }//防止上文出现的情况
    ff1.close();
}


void block_list::findnode(const string &key, vector<int> &offsets){//2.0
    fstream fin,fout;
    fin.open(filename,ios::in | ios::out | ios::binary);
    fout.open(filename,ios::in | ios::out | ios::binary);
    if(!fin || !fout)throw"e";
    fin.seekg(0,ios::end);
    int x = fin.tellg();
    if(x == 0){
        block tmp;
        fout.seekp(0);
        fout.write(reinterpret_cast<const char *>(&tmp), sizeof(block));
        fout.close();
        return;
    }
    block tmp;
    int cur = 0;
    fin.seekg(cur);
    fin.read(reinterpret_cast<char*>(&tmp),sizeof(block));
    int next = tmp.next_offset;
    int pre = 0;
    while(next != -1){
        block b;
        fin.seekg(next);
        fin.read(reinterpret_cast<char*>(&b),sizeof(block));
        if(strcmp(key.c_str(),b.nodes[0].key) < 0)break;
        fin.seekg(cur);
        fin.read(reinterpret_cast<char*>(&b),sizeof(block));
        if(strcmp(key.c_str(),b.nodes[0].key) > 0)pre = cur;
        cur = next;
        block tmp1;
        fin.seekg(cur);
        fin.read(reinterpret_cast<char*>(&tmp1),sizeof(block));
        next = tmp1.next_offset;
    }
    while(true){
        fout.seekg(pre);
        block s;
        fout.read(reinterpret_cast<char*>(&s),sizeof(block));
        for(int i = 0 ; i < s.length ; i++)
        {
            if(strcmp(key.c_str(),s.nodes[i].key) == 0){
                offsets.push_back(s.nodes[i].offset);
            }
        }
        if(pre == next)break;
        pre = s.next_offset;
    }
    fin.close();
    fout.close();
}
