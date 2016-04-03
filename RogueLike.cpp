#include<iostream>
#include<array>
#include<vector>
#include<cmath>
#include<utility>
#include<algorithm>
#include"ArkLib2_0.hpp"
#include"Frame_Rate.hpp"
//#include"D2D1_0.hpp"
#include"D2D1_1.hpp"
const double PI = 3.141592;
//Ark::D2D1_0 d;
Ark::D2D1_1 d;
Ark::Frame_Rate fps;
Ark::Key::Key key;
Ark::Mouse::Mouse ms;
Ark::Random::Random rnd;
const Ark::Vec2 cc(400, 300);

struct Chip {
	std::vector<int> Block;
	std::pair<int, int> Condition;
	Chip() :Block({ 0 }), Condition(std::make_pair(0, 0)) {}
};
struct Actor {
	int x, y;
	Actor(int x_, int y_) :x(x_), y(y_) {}
};
Actor pc(9, 7);
Actor npc1(4, 3);
Actor npc2(12, 10);
std::array<std::array<Chip, 20>, 15> Map;
void Draw_Surface(const double x1, const double y1, const double w, const double h1, const double z, const double rad, COLORREF color1, const double f1) {
	const double rh1 = h1*std::cos(PI / 180 * rad);
	const double ry1 = cc.y - (cc.y - y1)*std::cos(PI / 180 * rad) - z*std::sin(PI / 180 * rad);
	d.Draw_Rectangle(x1, ry1, x1 + w, ry1 + rh1, color1, f1);
	d.Draw_Rectangle(x1, ry1, x1 + w, ry1 + rh1, RGB(0, 0, 0), f1, 1);
}

void Draw_Cube(const double x1, const double y1, const double w, const double h1, const double h2, const double z, const double rad, COLORREF color1, const double f1, COLORREF color2, const double f2) {
	const double rh1 = h1*std::cos(PI / 180 * rad);
	const double rh2 = h2*std::sin(PI / 180 * rad);
	const double ry1 = cc.y - (cc.y - y1)*std::cos(PI / 180 * rad) - rh2 - z*std::sin(PI / 180 * rad);
	d.Draw_Rectangle(x1, ry1, x1 + w, ry1 + rh1, color1, f1);
	d.Draw_Rectangle(x1, ry1, x1 + w, ry1 + rh1, RGB(0, 0, 0), f1, 1);
	d.Draw_Rectangle(x1, ry1 + rh1, x1 + w, ry1 + rh1 + rh2, color2, f2);
	d.Draw_Rectangle(x1, ry1 + rh1, x1 + w, ry1 + rh1 + rh2, RGB(0, 0, 0), f2, 1);
}
void Draw_NPC(const double x1, const double y1, const double w, const double h, const double z, const double rad, COLORREF color, const double f) {
	d.Draw_Rectangle(x1, cc.y - (cc.y - y1)*std::cos(PI / 180 * rad) - (h + z) * std::sin(PI / 180 * rad), x1 + w, cc.y - (cc.y - y1)*std::cos(PI / 180 * rad) - (h + z) * std::sin(PI / 180 * rad) + h, color, f);
}

