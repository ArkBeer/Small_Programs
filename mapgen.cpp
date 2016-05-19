#include<iostream>
#include<cmath>
#include<cstdlib>
#include<utility>
#include<algorithm>
#include<array>
#include<vector>
#include"ArkLib2_0.hpp"
#include"Frame_Rate.hpp"
#include"Collision_Detection.hpp"
//#include"D2D1_0.hpp"
#include"D2D1_1.hpp"
#include"audio.hpp"
const double PI = 3.141592;
//Ark::D2D1_0 d;
Ark::D2D1_1 d;
Ark::Frame_Rate fps;
Ark::Key::Key key;
Ark::Mouse::Mouse ms;
Ark::Random::Random rnd;
class Surface {
public:
	int value;
	Surface(const int v) :value(v) {}
	Surface() :Surface(0) {}
	Surface& operator=(const Surface& sf) {
		value = sf.value;
		return *this;
	}
};
struct Map {
	Surface surface;
	Map(const Surface& s):surface(s) {}
	Map() :Map(Surface()) {}
};
template<int X, int Y>
using Maps = std::array<std::array<Map, X>, Y>;
template<int X, int Y>
class World {
	struct Length {
		std::pair<int, int> Short;
		std::pair<int, int> Long;
		Length(const int x) :Short({ 0,0 }), Long({1,x-2}) {}
	};
	struct Split {
		Length Width;
		Length Height;
			Split(const int x, const int y):Width(Length(x)),Height(Length(y)) {}
	};
	Maps<X, Y> m;
	std::vector<Maps<X, Y>> recmap;
	const int width = 20;
	const double x_ = 400, y_ = 280;
	double x, y;
	Split split;
	void Init_Map(const Map& mp = Map()) {
		std::array<Map, X> it;
		it.fill(mp);
		m.fill(it);
	}
	inline const bool is_Wall(const Map& m) {
		return m.surface.value > 0 ? m.surface.value <= 3 : false;
	}
	inline void Length_Check(Length& l,const int x) {
		if (x - l.Long.first >= l.Long.second - x) {
			l.Short.first = x + 1;
			l.Short.second = l.Long.second;
			l.Long.second = x - 1;
		}
		else {
			l.Short.first = l.Long.first;
			l.Short.second = x - 1;
			l.Long.first = x + 1;
		}
	}

public:
	World() : m({}), recmap({}),x(0),y(0),split(Split(X,Y)) {}
	Map& map(const int x, const int y,const int z=-1) {
		return z==-1?m.at(y).at(x):recmap.at(z).at(y).at(x);
	}
	template <int N>
	void Create_Map(const bool is_Width,int s=0) {
		if (is_Width) {
			if (split.Width.Long.first + 6 < split.Width.Long.second - 6) {
				const int x = rnd.Range(split.Width.Long.first + 6, split.Width.Long.second - 6);
				for (int i = split.Height.Long.first; i <= split.Height.Long.second; ++i) {
					map(x, i).surface.value = -1;
				}
				Length_Check(split.Width, x);
				const int x_ = rnd.Range(split.Width.Short.first + 1, split.Width.Short.second - 4);
				const int w_ = rnd.Range(3, split.Width.Short.second - x_ - 1);
				const int y_ = rnd.Range(split.Height.Long.first + 1, split.Height.Long.second - 4);
				const int h_ = rnd.Range(3, split.Height.Long.second - y_ - 1);
				for (int i = split.Height.Long.first; i <= split.Height.Long.second; ++i) {
					for (int j = split.Width.Short.first; j <= split.Width.Short.second; ++j) {
						if (i >= y_ && i <= y_ + h_ && j >= x_ && j <= x_ + w_) {
							map(j, i).surface.value = 4;
						}
					}
				}
				const int o = rnd.Range(y_, y_ + h_);
				if (x < x_) {
					for (int j = split.Width.Short.first-1; j <= x_+1; ++j) {
						map(j, o).surface.value = 4;
					}
				}
				else {
					for (int j = x_ + w_-1; j <= split.Width.Short.second+1; ++j) {
						map(j, o).surface.value = 4;
					}
				}
				if (s != 0) {
					const int o_ = rnd.Range(x_, x_ + w_);
					if (s < y_) {
						for (int i = split.Height.Long.first-1; i <= y_+1; ++i) {
							map(o_, i).surface.value = 4;
						}
					}
					else {
						for (int i = y_ + h_-1; i <= split.Height.Long.second+1; ++i) {
							map(o_, i).surface.value = 4;
						}
					}
				}
				s = x;
				Create_Map<N - 1>(!is_Width, s);
			}
			else Create_Map<0>(is_Width,s);
		}else{
			if (split.Height.Long.first + 6 < split.Height.Long.second - 6) {
				const int y = rnd.Range(split.Height.Long.first + 6, split.Height.Long.second - 6);
				for (int i = split.Width.Long.first; i <= split.Width.Long.second; ++i) {
					map(i, y).surface.value = -2;
				}
				Length_Check(split.Height, y);
				const int x_ = rnd.Range(split.Width.Long.first + 1, split.Width.Long.second - 4);
				const int w_ = rnd.Range(3, split.Width.Long.second - x_ - 1);
				const int y_ = rnd.Range(split.Height.Short.first + 1, split.Height.Short.second - 4);
				const int h_ = rnd.Range(3, split.Height.Short.second - y_ - 1);
				for (int i = split.Height.Short.first; i <= split.Height.Short.second; ++i) {
					for (int j = split.Width.Long.first; j <= split.Width.Long.second; ++j) {
						if (i >= y_ && i <= y_ + h_ && j >= x_ && j <= x_ + w_) {
							map(j, i).surface.value = 4;
						}
					}
				}
				const int o = rnd.Range(x_, x_ + w_);
				if (y < y_) {
					for (int i = split.Height.Short.first-1; i <= y_-1; ++i) {
						map(o, i).surface.value = 4;
					}
				}
				else {
					for (int i = y_ + h_-1; i <= split.Height.Short.second+1; ++i) {
						map(o, i).surface.value = 4;
					}
				}
				if (s != 0) {
					const int o = rnd.Range(y_, y_ + h_);
					if (s < x_) {
						for (int j = split.Width.Long.first-1; j <= x_+1; ++j) {
							map(j, o).surface.value = 4;
						}
					}
					else {
						for (int j = x_ + w_-1; j <= split.Width.Long.second+1; ++j) {
							map(j, o).surface.value = 4;
						}
					}
				}
				s = y;
			Create_Map<N - 1>(!is_Width, s);
			}
			else Create_Map<0>(is_Width,s);
		}
	}
	template<>
	void Create_Map<0>(const bool is_Width ,int s) {
		const int x_ = rnd.Range(split.Width.Long.first + 1, split.Width.Long.second - 4);
		const int w_ = rnd.Range(3, split.Width.Long.second - x_ - 1);
		const int y_ = rnd.Range(split.Height.Long.first + 1, split.Height.Long.second - 4);
		const int h_ = rnd.Range(3, split.Height.Long.second - y_ - 1);
		for (int i = split.Height.Long.first; i <= split.Height.Long.second; ++i) {
			for (int j = split.Width.Long.first; j <= split.Width.Long.second; ++j) {
				if (i >= y_ && i <= y_ + h_ && j >= x_ && j <= x_ + w_) {
					map(j, i).surface.value = 4;
				}
			}
		}
		if (is_Width) {
			if (s != 0) {
				const int o_ = rnd.Range(x_, x_ + w_);
				if (s < y_) {
					for (int i = split.Height.Long.first-1; i <= y_+1; ++i) {
						map(o_, i).surface.value = 4;
					}
				}
				else {
					for (int i = y_ + h_-1; i <= split.Height.Long.second+1; ++i) {
						map(o_, i).surface.value = 4;
					}
				}
			}
		}
		else {
			if (s != 0) {
				const int o = rnd.Range(y_, y_ + h_);
				if (s < x_) {
					for (int j = split.Width.Long.first-1; j <= x_+1; ++j) {
						map(j, o).surface.value = 4;
					}
				}
				else {
					for (int j = x_ + w_-1; j <= split.Width.Long.second+1; ++j) {
						map(j, o).surface.value = 4;
					}
				}
			}
		}
		for (auto it1 = m.begin(); it1 < m.end();++it1) {
			for (auto it2 = it1->begin(); it2 < it1->end();++it2) {
				if (it2->surface.value == -2)it2->surface.value = 1;
				if (it2->surface.value == 4 && it2!=it1->end()-1 && (it2+1)->surface.value == -2)break;
			}
		}
		for (auto it1 = m.begin(); it1 < m.end(); ++it1) {
			for (auto it2 = it1->rbegin(); it2 < it1->rend(); ++it2) {
				if (it2->surface.value == -2)it2->surface.value = 1;
				if (it2->surface.value == 4 && it2 != it1->rend() - 1 && (it2 + 1)->surface.value == -2)break;
			}
		}
		for (int j = 0; j < m.at(0).size(); ++j) {
			for (int i = 0; i < m.size();++i) {
				if (map(j, i).surface.value == -1)map(j, i).surface.value = 1;
				if (map(j, i).surface.value == 4 && i!=m.size()-1 && map(j, i+1).surface.value == -1)break;
			}
		}
		for (int j = 0; j < m.at(0).size(); ++j) {
			for (int i = m.size()-1; i >= 0; --i) {
				if (map(j, i).surface.value == -1)map(j, i).surface.value = 1;
				if (map(j, i).surface.value == 4 && i!=0 && map(j, i - 1).surface.value == -1)break;
			}
		}
		for (auto& v1:m) {
			for (auto& v2:v1) {
				if (v2.surface.value==-1 || v2.surface.value==-2)v2.surface.value=4;
			}
		}
	}
	void Init_Dung_Map() {
		Init_Map();
		split = Split(X,Y);
		for (auto it1 = m.begin(); it1 < m.end();++it1) {
			for (auto it2 = it1->begin(); it2 < it1->end();++it2) {
				it2->surface.value = 1;
			}
		}
		Create_Map<10>(static_cast<bool>(rnd.Range(0,1)));
	}
	void Draw_Map() {
		for (int i = 0; i < 30; ++i) {
			for (int j = 0; j < 40; ++j) {
				switch (w.map(j, i).surface.value) {
				case 1:
					d.Draw_Rectangle(x+j * width, y+i * width, x+(j + 1) * width, y+(i + 1) * width, RGB(255, 0, 0), 1.0f);
					break;
				case 2:
					d.Draw_Rectangle(x+j * width, y+i * width, x+(j + 1) * width, y+(i + 1) * width, RGB(0, 255, 0), 1.0f);
					break;
				case 3:
					d.Draw_Rectangle(x+j * width, y+i * width, x+(j + 1) * width, y+(i + 1) * width, RGB(0,0,255), 1.0f);
					break;
				case 4:
					d.Draw_Rectangle(x+j * width, y+i * width, x+(j + 1) * width, y+(i + 1) * width, RGB(255, 0, 0), 1.0f,1);
					break;
				case 5:
					d.Draw_Rectangle(x+j * width, y+i * width, x+(j + 1) * width, y+(i + 1) * width, RGB(0,255, 0), 1.0f,1);
					break;
				case 6:
					d.Draw_Rectangle(x+j * width, y+i * width, x+(j + 1) * width, y+(i + 1) * width, RGB(0,0,255), 1.0f,1);
					break;

				}
			}
		}
	}
};
World<40, 30> w;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Ark::WinClass<Ark::D2D1_1> wc(hInstance, hPrevInstance, lpCmdLine, nCmdShow, d);
	wc.Set_Window_Title(_T("rogue"));
	wc.Set_Window_Size(800, 600);
	//w.Init_Dung_Map();
	wc.Boot();
	return 0;
}	

int Ark::WinClass<Ark::D2D1_1>::Main() {
	auto s = std::chrono::steady_clock::now();
	Begin_Draw();
	Draw_Clear(RGB(0, 0, 0));
	//w.Draw_Map();
	//if (key.Key_Check(VK_RETURN,false))w.Init_Dung_Map();
	fps.Count();
	Ark::Tstringstream tstr;
	tstr << static_cast<double>(1 / fps.Duration<double>(s));
	Draw_String(0, 0, tstr.str().c_str(), RGB(55, 255, 255), 1.0f, 20);
	End_Draw();
	return 0;
}

