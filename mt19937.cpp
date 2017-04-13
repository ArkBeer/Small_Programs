#include<iostream>
#include<array>
class mt19937{
    static constexpr int N=624;
    static constexpr int M=397;
    static constexpr unsigned long MATRIX_A=0x9908b0dfUL;
    static constexpr unsigned long UPPER_MASK=0x80000000UL;
    static constexpr unsigned long LOWER_MASK=0x7fffffffUL;
    static std::array<unsigned long,N> mt;
    static int mti;
    void init_genrand(const unsigned long s){
        mt.at(0)=s&0xffffffffUL;
        for(auto it=mt.begin()+1;it!=mt.end();++it){
            *it=1812433253UL * (*(it-1) ^ (*(it-1) >> 30)) +std::distance(mt.begin(),it);
            *it &= 0xffffffffUL;
        }
        mti=N;
    }
    public:
    mt19937(){
        mti=N+1;
    }
    void init_by_array(const std::array<unsigned long,4>& init_key){
        const int key_length=init_key.size();
        init_genrand(19650218UL);
        for(int k=0;k<(N>key_length ? N : key_length);++k){
            if(k==N-1)mt.at(0)=mt.at(N-1);
            int i=(k+1)%N!=0?k+1:1;
            int j=k%key_length;
            mt.at(i) = (mt.at(i) ^ ((mt.at(i-1) ^ (mt.at(i-1) >> 30)) * 1664525UL))
                +init_key.at(j) + j;
            mt.at(i) &= 0xffffffffUL;
        }
        for(int k=0;k<N;++k){
            if(k==N-2)mt.at(0)=mt.at(N-1);
            int i=(k+2)%N!=0?k+2:1;
            mt.at(i) = (mt.at(i) ^ ((mt.at(i-1) ^ (mt.at(i-1) >> 30)) * 1566083941UL))            
                -i;
            mt.at(i) &= 0xffffffffUL;
        }
        mt.at(0)=0x80000000UL;
    }
    const unsigned long genrand_int32(){
        
    }
    
    mt19937(const mt19937&)=delete;
    mt19937(mt19937&&)=delete;
    mt19937 operator=(mt19937&&)=delete;
};
int main(){
    
    return 0;
}