const double calc(const double x1,const double y1,const double x2,const double y2,const double x,const double y) {
	return (x2-x1)*(y-y1)-(y2-y1)*(x-x1);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Ark::WinClass<Ark::D2D1_1> wc(hInstance, hPrevInstance, lpCmdLine, nCmdShow, d);
	wc.Set_Window_Title(_T("ace"));
	wc.Set_Window_Size(800, 600);
	for (int i = 0; i < Map.size(); ++i) {
		for (int j = 0; j < Map.at(0).size(); ++j) {
			if (i == 0 || i == Map.size()-1 || j == 0 || j == Map.at(0).size()-1)Map.at(i).at(j).Block.at(0) = 1;
		}
	}
	for (int i = 0; i < 20; ++i)Map.at(rnd.Range(0, 14)).at(rnd.Range(0, 19)).Block.at(0) = 1;
	wc.Boot();
	return 0;
}
int rad = 0;
int Ark::WinClass<Ark::D2D1_1>::Main() {
	auto s = std::chrono::steady_clock::now();
	Draw_Type.Begin_Draw(hWnd);
	Draw_Type.Draw_Clear(RGB(0, 0, 0));
	for (int i = 0; i < 15;++i) {
		for (int j = 0; j < 20;++j) {
			if (Map.at(i).at(j).Block.at(0) == 0) {
				Draw_Surface(j * 40, i * 40, 40, 40, 0, rad, RGB(25,25,25), 1.0f);
			}
			bool result = false;
			if (result == false && Map.at(i).at(j).Block.at(0)==0) {
				for (int i_ = 0; i_ < 15; ++i_) {
					for (int j_ = 0; j_ < 20; ++j_) {
						if (Map.at(i_).at(j_).Block.at(0) == 1) {
							if (calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40, i_ * 40)*calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40 + 40, i_ * 40 + 40) < 0 || calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40, i_ * 40 + 40)*calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40 + 40, i_ * 40) < 0) {
								if (calc(j_ * 40, i_ * 40, j_ * 40 + 40, i_ * 40 + 40, pc.x * 40 + 20, pc.y * 40 + 20)*calc(j_ * 40, i_ * 40, j_ * 40 + 40, i_ * 40 + 40, j * 40 + 20, i * 40 + 20) < 0 || calc(j_ * 40, i_ * 40 + 40, j_ * 40 + 40, i_ * 40, pc.x * 40 + 20, pc.y * 40 + 20)*calc(j_ * 40, i_ * 40 + 40, j_ * 40 + 40, i_ * 40, j * 40 + 20, i * 40 + 20) < 0) {
									Draw_Surface(j * 40, i * 40, 40, 40, 0, rad, RGB(50,50,200), 0.3f);
									result = true;
								}
							}
						}
						if (result)break;
					}
					if (result)break;
				}
			}
			if (!result && Map.at(i).at(j).Block.at(0) == 0) {
				const double  r = std::sqrt((j - pc.x)*(j - pc.x) + (i - pc.y)*(i - pc.y));
				Draw_Surface(j * 40, i * 40, 40, 40, 0, rad, RGB(255,200,200), 0.1f*(7-r));
			}
			if (Map.at(i).at(j).Block.at(0) == 1) {
				Draw_Cube(j * 40, i * 40, 40, 40, 40, 0, rad, RGB(155, 155, 155), 1.0f, RGB(50, 50, 50), 1.0f);
			}
			if (result == false && Map.at(i).at(j).Block.at(0) == 1) {
				for (int i_ = 0; i_ < 15; ++i_) {
					for (int j_ = 0; j_ < 20; ++j_) {
						if (i!=i_ && j!=j_ && Map.at(i_).at(j_).Block.at(0) == 1) {
							if (calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40, i_ * 40)*calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40 + 40, i_ * 40 + 40) < 0 || calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40, i_ * 40 + 40)*calc(pc.x * 40 + 20, pc.y * 40 + 20, 40 * j + 20, 40 * i + 20, j_ * 40 + 40, i_ * 40) < 0) {
								if (calc(j_ * 40, i_ * 40, j_ * 40 + 40, i_ * 40 + 40, pc.x * 40 + 20, pc.y * 40 + 20)*calc(j_ * 40, i_ * 40, j_ * 40 + 40, i_ * 40 + 40, j * 40 + 20, i * 40 + 20) < 0 || calc(j_ * 40, i_ * 40 + 40, j_ * 40 + 40, i_ * 40, pc.x * 40 + 20, pc.y * 40 + 20)*calc(j_ * 40, i_ * 40 + 40, j_ * 40 + 40, i_ * 40, j * 40 + 20, i * 40 + 20) < 0) {
									Draw_Surface(j * 40, i * 40, 40, 40, 0, rad, RGB(50, 50, 200), 0.3f);
									result = true;
								}
							}
						}
						if (result)break;
					}
					if (result)break;
				}
			}
			if (!result && Map.at(i).at(j).Block.at(0) == 1) {
				const double  r = std::sqrt((j - pc.x)*(j - pc.x) + (i - pc.y)*(i - pc.y));
				Draw_Cube(j * 40, i * 40, 40, 40,40, 0, rad,0,0, RGB(255, 200, 200), 0.1f*(7 - r));
			}
			if (pc.x == j && pc.y == i) {
				Draw_NPC(j * 40, i * 40, 40, 40, 0, rad, RGB(255, 120, 25), 1.0f);
			}
			if (!result && npc1.x == j && npc1.y == i) {
				Draw_NPC(j * 40, i * 40, 40, 40, 0, rad, RGB(25, 120, 255), 1.0f);
			}
			if (!result && npc2.x == j && npc2.y == i) {
				Draw_NPC(j * 40, i * 40, 40, 40, 0, rad, RGB(25, 255, 120), 1.0f);
			}

		}
	}
	d.Draw_Rectangle(0, 0, 800, 600, RGB(0, 0, 0), 0.3f);
	if (key.Key_Check(VK_LCONTROL, true)) {
		if (rad != 0 && key.Key_Check(VK_UP, true)) {
			rad -= 1;
		}
		if (rad != 90 && key.Key_Check(VK_DOWN, true)) {
			rad += 1;
		}
	}
	else {
		if ((key.Key_Check(VK_UP, false) || key.Key_Check(VK_NUMPAD8,false)) && pc.y>0 && Map.at(pc.y - 1).at(pc.x).Block.at(0) != 1) {
			pc.y -= 1;
		}
		if ((key.Key_Check(VK_DOWN, false) || key.Key_Check(VK_NUMPAD2, false)) && pc.y < 14 && Map.at(pc.y + 1).at(pc.x).Block.at(0) != 1) {
			pc.y += 1;
		}
		if ((key.Key_Check(VK_LEFT, false) || key.Key_Check(VK_NUMPAD4, false)) && pc.x > 0 && Map.at(pc.y).at(pc.x - 1).Block.at(0) != 1) {
			pc.x -= 1;
		}
		if ((key.Key_Check(VK_RIGHT, false) || key.Key_Check(VK_NUMPAD6, false)) && pc.x < 19 && Map.at(pc.y).at(pc.x + 1).Block.at(0) != 1) {
			pc.x += 1;
		}
		if (key.Key_Check(VK_NUMPAD7, false) && pc.x >0 &&pc.y>0 && Map.at(pc.y-1).at(pc.x - 1).Block.at(0) != 1) {
			pc.x -= 1;
			pc.y -= 1;
		}
		if (key.Key_Check(VK_NUMPAD9, false) && pc.x <19 && pc.y>0 && Map.at(pc.y - 1).at(pc.x + 1).Block.at(0) != 1) {
			pc.x += 1;
			pc.y -= 1;
		}
		if (key.Key_Check(VK_NUMPAD1, false) && pc.x >0 && pc.y<15 && Map.at(pc.y + 1).at(pc.x - 1).Block.at(0) != 1) {
			pc.x -= 1;
			pc.y += 1;
		}
		if (key.Key_Check(VK_NUMPAD3, false) && pc.x <19 && pc.y<15 && Map.at(pc.y + 1).at(pc.x + 1).Block.at(0) != 1) {
			pc.x += 1;
			pc.y += 1;
		}

	}
	if (pc.x == npc1.x && pc.y == npc1.y)std::swap(pc, npc1);
	if (pc.x == npc2.x && pc.y == npc2.y)std::swap(pc, npc2);
	if (rad < 0)rad = 0;
	if (rad > 90)rad = 90;
	Draw_Type.Draw_Ellipse(cc.x, cc.y, 2, 2, RGB(55, 255, 255), 1.0f);
	Ark::Tstringstream tstr;
	tstr << rad<<"\n";
	fps.Count();
	tstr << static_cast<double>(1 / fps.Duration<double>(s));
	d.Draw_String(0, 0, tstr.str().c_str(), RGB(55, 255, 255), 1.0f, 20);
	Draw_Type.End_Draw();
	return 0;
}