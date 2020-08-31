#pragma once

#include <windows.h>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <unordered_map>

//��������� ������
struct FigureParams {
	int pos_x;
	int pos_y;

	std::vector<int> params; // ������ ����������

	HBRUSH color;
};

//���� �����
enum class FigureType {
	CIRCLE,
	RECT
};


//������� ����� ��� �����
class Figure {
public:
	Figure(){}
	Figure(int new_pos_x, int new_pos_y, HBRUSH new_color);
	//�������� ��������� ������
	Figure& SetPosX(int);
	Figure& SetPosY(int);

	//�������� ���� ������
	Figure& SetColor(HBRUSH);
	//��������� ������
	virtual void Paint(HDC) = 0;
protected:
	int pos_x;
	int pos_y;
	HBRUSH color;
};

namespace FigureObj {

// ����� ��� �������������� 
class Rect : public Figure {
public:
	Rect() {}
	Rect(const FigureParams& params);

	void Paint(HDC) override;
	Rect& SetHeight(int);
	Rect& SetWidth(int);
private:
	int width;
	int height;
	
};

//����� ��� �����
class Circle : public Figure {
public:
	Circle() {}
	Circle(const FigureParams& params);

	void Paint(HDC) override;
	Circle& SetRadius(int);
private:

	int radius;
};

}



//������� ��� �������� �����
class FigureFactory {
public:
	FigureFactory();
	//����������� ��������� ������� �������, � ������� ����� ������������ ������
	FigureFactory(int n_top, int n_left, int n_bottom, int n_right);

	//�������� �����
	std::unique_ptr<FigureObj::Rect> CreateRect(const std::string& input_params);

	std::unique_ptr<FigureObj::Circle> CreateCircle(const std::string& input_params);
	~FigureFactory();
private:
	int top;
	int left;
	int bottom;
	int right;

	HBRUSH current_color;
	// ����� �����
	//������ ���� ����������� � ����� �������
	void ChangeColor();
	std::queue<HBRUSH> colors;		//������� ��� ����� �������� �����

	//����������� ���������� �� ������ �����
	FigureParams ParseInput(const std::string& input_params);
	//���������� ������� colors � ���������� ����� current_collor
	void FillColors();

};

//������� ����� ��� �������
class Screen {
public:
	virtual ~Screen() = default;
	//�������� �����
	virtual Screen& AddScreen(const std::string& name, std::unique_ptr<Screen>) = 0;
	//�������� ������
	virtual Screen& AddPicture(const std::string& input_params, FigureType type) = 0;
	//���������� ����������
	virtual void Paint(HDC) = 0;
	//������� ������ �� ����� (���� ��� ����)
	virtual Screen& DeleteSub(const std::string name = "") = 0;
};

//�������� �����. �� ���������� 
class FullScreen : public Screen{
public:
	FullScreen() {}
	// ��������� �����������
	FullScreen& operator=(const FullScreen&) = delete;
	FullScreen(const FullScreen&) = delete;

	// �� ��������� - �����������
	FullScreen& operator=(FullScreen&&) = default;
	FullScreen(FullScreen&&) = default;

	FullScreen(RECT new_client_rect, HBRUSH New_Back);

	//�������� ����
	Screen& AddScreen(const std::string& name,std::unique_ptr<Screen> new_screen) override;

	//�� ������������
	Screen& AddPicture(const std::string& input_params, FigureType type) override;
	//���������
	void Paint(HDC) override;

	//�������� �������� �� ����� 
	Screen& DeleteSub(const std::string name = "") override;

	//�������� ���� � ������ � ���-�������
	FullScreen& AddWindow(const std::string& name, HWND wind);

	//�������� ���� �� �����
	FullScreen& DeleteWindow(const std::string& name);

	//�������� ������ ��� �������������� � ����� �� �����
	HWND GetWindow(const std::string& name);

	//�������� ��� �������������� ������ Screen �� �����
	Screen& GetScreen(const std::string& name);

	~FullScreen() final;

private:	
	std::unordered_map<std::string,std::unique_ptr<Screen>> screens; 
	std::unordered_map<std::string, HWND> windows;
	RECT ClientRect;   //  ������� ������� ����
	HBRUSH Back;
};

class PaintScreen : public Screen {
public:
	PaintScreen() {}

	PaintScreen(FigureParams params);

	//�������� ���� (�� ������������)
	Screen& AddScreen(const std::string& name, std::unique_ptr<Screen> new_screen) override;
	//�������� ������ � �����������
	Screen& AddPicture(const std::string& input_params, FigureType type) override;
	//������� ������ (��������)
	Screen& DeleteSub(const std::string) override;
	//��������� �����������
	void Paint(HDC) override;

	~PaintScreen() override;

private:
	FigureObj::Rect borders; //������������ ��� �����������
	FigureFactory figure_factory;		// ������� ��� �������� �����
	std::vector<std::unique_ptr<Figure>> pictures;		// ������ ����������� �����
};
