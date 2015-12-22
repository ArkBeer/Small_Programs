#include<iostream>
#include<vector>
#include"ArkLib2.0\ArkLib2_0.hpp"
#include"ArkLib2.0\D2D1_1.hpp"
#include"ArkLib2.0\Frame_Rate.hpp"
#include<random>
std::random_device rnd;     
std::mt19937 mt(rnd());
int ranged_rand(int x,int y) {
	std::uniform_int_distribution<> rand_(x, y);
	return rand_(mt);
}

Ark::Frame_Rate fr;
Ark::D2D1_1 d;

struct Birds {
	double x, y, sx, sy;
	const COLORREF color;
	std::vector<Birds>::iterator nearit;
	double centerx, centery,cmem;
	Birds(double x_, double y_, double sx_, double sy_, COLORREF color_) :x(x_), y(y_), sx(sx_), sy(sy_), color(color_),centerx(x_),centery(y_) {}
	
};

struct duration {
	Ark::Frame_Rate* fr;
	std::chrono::time_point<std::chrono::steady_clock> s;
	duration(Ark::Frame_Rate* f):s(std::chrono::steady_clock::now()),fr(f) {}
	~duration(){
		Ark::Tstringstream tstr;
		tstr << static_cast<double>(1 / fr->Duration<double>(s));
		OutputDebugString((tstr.str() + _T("\n")).c_str());
	}
};

double check_distance_birds(const Birds& b1, const Birds& b2) {
	return (b1.x - b2.x)*(b1.x - b2.x) + (b1.y - b2.y)*(b1.y - b2.y);

}

