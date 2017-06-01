#include"Header.hpp"
#include<array>
#include<vector>

Ark::FrameRate fps;
Ark::Random rnd;
Ark::Key key;
const int w_x = 50, w_y = 50;

class poyo {
	int life;
	std::vector<int> gene;
	int gcount;
	int x, y;
	const auto& makegene(std::vector<int> vec1, std::vector<int> vec2) {

		std::vector<int> vec;
		const int max = vec1.size() >= vec2.size() ? vec1.size() : vec2.size();
		const int min = vec1.size() <= vec2.size() ? vec1.size() : vec2.size();
		for (int i = min; i < max;++i) {
			if (vec1.size() != max) {
				vec1.push_back(rnd.Range(0, 4));
			}
			else vec2.push_back(rnd.Range(0, 4));
		}
		for (int i = 0; i < max;++i) {
			vec.push_back(vec1.at(i) == vec2.at(i) ? vec1.at(i) : (rnd.Range(0, 1) ? vec1.at(i):vec2.at(i)));
		}
		return vec;
	}
	void randomize() {
		life = life*(rnd.Range(0, 10) + 5) / 10;
		for (auto& v:gene) {
			if (rnd.Range(1, 10) == 1)v = rnd.Range(0, 4);
		}
	}
public:
	poyo(const std::vector<int>& _gene, const int _life, const int _x,const int _y):life(_life),gene(_gene), gcount(0),x(_x),y(_y) {
	
	}
	poyo() :poyo(std::vector<int>{rnd.Range(0,4), rnd.Range(0, 4), rnd.Range(0, 4), rnd.Range(0, 4), rnd.Range(0, 4)},5,rnd.Range(0,w_x-1), rnd.Range(0,w_y-1)) {}
	void move() {
		if (gcount!=gene.size()) {
			switch (gene.at(gcount)) {
			case 0:
				break;
			case 1:		//left
				x != 0 ? --x : x = w_x;
				break;
			case 2:		//right
				x != w_x ? ++x : x = 0;
				break;
			case 3:		//up
				y != 0 ? --y : y = w_y;
				break;
			case 4:		//down
				y != w_y ? ++y : y = 0;
				break;
			}
			++gcount;
		}
	}
	const int getx() { return x; }
	const int gety() { return y; }
	poyo& operator+(const poyo& r) {
		poyo p(makegene(this->gene,r.gene),rnd.Range(this->life,r.life),this->x,this->y);
		if (rnd.Range(1, 10)==1) {
			p.randomize();
		}
		return p;
	}
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	Ark::WinClass wc(hInstance);
	wc.AddStyle(WS_THICKFRAME).SetTitle(_T("Title")).SetSize(300,200);
	Ark::D2D1_1 d;
	poyo p1,p2;
	while (!wc.EndFlag()) {
		auto s = std::chrono::steady_clock::now();
		wc.LockAspectRatio(10, 16);
		auto rect=wc.GetSize();
		if (key.KeyCheck('Z', false))p1.move(), p2.move();
		d.BeginDraw(wc.GethWnd());
		d.DrawClear(RGB(0,66,66));
		d.DrawRectangle(p1.getx()*rect.bottom / 50, p1.gety()*rect.bottom/50, p1.getx()*rect.bottom / 50 + rect.bottom / 50, p1.gety()*rect.bottom / 50 + rect.bottom / 50, RGB(255, 0, 0), 1.0f);
		d.DrawRectangle(p2.getx()*rect.bottom / 50, p2.gety()*rect.bottom/50, p2.getx()*rect.bottom / 50 + rect.bottom / 50, p2.gety()*rect.bottom / 50 + rect.bottom / 50, RGB(0, 255, 0), 1.0f);
		d.EndDraw();
		fps.Count();
		Ark::TstringStream tstr;
		tstr << fps.perDuration<double>(s);
	}
	return 0;
}
