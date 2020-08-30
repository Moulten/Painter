#pragma once

#include <windows.h>
#include <vector>
#include <memory>
#include <string>
#include <sstream>

struct FigureParams {
	int pos_x;
	int pos_y;

	std::vector<int> params;

	HBRUSH color;
};

enum class FigureType {
	CIRCLE,
	RECT
};

class Figure {
public:
	Figure(){}
	Figure(int new_pos_x, int new_pos_y, HBRUSH new_color);
	Figure& SetPosX(int);
	Figure& SetPosY(int);
	Figure& SetColor(HBRUSH);

	virtual void Paint(HDC) = 0;
public:
	int pos_x;
	int pos_y;
	HBRUSH color;
};

namespace FigureObj {


class Rect : public Figure {
public:
	Rect() {}
	Rect(const FigureParams& params);

	void Paint(HDC) final;
	Rect& SetHeight(int);
	Rect& SetWidth(int);
private:
	int height;
	int width;
};

class Circle : public Figure {
public:
	Circle() {}
	Circle(const FigureParams& params);

	void Paint(HDC) final;
	Circle& SetRadius(int);
private:

	int radius;
};

}



//Фабрика для создания фигур
class FigureFactory {
public:
	FigureFactory();
	FigureFactory(int x, int y);
	std::unique_ptr<FigureObj::Rect> CreateRect(const std::string& input_params);

	std::unique_ptr<FigureObj::Circle> CreateCircle(const std::string& input_params);
private:
	int pos_x;
	int pos_y;

	HBRUSH current_color;
	void ChangeColor();

	FigureParams ParseInput(const std::string& input_params);

};


class Screen {
public:
	virtual ~Screen() = default;
	virtual Screen& AddScreen(std::shared_ptr<Screen>) = 0;
	virtual Screen& AddPicture(const std::string& input_params, FigureType type) = 0;
	virtual void Paint(HDC) = 0;
};


class FullScreen : public Screen{
public:
	FullScreen() {}
	FullScreen(RECT new_client_rect, HBRUSH New_Back);

	Screen& AddScreen(std::shared_ptr<Screen> new_screen) override;

	Screen& AddPicture(const std::string& input_params, FigureType type) override;

	void Paint(HDC) override;

	~FullScreen() final;

private:	
	std::vector<std::shared_ptr<Screen>> screens;

	RECT ClientRect;   //
	HBRUSH Back;
};

class PaintScreen : public Screen {

	PaintScreen() {}

	PaintScreen(FigureParams params);

	Screen& AddScreen(std::shared_ptr<Screen> new_screen) override;

	Screen& AddPicture(const std::string& input_params, FigureType type) override;

	void Paint(HDC) override;

	~PaintScreen() override;

private:
	FigureObj::Rect borders;
	FigureFactory figure_factory;
	std::vector<std::unique_ptr<Figure>> pictures;
};
