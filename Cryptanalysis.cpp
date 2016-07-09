#include<iostream>
#include<cmath>
#include<algorithm>
struct server{
	int p,q,e;
	int d;
	int msg;
	const int phi(){
		return (p-1)*(q-1);
	}
	void make_d(){
		d=1;
		while(true){
			if(e*d%phi()==1)break;
			++d;
		}
	}
};
const bool is_prime(const int n){
	int x=n-1;
	if(x<1)return false;
	while(x!=1){
		if(n%x==0)return false;
		x-=1;
	}
	return true;
}
const bool is_disjoint(const int n,const int m){
	const int x=std::max(n,m);
	const int y=std::min(n,m);
	int z=y;
	while(z!=1){
		if(y%z==0 && x%z==0){
			return false;
		}
		z-=1;
	}
	return true;
}
const int mod(const int n,int e,const int pq){
	int ans=1;
	while(e!=0){
		ans=ans*n%pq;
		e-=1;
	}
	return ans;
}

struct spy{
	int msg;
	int ans;
};
int main(){
	server svr;
	spy s;
	std::cout<<"p,q:";
	while(true){
			int x,y;
			std::cin>>x>>y;
			if(is_prime(x)&& is_prime(y)){
				svr.p=x,svr.q=y;
				break;
			}
	}
	std::cout<<"pq:"<<svr.p*svr.q<<", phi:"<<svr.phi()<<"\ne:";
	while(true){
		int x;
		std::cin>>x;
		if(is_disjoint(x,svr.phi())){
			svr.e=x;
			break;
		}
	}
	svr.make_d();
	std::cout<<"d:"<<svr.d<<std::endl;
	std::cout<<"msg:";
	while(true){
		int x;
		std::cin>>x;
		if(is_disjoint(x,svr.p*svr.q)){
			s.msg=x;
			break;
		}
	}
	s.ans=mod(s.msg,svr.e,svr.p*svr.q);
	std::cout<<"send:"<<s.ans<<std::endl;
	svr.msg=mod(s.ans,svr.d,svr.p*svr.q);
	std::cout<<"msg:"<<svr.msg<<"\nspy msg:";
	int msg;
	std::cin>>msg;
	std::cout<<"key(n,e) or key(d,n):";
	int n,e;
	std::cin>>n>>e;
	std::cout<<"spy send:"<<mod(msg,e,n);
	return 0;
}