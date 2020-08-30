#include "common.h"

using namespace std;

//-----------------------------------------Основной экран-------------------------------------------
	FullScreen::FullScreen(RECT new_client_rect, HBRUSH New_Back = WHITE_BRUSH) 
		: ClientRect(new_client_rect), Back(New_Back) {}

	Screen& FullScreen::AddScreen(std::shared_ptr<Screen> new_screen) {
		screens.push_back(move(new_screen));
		return *this;
	}

	Screen& FullScreen::AddPicture(const std::string& input_params, FigureType type) {
		return *this;
	}

	void FullScreen::Paint(HDC hdc) {
		FillRect(hdc, &ClientRect, Back);


		Rectangle(hdc, ClientRect.left+100, ClientRect.top, ClientRect.right - 100, ClientRect.bottom);
		HBRUSH new_color = CreateSolidBrush(RGB(0, 0, 255));

		HGDIOBJ old_brush = SelectObject(hdc, new_color);
		Rectangle(hdc, ClientRect.left + 500, ClientRect.top + 500, ClientRect.right - 500, ClientRect.bottom - 500);
		SelectObject(hdc, old_brush);
		Ellipse(hdc, ClientRect.left + 500, ClientRect.top + 500, ClientRect.right - 500, ClientRect.bottom - 500);
		DeleteObject(new_color);
		for(auto& item: screens) {
			item->Paint(hdc);
		}
	}

	FullScreen::~FullScreen() {
	}

//--------------------------------------------------------------------------------------------------

//------------------------Экран отображения--------------------------------------------------------
	PaintScreen::PaintScreen(FigureParams params) : 
		borders(params), figure_factory(params.pos_x, params.pos_y) {}

	Screen& PaintScreen::AddScreen(std::shared_ptr<Screen> new_screen) {
		return *this;
	}

	Screen& PaintScreen::AddPicture(const std::string& input_params, FigureType type) {
		switch (type) {

		case FigureType::CIRCLE:
			pictures.push_back(figure_factory.CreateCircle(input_params));
			break;

		case FigureType::RECT:
			pictures.push_back(figure_factory.CreateRect(input_params));
			break;
		}
		return *this;
	}

	void PaintScreen::Paint(HDC hdc) {
		borders.Paint(hdc);
		for (auto& item : pictures) {
			item->Paint(hdc);
		}
	}

	PaintScreen::~PaintScreen() {

	}

//-------------------------------------------------------------------------------------------------