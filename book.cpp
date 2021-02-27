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

void block_list::addnode(node &a){//2.0
    fstream f_in,f_out;
    f_in.open(filename,ios::in | ios::binary);
    f_out.open(filename,ios::in | ios::out | ios::binary);
    if(!f_in || !f_out)cout<<"Invalid"<<"can open"<<endl;
    f_in.seekg(0,ios::end);
    int offset_mid = f_in.tellg();
    if(offset_mid == 0){
        f_out.seekp(0,ios::beg);
        block mid;
        f_out.write(reinterpret_cast<char*>(&mid),sizeof(block));
    }
    int cur_off = 0;
    int next = get_next_block(cur_off);
    while(next != -1){
        f_in.seekg(next,ios::beg);
        block mid_;
        f_in.read(reinterpret_cast<char*>(&mid_),sizeof(block));
        if(strcmp(a.key,mid_.nodes[0].key) < 0)break;
        else{
        cur_off = next;
        next = get_next_block(cur_off);
        }
    }
    int p = -1;
    bool flag = false;
    f_in.seekg(cur_off,ios::beg);
    block mid__;
    f_in.read(reinterpret_cast<char*>(&mid__),sizeof(block));
    for(int i = 0 ; i < mid__.length ; i++)
    {
        if(strcmp(a.key,mid__.nodes[i].key) < 0){
            p = i;
            flag = true;
            break;
        }
    }
    if(!flag)p = mid__.length;
    for(int i = mid__.length; i > p ; i--)mid__.nodes[i] = mid__.nodes[i - 1];
    mid__.nodes[p] = a; mid__.length++;
    f_out.seekp(cur_off,ios::beg);
    f_out.write(reinterpret_cast<char*>(&mid__),sizeof(block));
    f_in.close();
    f_out.close();
    if(mid__.length >= 200)split(cur_off);
}



void block_list::deletenode(node &a){//2.0
    fstream f_in,f_out;
    f_in.open(filename, ios::binary|ios::in );
    f_out.open(filename,  ios::binary|ios::in | ios::out);
    if(!f_in || !f_out)cout<<"Invalid"<<"can open"<<endl;
    f_in.seekg(0,ios::end);
    int offset_ = f_in.tellg();
    if(offset_ == 0){

        //cout<<"Invalid"<<endl;
        f_in.close();
        f_out.close();
        return;
    }
    int cur_off = 0;
    int next = get_next_block(cur_off);
    while(next != -1){
        f_in.seekg(next);
        block mid_;
        f_in.read(reinterpret_cast<char*>(&mid_),sizeof(block));
        if(strcmp(a.key,mid_.nodes[0].key) < 0)break;
        cur_off = next; next = get_next_block(cur_off);
    }
    bool flag = false;
    int p ;
    f_out.seekg(cur_off);
    block mid__;
    f_out.read(reinterpret_cast<char*>(&mid__),sizeof(block));
    for(int i = 0 ; i < mid__.length ; i++)
    {
        if(strcmp(a.key,mid__.nodes[i].key) == 0 && a.offset == mid__.nodes[i].offset){
            p = i;
            flag = true;
            break;
        }
    }

    if(!flag){f_in.close();
        f_out.close();return;}
   // if(flag){cout<<"Invalid"<<endl;return;}

    for(int i = p ; i <= mid__.length - 2; i++)
    {
        mid__.nodes[i] = mid__.nodes[i + 1];
    }
    mid__.length--;
    f_out.seekp(cur_off,ios::beg);
    f_out.write(reinterpret_cast<char*>(&mid__),sizeof(block));
    f_in.seekg(cur_off);
    block mid_mid;
    f_in.read(reinterpret_cast<char*>(&mid_mid),sizeof(block));
    next = mid_mid.next_offset;
    int next_ = 0;
    if(next != -1) {
        block az;
        f_in.seekg(next);
        f_in.read(reinterpret_cast<char *>(&az), sizeof(block));
        next_ = az.length;
    }
    f_in.close();
    f_out.close();
    if(mid__.length + next_ <= -1 && next != -1)merge(cur_off,next);//不想要merge了
}



