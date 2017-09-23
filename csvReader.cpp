#include<iostream>
#include<fstream>
#include<string>
#include<sstream>
#include<vector>
auto readData(std::string lp) {
	std::vector<std::string> v;
	std::ifstream ifs;
	ifs.open(lp,std::ios::in);
	std::string str;
	while (!ifs.eof()) {
		std::getline(ifs, str);
		std::stringstream ss;
		ss<<str;
		while (std::getline(ss, str, ',')) {
			v.push_back(str);
		}
	}
	return v;
}
int main(){
	auto v=readData("test.csv");
	for(auto& vec:v){
		std::cout<<vec<<std::endl;
	}
	return 0;
}
