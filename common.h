#pragma once

#include <windows.h>
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>
#include <queue>
#include <unordered_map>

//Параметры фигуры
struct FigureParams {
	int pos_x;
	int pos_y;

	std::vector<int> params; // Вектор параметров

	HBRUSH color;
};

//Типы фигур
enum class FigureType {
	CIRCLE,
	RECT
};


//Базовый класс для фигур
class Figure {
public:
	Figure(){}
	Figure(int new_pos_x, int new_pos_y, HBRUSH new_color);
	//Изменить положение фигуры
	Figure& SetPosX(int);
	Figure& SetPosY(int);

	//Изменить цвет фигуры
	Figure& SetColor(HBRUSH);
	//Отрисовка фигуры
	virtual void Paint(HDC) = 0;
protected:
	int pos_x;
	int pos_y;
	HBRUSH color;
};

namespace FigureObj {

// Класс для прямоугольника 
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

//Класс для круга
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



//Фабрика для создания фигур
class FigureFactory {
public:
	FigureFactory();
	//Конструктор принимает границы области, в которой будет отрисовывать фигуры
	FigureFactory(int n_top, int n_left, int n_bottom, int n_right);

	//Создание фигур
	std::unique_ptr<FigureObj::Rect> CreateRect(const std::string& input_params);

	std::unique_ptr<FigureObj::Circle> CreateCircle(const std::string& input_params);
	~FigureFactory();
private:
	int top;
	int left;
	int bottom;
	int right;

	HBRUSH current_color;
	// Смена цвета
	//Старый цвет добавляется в конец очереди
	void ChangeColor();
	std::queue<HBRUSH> colors;		//Очередь для смены текущего цвета

	//Извдвечение параметров из строки ввода
	FigureParams ParseInput(const std::string& input_params);
	//Заполнение очереди colors и назначение цвета current_collor
	void FillColors();

};

//Базовый класс для экранов
class Screen {
public:
	virtual ~Screen() = default;
	//Добавить экран
	virtual Screen& AddScreen(const std::string& name, std::unique_ptr<Screen>) = 0;
	//Добавить Фигуру
	virtual Screen& AddPicture(const std::string& input_params, FigureType type) = 0;
	//Отрисовать содержимое
	virtual void Paint(HDC) = 0;
	//Удалить объект по имени (Если оно есть)
	virtual Screen& DeleteSub(const std::string name = "") = 0;
};

//Основной экран. Не копируется 
class FullScreen : public Screen{
public:
	FullScreen() {}
	// Запрещаем копирование
	FullScreen& operator=(const FullScreen&) = delete;
	FullScreen(const FullScreen&) = delete;

	// По умолчанию - перемещение
	FullScreen& operator=(FullScreen&&) = default;
	FullScreen(FullScreen&&) = default;

	FullScreen(RECT new_client_rect, HBRUSH New_Back);

	//Добавить окно
	Screen& AddScreen(const std::string& name,std::unique_ptr<Screen> new_screen) override;

	//Не используется
	Screen& AddPicture(const std::string& input_params, FigureType type) override;
	//Отрисовка
	void Paint(HDC) override;

	//Удаление объектов по имени 
	Screen& DeleteSub(const std::string name = "") override;

	//Добавить окно с именем в хэш-таблицу
	FullScreen& AddWindow(const std::string& name, HWND wind);

	//Удаление окон по имени
	FullScreen& DeleteWindow(const std::string& name);

	//Получить объект для взаимодействия с окном по имени
	HWND GetWindow(const std::string& name);

	//Получить для взаимодействия объект Screen по имени
	Screen& GetScreen(const std::string& name);

	~FullScreen() final;

private:	
	std::unordered_map<std::string,std::unique_ptr<Screen>> screens; 
	std::unordered_map<std::string, HWND> windows;
	RECT ClientRect;   //  Границы области окна
	HBRUSH Back;
};

class PaintScreen : public Screen {
public:
	PaintScreen() {}

	PaintScreen(FigureParams params);

	//Добавить окно (Не используется)
	Screen& AddScreen(const std::string& name, std::unique_ptr<Screen> new_screen) override;
	//Добавить фигуру с параметрами
	Screen& AddPicture(const std::string& input_params, FigureType type) override;
	//Удалить объект (Картинку)
	Screen& DeleteSub(const std::string) override;
	//Отрисовка содержимого
	void Paint(HDC) override;

	~PaintScreen() override;

private:
	FigureObj::Rect borders; //Пространство для отображения
	FigureFactory figure_factory;		// фабрика для создания фигур
	std::vector<std::unique_ptr<Figure>> pictures;		// вектор добавленных фигур
};
