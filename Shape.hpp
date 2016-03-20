#pragma once
#include<iostream>
#include<array>
#include<cmath>
namespace Ark {
	struct Vec2 {
		double x, y;
		Vec2(const double x_, const double y_) :x(x_), y(y_) {}
		Vec2() :Vec2(0, 0) {}
		Vec2 operator+(const Vec2& v2) {
			Vec2 v(x, y);
			v.x += v2.x;
			v.y += v2.y;
			return v;
		}
		Vec2 operator-(const Vec2& v2) {
			Vec2 v(x, y);
			v.x -= v2.x;
			v.y -= v2.y;
			return v;
		}
		Vec2& operator+=(const Vec2& v2) {
			x += v2.x, y += v2.y;
			return *this;
		}
		Vec2& operator-=(const Vec2& v2) {
			x -= v2.x, y -= v2.y;
			return *this;
		}
		Vec2& operator/=(const double n) {
			x /= n, y /= n;
			return *this;
		}
		Vec2& operator=(const Vec2& v2) {
			x = v2.x, y = v2.y;
			return *this;
		}
		bool operator==(const Vec2& v2) const {
			return (x == v2.x && y == v2.y);
		}
		bool operator!=(const Vec2& v2) const {
			return !(*this == v2);
		}
	};
	std::ostream& operator<<(std::ostream& ost, const Vec2& v2) {
		ost << "(" << v2.x << "," << v2.y << ")";
		return ost;
	}
	template<int N>
	struct Shape {
		std::array<Vec2, N> Vertex;
		Vec2 Gravity_Point;
		double Radius;
		Shape(const std::array<Vec2, N>& v2, const double r) :Shape(v2) {}
		Shape(const std::array<Vec2, N>& v2) :Vertex(v2) {
			Gravity_Point = Get_GP();
			Radius = Get_R();
		}
	private:
		Vec2 Get_GP() {
			Vec2 v2;
			for (const auto& v : Vertex)v2 += v;
			v2 /= N;
			return v2;
		}
		double Get_R() {
			Vec2 v2;
			for (auto v : Vertex) {
				Vec2 v3 = v - Gravity_Point;
				if (v3.x*v3.x + v3.y*v3.y >= v2.x*v2.x + v2.y*v2.y)v2 = v3;
			}
			return std::sqrt(v2.x*v2.x + v2.y*v2.y);
		}
	};
	template<int N>
	std::ostream& operator<<(std::ostream& ost, const Shape<N>& sp) {
		ost << "Vertex:";
		for (const auto& v : sp.Vertex) {
			ost << v << std::endl;
		}
		ost << "gp:" << sp.Gravity_Point << std::endl << "r:" << sp.Radius;
		return ost;

	}
	template<>
	Shape<2>::Shape(const std::array<Vec2, 2>& v2) :Vertex(v2) {
		Gravity_Point = Get_GP();
		Radius = 0;
	}
	template<>
	Shape<1>::Shape(const std::array<Vec2, 1>&v2, const double r) : Vertex(v2), Radius(r) {
		Gravity_Point = Get_GP();
	}
	std::array<Vec2, 4> Make_Rectangle(const Vec2& v2, const double width, const double height) {
		return{ v2,Vec2(v2.x + width,v2.y),Vec2(v2.x + width,v2.y + height),Vec2(v2.x,v2.y + height) };
	}
	typedef Shape<1> Circle;
	typedef Shape<2> Line;
	typedef Shape<3> Triangle;
	typedef Shape<4> Square;
}