bool check_distance(std::vector<Birds>::iterator it, std::vector<Birds>::iterator it_) {
	if (it == it_)return false;
	if (it == (*it).nearit || check_distance_birds((*it), (*it_)) < check_distance_birds((*it), (*(*it).nearit)))(*it).nearit = it_;
	return true;
}
double set_speed(const double bef,const double add,const double rad=0.01f) {
	return bef*(1-rad) + add*rad;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Ark::WinClass<int> wc(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
	fr.Fps = 60;
	wc.Set_Window_Size(Ark::Size_Width(1200),Ark::Size_Height(768));
	wc.Set_Window_Title(_T("boids"));
	//wc.Set_Window_Elements(GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX);
	//wc.Set_Window_Pos(nullptr,0,0,0,0);
	wc.Boot();
	return 0;
}
int x = 0;
int y = 0;
auto s = std::chrono::steady_clock::now();
bool lb = false, rb = false, sb = false;
bool init = false;
std::vector<Birds> vec;
int Ark::WinClass<int>::Main() {
	duration dr(&fr);
	Ark::D2D_Begin<D2D1_1> db(&d,hWnd);
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hWnd,&pt);
	x = pt.x;
	y = pt.y;
	RECT rc;
	GetClientRect(hWnd, &rc);
	d.Draw_Clear(RGB(255,255,255));
	if (GetAsyncKeyState(VK_SPACE)) {
		if (!sb) {
			for (int i = 0; i < 50; ++i) {
				vec.push_back(Birds(ranged_rand(0,1200), ranged_rand(0,768),ranged_rand(-2,2), ranged_rand(-2,2), RGB(ranged_rand(0,255), ranged_rand(0,255), ranged_rand(0,255))));
				(vec.end() - 1)->nearit = vec.end() - 1;
				init = false;
			}
		}
		sb = true;
	}
	else sb = false;

	if (GetAsyncKeyState(VK_LBUTTON)) {
		if (!lb) {

			vec.push_back(Birds(x, y, ranged_rand(-2,2), ranged_rand(-2,2),RGB(ranged_rand(0,255), ranged_rand(0,255), ranged_rand(0,255))));
			(vec.end() - 1)->nearit = vec.end() - 1;
			init = false;
		}
		lb = true;
	}
	else lb=false;
	if (GetAsyncKeyState(VK_RBUTTON)) {
		if (!rb) {

			if(!vec.empty())vec.pop_back();
		}
		rb = true;
	}
	else rb = false;
	if (!init) {
		for (auto it = vec.begin(); it != vec.end(); ++it) {
			it->nearit = it;
		}
		init = true;
	}
	for (auto it_ = vec.begin(); it_ < vec.end(); ++it_) {
		if (GetAsyncKeyState('Z')) {
			//if (it_->nearit == it_ || check_distance_birds(*it_, *it_->nearit)>10*10*2 || (it_->sx - it_->nearit->sx)*(it_->sx - it_->nearit->sx) + (it_->sy - it_->nearit->sy)*(it_->sy - it_->nearit->sy) > 1) {
				(*it_).centerx = (*it_).x;
				(*it_).centery = (*it_).y;
				(*it_).cmem = 1;
				for (auto it = vec.begin(); it < vec.end(); ++it) {
					if (check_distance_birds(*it_, *it) < 200 * 200) {
						(*it_).centerx += (*it).x;
						(*it_).centery += (*it).y;
						(*it_).cmem += 1;
					}
					if(!(it_->x<20&&it_->nearit->x>1180) && !(it_->nearit->x<20 && it_->x>1180) && !(it_->y<20 && it_->nearit->y>748) && !(it_->nearit->y<20 && it_->y>748))check_distance(it_, it);
					/*if (it != it_) {
						if (it_ == it_->nearit || check_distance_birds((*it_), (*it)) < check_distance_birds((*it_), (*(*it_).nearit)))(*it_).nearit = it;
					}*/
				}
				if (GetAsyncKeyState('X')) {
					d.Draw_Ellipse((*it_).x, (*it_).y, 5, 5, RGB(255, 0, 0), 1.0f, 2);
					d.Draw_Line((*it_).x, (*it_).y, (*(*it_).nearit).x, (*(*it_).nearit).y, RGB(255, 0, 0), 1.0f, 1);
				}
				(*it_).centerx = (*it_).centerx / (*it_).cmem;
				(*it_).centery = (*it_).centery / (*it_).cmem;
				if (/*check_distance_birds(*it_, *(*it_).nearit) > 1.0f*2 &&*/ check_distance_birds(*it_, *(*it_).nearit) < 30 * 30*2) {
					if ((it_->x - it_->nearit->x) > 1 || (it_->x - it_->nearit->x) < -1)(*it_).sx = set_speed(it_->sx, 50.0f / (it_->x - it_->nearit->x));
					if ((it_->y - it_->nearit->y) > 1 || (it_->y - it_->nearit->y) < -1)(*it_).sy = set_speed(it_->sy, 50.0f / (it_->y - it_->nearit->y));
				}
				else if (check_distance_birds(*it_, *(*it_).nearit)>70*70*2 &&check_distance_birds(*it_,*it_->nearit)<500*500) {
					(*it_).sx = set_speed(it_->sx, (it_->nearit->x - it_->x) / 30);
					(*it_).sy = set_speed(it_->sy, (it_->nearit->y - it_->y) / 30);
				}
				it_->sx = set_speed(it_->sx, it_->nearit->sx,0.05f);
				it_->sy = set_speed(it_->sy, it_->nearit->sy,0.05f);
				(*it_).sx = set_speed(it_->sx, (it_->centerx - it_->x) / 30);
				(*it_).sy = set_speed(it_->sy, (it_->centery - it_->y) / 30);
				if ((*it_).sx*(*it_).sx + (*it_).sy*(*it_).sy > 50 * 50 * 2)(*it_).sx *= 0.6f, (*it_).sy *= 0.6f;
				if ((*it_).sx*(*it_).sx + (*it_).sy*(*it_).sy > 8)(*it_).sx *= 0.95f, (*it_).sy *= 0.95f;
				if ((*it_).sx*(*it_).sx + (*it_).sy*(*it_).sy < 2)it_->sx *= 1.05f, it_->sy *= 1.05f;
			//}
			/*else {
				if (GetAsyncKeyState('X')) {
					d.Draw_Ellipse((*it_).x, (*it_).y, 5, 5, RGB(0, 255, 0), 1.0f, 2);
					d.Draw_Line((*it_).x, (*it_).y, (*(*it_).nearit).x, (*(*it_).nearit).y, RGB(0, 255, 0), 1.0f, 1);
				}
			}*/
		}
		if ((*it_).sx == 0 && (*it_).sy == 0)(*it_).sx = ranged_rand(-2, 2), (*it_).sy = ranged_rand(-2, 2);
		(*it_).x += (*it_).sx;
		(*it_).y += (*it_).sy;

		if ((*it_).x < rc.left - 5)(*it_).x = rc.right + 5;
		if ((*it_).x > rc.right + 5)(*it_).x = -5;
		if ((*it_).y < rc.top - 5)(*it_).y = rc.bottom + 5;
		if ((*it_).y > rc.bottom + 5)(*it_).y = -5;
		d.Draw_Ellipse((*it_).x, (*it_).y, 5, 5, (*it_).color, 0.5f);
		
	}

	fr.Count();
	return 0;
}