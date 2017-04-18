#include<iostream>
#include<array>
#include<random>
#include <algorithm>
#include<chrono>
const auto init(){
    std::array<bool,32> result;
    auto s=std::chrono::steady_clock::now().time_since_epoch().count();
    for(auto& v:result){
        v=s%2?true:false;
        s/=2;
    }
    return result;
}
/*const auto init(){
    std::random_device gen;
    std::mt19937 rnd(gen());
    std::array<bool,32> result;
    for(auto& v:result){
        v=static_cast<bool>(rnd()%2);
    }
    return result;
}*/
const auto lshift(const std::array<bool,32>& s,const int n){
    std::array<bool,32> result;
    result.fill(false);
    for(int i=31;i>=0;--i){
        if(i<=31-n){        
            result.at(i+n)=s.at(i);
        }
    }
    return result;
}
const auto rshift(const std::array<bool,32>& s,const int n){
    std::array<bool,32> result;
    result.fill(false);
    for(int i=0;i<32;++i){
        if(i>=n){        
            result.at(i-n)=s.at(i);
        }
    }
    return result;
}
const auto add(const std::array<bool,32>& s,const std::array<bool,32>& t){
    std::array<bool,32> result;
    for(int i=0;i<32;++i){
        result.at(i)=s.at(i)^t.at(i);
    }
    return result;
}
void write(const std::array<bool,32>& s){
    for(const auto&v:s){
        std::cout<<v;
    }
    std::cout<<std::endl;
}
void rwrite(const std::array<bool,32>& s){
    std::array<bool,32> t=s;
    std::reverse(t.begin(),t.end());
    write(t);
}
auto convert(const std::array<bool,32>& s){
    long root=1;
    long result=0;
    for(int i=0;i<32;++i){
        result+=(s.at(i)?1:0)*root;
        root*=2;
    }
    return result;
}
const auto XorShift(std::array<bool,32>& s){
    s=add(s,lshift(s,13));
    s=add(s,rshift(s,17));
    s=add(s,lshift(s,5));
    return convert(s);
}
int main(){

    auto s=init();

    for(int i=0;i<500;++i){
        std::cout<<XorShift(s)<<std::endl;
    }
    return 0;
}