void block_list::split(int cur_off){
    block block1,block2,block_mid;
    fstream f_in,f_out;
    f_in.open(filename,ios::binary|ios::in );
    f_out.open(filename,ios ::binary|ios::out | ios :: in );
    if(!f_in || !f_out){cout<<"Invalid"<<endl;f_out.close();f_in.close();return;}
    f_out.seekg(cur_off,ios::beg);
    f_out.read(reinterpret_cast<char *>(&block_mid),sizeof(block));
    for(int i = 0 ; i <block_mid.length ; i++)
    {
        if(i < 150 ){
            block1.nodes[i] = block_mid.nodes[i];
        }
        else{
            block2.nodes[i - 150] = block_mid.nodes[i];
        }
    }
    block1.length = 150;
    block2.length = block_mid.length - 150;
    f_in.seekg(0,ios::end);
    int cur_offset2 = f_in.tellg();
    if(block_mid.next_offset >= 0) {
        f_out.seekp(block_mid.next_offset + sizeof(int));
        f_out.write(reinterpret_cast<char *>(&cur_offset2), sizeof(int));
    }
    block2.next_offset = block_mid.next_offset; block2.back_offset = cur_off;
    block1.next_offset = cur_offset2; block1.back_offset = block_mid.back_offset;
    f_out.seekp(cur_off,ios::beg);
    f_out.write(reinterpret_cast<char*>(&block1),sizeof(block));
    f_out.seekp(cur_offset2,ios::beg);
    f_out.write(reinterpret_cast<char *>(&block2),sizeof(block));
    f_in.close();
    f_out.close();
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



void block_list::findnode(const string &key, vector<int> &offsets){//2.0
    fstream f_in,f_out;
    f_in.open(filename, ios::binary|ios::in | ios::out );
    f_out.open(filename, ios::binary|ios::in | ios::out );
    if(!f_in || !f_out){cout<<"Invalid"<<endl;f_in.close();f_out;return;};
    f_in.seekg(0,ios::end);
    int off_ = f_in.tellg();
    if(off_ == 0){
        block mid;
        f_out.seekp(0,ios::beg);
        f_out.write(reinterpret_cast<const char *>(&mid), sizeof(block));
        f_out.close(); f_in.close();
        return;
    }
    block mid;
    int cur_off = 0;
    f_in.seekg(cur_off,ios::beg);  f_in.read(reinterpret_cast<char*>(&mid),sizeof(block));
    int next_block = mid.next_offset;
    int pre_off = 0;
    while(next_block != -1){
        block block_mid;
        f_in.seekg(next_block,ios::beg);
        f_in.read(reinterpret_cast<char*>(&block_mid),sizeof(block));
        if(strcmp(key.c_str(),block_mid.nodes[0].key) < 0)break;
        f_in.seekg(cur_off,ios::beg);f_in.read(reinterpret_cast<char*>(&block_mid),sizeof(block));
        if(strcmp(key.c_str(),block_mid.nodes[0].key) > 0)pre_off = cur_off;
        cur_off = next_block;
        block mid1;
        f_in.seekg(cur_off,ios::beg);
        f_in.read(reinterpret_cast<char*>(&mid1),sizeof(block));
        next_block = mid1.next_offset;
    }
    while(true)
    {
        if(pre_off==-1)break;
        f_out.seekg(pre_off,ios::beg);
        block mid__;
        f_out.read(reinterpret_cast<char*>(&mid__),sizeof(block));
        for(int i = 0 ; i < mid__.length ; i++)
        {
            if(strcmp(key.c_str(),mid__.nodes[i].key) == 0){
                offsets.push_back(mid__.nodes[i].offset);
            }
        }
        if(pre_off == next_block)break;
        pre_off = mid__.next_offset;
        if(pre_off==-1)break;
    }
    f_in.close();
    f_out.close();
}
