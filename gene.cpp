#include"Header.hpp"
#include<array>
#include<vector>

Ark::FrameRate fps;
Ark::Random rnd;
Ark::Key key;
const int w_x = 50, w_y = 50,num=100,gene_num=20;

class pyo {
	std::vector<int> gene;
	int x, y;
	int count;
	int child;
	std::vector<int> rndgene(const int gene_num) {
		std::vector<int> _gene;
		for (int i = 0; i < gene_num; ++i)_gene.push_back(rnd.Range(0, 4));
		return _gene; 
	}
	std::vector<int> makegene(const std::vector<int>& v1,const std::vector<int>& v2){
		std::vector<int> v;
		for (int i = 0; i < v1.size();++i) {
			if (rnd.Range(1, 10) != 1) {
				v.push_back(rnd.Range(0, 1) ? v1.at(i) : v2.at(i));
			}
			else v.push_back(rnd.Range(0, 4));
		}
		return v;
	}
public:
	pyo(const std::vector<int>& _gene, const int _x,const int _y):gene(_gene),x(_x),y(_y),count(0){}
	pyo(const std::vector<int>& _gene) :pyo(_gene, rnd.Range(0, w_x), rnd.Range(0, w_y)) {}
	pyo() :pyo(rndgene(gene_num)) {}
	pyo(const pyo& p1,const pyo& p2):pyo(makegene(p1.gene,p2.gene)) {}
	void move() {
		if (count!=gene.size()) {
			switch (gene.at(count)) {
			case 1://left
				x != 0 ? x -= 1 : x = w_x;
				break;
			case 2://right
				x != w_x ? x += 1 : x = 0;
				break;
			case 3://up
				y != 0 ? x -= 1 : y = w_y;
				break;
			case 4://down
				y != w_y ? y += 1 : y = 0;
				break;
			}
			++count;
		}
	}
	const int getx() { return x; }
	const int gety() { return y; }
	const bool is_dead() { return count == gene.size(); }
	friend Ark::TstringStream& operator<<(Ark::TstringStream& ost, const pyo& p);

};
	Ark::TstringStream& operator<<(Ark::TstringStream& ost, const pyo& p) {
		ost << "{";
		for (auto&v:p.gene) {
			ost << v;
		}
		ost << "}";
		return ost;
	}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	Ark::WinClass wc(hInstance);
	wc.AddStyle(WS_THICKFRAME).SetTitle(_T("Title")).SetSize(300,200);
	Ark::D2D1_1 d;
	
	std::vector<pyo> p1;
	std::vector<pyo> p2;
	for (int i = 0; i < num; ++i)p1.push_back(pyo());
	int counter=0;
	while (!wc.EndFlag()) {
		auto s = std::chrono::steady_clock::now();
		wc.LockAspectRatio(10, 16);
		auto rect=wc.GetSize();
		//if (key.KeyCheck('Z', false)) {
			for (auto&v:p1) {
				v.move();
			}
			for (auto it1 = p1.begin(); it1 != p1.end()-1;++it1) {
				for (auto it2 = it1 + 1; it2 != p1.end();++it2) {
					if (it1->getx() == it2->getx() && it1->gety() == it2->gety() && !it1->is_dead() && !it2->is_dead()) {
						p2.push_back(pyo(*it1, *it2));
						p2.push_back(pyo(*it1, *it2));

					}
				}
			}
			if (p1.at(0).is_dead()) {
				Ark::TstringStream tstr;
				tstr << p2.size()<<"\n";
				OutputDebugString(tstr.str().c_str());
				++counter;
				p1 = p2;
				p2.clear();
				if (p1.size() - 1 < num) {
					for (int i = p1.size(); i < num; ++i) p1.push_back(pyo{});
				}
			}
		//}
		d.BeginDraw(wc.GethWnd());
		d.DrawClear(RGB(0,66,66));

		Ark::TstringStream list;
		int c=1;
		for (auto&v:p1) {
			d.DrawRectangle(v.getx()*rect.bottom/50, v.gety()*rect.bottom / 50, (v.getx()+1)*rect.bottom / 50, (v.gety()+1)*rect.bottom / 50,RGB(0,250,150),1.0f);
			list << v;
			if (c%3== 0)list << "\n";
			++c;
		}
		d.DrawString(rect.bottom,0,list.str().c_str(),RGB(0,0,0),1.0f,rect.bottom/num*3);
		Ark::TstringStream countstr;
		countstr << counter;
		d.DrawString(0,0,countstr.str().c_str(),RGB(255,255,255),0.7f,rect.bottom/num*3);
		d.EndDraw();
		fps.Count();
		Ark::TstringStream tstr;
		//tstr << fps.perDuration<double>(s)<<"\n";
		OutputDebugString(tstr.str().c_str());
	}
	return 0;
}
