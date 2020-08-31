#include "common.h"

using namespace std;

//-----------------------------------------Основной экран-------------------------------------------
	FullScreen::FullScreen(RECT new_client_rect, HBRUSH New_Back = WHITE_BRUSH) 
		: ClientRect(new_client_rect), Back(New_Back) {}

	Screen& FullScreen::AddScreen(const string& name, unique_ptr<Screen> new_screen) {
		screens.insert({ name,move(new_screen) });
		return *this;
	}

	Screen& FullScreen::AddPicture(const string& input_params, FigureType type) {
		return *this;
	}

	FullScreen& FullScreen::AddWindow(const string& name, HWND wind) {
		windows.insert({ name, wind });
		return *this;
	}

	FullScreen& FullScreen::DeleteWindow(const string& name) {
		DestroyWindow(windows[name]);
		windows.erase(name);
		return *this;
	}

	HWND FullScreen::GetWindow(const string& name) {
		return windows.at(name);
	}

	Screen& FullScreen::GetScreen(const string& name) {
		return *(screens.at(name).get());
	}

	Screen& FullScreen::DeleteSub(const string name) {
		if (screens.count(name)) {
			screens.erase(name);
		}
		return *this;
	}

	void FullScreen::Paint(HDC hdc) {
		FillRect(hdc, &ClientRect, Back);
		for(auto& [name, screen]: screens) {
			screen->Paint(hdc);
		}
	}

	FullScreen::~FullScreen() {
		for (auto& [name, wind] : windows) {
			DestroyWindow(wind);
		}
	}
	

//--------------------------------------------------------------------------------------------------

//------------------------Экран отображения--------------------------------------------------------
	PaintScreen::PaintScreen(FigureParams params) : 
		borders(params), 
		figure_factory(
			params.pos_y - params.params[1] / 2,
			params.pos_x - params.params[0] / 2,
			params.pos_y + params.params[1] / 2,
			params.pos_x + params.params[0] / 2)
			 {}

	Screen& PaintScreen::AddScreen(const string& name, unique_ptr<Screen> new_screen) {
		return *this;
	}

	Screen& PaintScreen::AddPicture(const string& input_params, FigureType type) {
		unique_ptr<Figure> new_fig;
		
		switch (type) {
			
		case FigureType::CIRCLE:
			new_fig = figure_factory.CreateCircle(input_params);
			break;

		case FigureType::RECT:
			new_fig = figure_factory.CreateRect(input_params);
			break;
		}
		if (new_fig.get() != nullptr) {
			pictures.push_back(move(new_fig));
		}
		return *this;
	}
	
	Screen& PaintScreen::DeleteSub(const string name = "") {
		if (!pictures.empty()) {
			pictures.pop_back();
		}
		return *this;
	}

	void PaintScreen::Paint(HDC hdc) {
		HPEN new_pen = CreatePen(PS_SOLID, 4, BLACK_PEN);
		HGDIOBJ old_pen = SelectObject(hdc, new_pen);
		borders.Paint(hdc);
		SelectObject(hdc, old_pen);
		DeleteObject(new_pen);

		for (auto& item : pictures) {
			item->Paint(hdc);
		}
	}

	PaintScreen::~PaintScreen() {

	}

//-------------------------------------------------------------------------------------------------