#include "common.h"
#include <random>

using namespace std;

int bounded_rand(int range) {
	std::uniform_int_distribution<uint32_t> dist(0, range - 1);
	mt19937 rng(42);
	return dist(rng);
}

	//---------------------Фигура----------------------------------------------
Figure::Figure(int new_pos_x, int new_pos_y, HBRUSH new_color) :
		pos_x(new_pos_x), pos_y(new_pos_y), color(new_color) {}

Figure& Figure::SetPosX(int value) {
		pos_x = value;
		return *this;
	}
	Figure& Figure::SetPosY(int value) {
		pos_y = value;
		return *this;
	}
	Figure& Figure::SetColor(HBRUSH value) {
		color = value;
		return *this;
	}
	//--------------------------------------------------------------------------

namespace FigureObj {



	Rect::Rect(const FigureParams& params) :
		Figure(params.pos_x, params.pos_y, params.color), height(params.params[0]), width(params.params[1]) {}

	void Rect::Paint(HDC hdc) {
		HGDIOBJ old_brush = SelectObject(hdc, color);
		Rectangle(hdc, pos_x+width/2, pos_y + height/2, pos_x + width/2, pos_y + height/2);
		SelectObject(hdc, old_brush);
	}
	Rect& Rect::SetHeight(int value) {
		height = value;
		return *this;
	}
	Rect& Rect::SetWidth(int value) {
		width = value;
		return *this;
	}



	Circle::Circle(const FigureParams& params) :
		Figure(params.pos_x, params.pos_y, params.color), radius(params.params[0]) {}

	void Circle::Paint(HDC hdc) {
		HGDIOBJ old_brush = SelectObject(hdc, color);
		Ellipse(hdc, pos_x + radius / 2, pos_y + radius / 2, pos_x + radius / 2, pos_y + radius / 2);
		SelectObject(hdc, old_brush);

	}
	Circle& Circle::SetRadius(int value) {
		radius = value;
		return *this;
	}


}

	//------------------------------Фабрика фигур-------------------------------------------

FigureFactory::FigureFactory() : pos_x(0), pos_y(0) {
	current_color = CreateSolidBrush(RGB(bounded_rand(256), bounded_rand(256), bounded_rand(256)));
}

FigureFactory::FigureFactory(int x, int y) : pos_x(x), pos_y(y) {
	current_color = CreateSolidBrush(RGB(bounded_rand(256), bounded_rand(256), bounded_rand(256)));
}

unique_ptr<FigureObj::Rect> FigureFactory::CreateRect(const string& input_params) {
	return make_unique<FigureObj::Rect>(ParseInput(input_params));
	}

unique_ptr<FigureObj::Circle> FigureFactory::CreateCircle(const string& input_params) {
	return make_unique<FigureObj::Circle>(ParseInput(input_params));
	}

FigureParams FigureFactory::ParseInput(const string& input_params) {
	stringstream ss;
	FigureParams ans;
	ss >> ans.pos_x;
	ans.pos_x += pos_x;
	ss >> ans.pos_y;
	ans.pos_y += pos_y;
	while (ss) {
		int new_value;
		ss >> new_value;
		ans.params.push_back(new_value);
	}
	ChangeColor();
	ans.color = current_color;
	
	return move(ans);
}
void FigureFactory::ChangeColor() {
	DeleteObject(current_color);
	current_color = CreateSolidBrush(RGB(bounded_rand(256), bounded_rand(256), bounded_rand(256)));
	}

//------------------------------------------------------------------------------------------