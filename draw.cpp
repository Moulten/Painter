#include "common.h"
#include <random>

using namespace std;

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
		Figure(params.pos_x, params.pos_y, params.color),  width(params.params[0]), height(params.params[1]) {}

	void Rect::Paint(HDC hdc) {
		
		HGDIOBJ old_brush = SelectObject(hdc, color);
		
		Rectangle(hdc, pos_x-width/2, pos_y - height/2, pos_x + width/2, pos_y + height/2);
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
		Ellipse(hdc, pos_x - radius / 2, pos_y - radius / 2, pos_x + radius / 2, pos_y + radius / 2);
		SelectObject(hdc, old_brush);

	}
	Circle& Circle::SetRadius(int value) {
		radius = value;
		return *this;
	}


}

	//------------------------------Фабрика фигур-------------------------------------------

FigureFactory::FigureFactory() :
	top(0), right(0), bottom(0), left(0) {
	FillColors();
}

FigureFactory::FigureFactory(int n_top, int n_left, int n_bottom, int n_right) : 
						top(n_top), left(n_left), bottom(n_bottom), right(n_right)  {
	FillColors();
}

unique_ptr<FigureObj::Rect> FigureFactory::CreateRect(const string& input_params) {
	FigureParams p = ParseInput(input_params);
	if (!(p.params.empty()) &&
		(p.pos_y + p.params[0] / 2) < bottom &&
		(p.pos_x + p.params[1] / 2) < right &&
		(p.pos_y - p.params[0] / 2) > top &&
		(p.pos_x - p.params[1] / 2) > left) {
		return make_unique<FigureObj::Rect>(move(p));
	}

	return {};
	}

unique_ptr<FigureObj::Circle> FigureFactory::CreateCircle(const string& input_params) {
	FigureParams p = ParseInput(input_params);
	if (!(p.params.empty()) &&
		(p.pos_y + p.params[0]) < bottom &&
			(p.pos_x + p.params[0] < right) &&
				(p.pos_y - p.params[0] > top) &&
						(p.pos_x - p.params[0] > left)) {
		return make_unique<FigureObj::Circle>(move(p));
	}
	return {};
}

FigureParams FigureFactory::ParseInput(const string& input_params) {
	istringstream is(input_params);
	FigureParams ans;
	is >> ans.pos_x;
	ans.pos_x += left;
	is >> ans.pos_y;
	ans.pos_y += top;
	while (is.peek() != EOF) {
		int new_value;
		is >> new_value;
		ans.params.push_back(new_value);
	}
	ChangeColor();
	ans.color = current_color;
	
	return move(ans);
}
void FigureFactory::ChangeColor() {
	colors.push(current_color);
	current_color = colors.front();
	colors.pop();
	}

void FigureFactory::FillColors() {
	current_color = CreateSolidBrush(RGB(139, 0, 0));
	colors.push(CreateSolidBrush(RGB(255, 69, 0)));
	colors.push(CreateSolidBrush(RGB(0, 0, 128)));
	colors.push(CreateSolidBrush(RGB(0, 128, 128)));
	colors.push(CreateSolidBrush(RGB(218, 165, 32)));
	colors.push(CreateSolidBrush(RGB(75, 0, 130)));
	colors.push(CreateSolidBrush(RGB(0, 100, 0)));
	colors.push(CreateSolidBrush(RGB(255, 0, 255)));
	colors.push(CreateSolidBrush(RGB(139, 0, 139)));
	colors.push(CreateSolidBrush(RGB(255, 99, 71)));
	colors.push(CreateSolidBrush(RGB(199, 21, 133)));
}

FigureFactory::~FigureFactory() {

	while (!colors.empty()) {
		DeleteObject(colors.back());
		colors.pop();
	}
	DeleteObject(current_color);
}

//------------------------------------------------------------------------------------------