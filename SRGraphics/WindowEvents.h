#pragma once

class WindowEvents {
private:
	enum class MousePos {
		Workspace = 0, UpToolBar = 2, Icon = 3, Collapse = 8, Expand = 9,
		LeftSide = 10, RightSide = 11, UpSide = 12, DownSide = 15, Close = 20
	};
	enum class MouseClickInToolBar {
		Idle = 160, LeftDown = 161, LeftUp = 162, Error = 49774
	};
	enum class Keyboard {
		Down = 256, Up = 257
	};
public:
	enum class Event {
		Unknown, Close, Collapse, Expand, Maximize, ALT_F4, LeftClick, MouseIdle
	};
public:
	static Event GetEvent(int msg, int param) {
		if (msg == (int)MouseClickInToolBar::LeftDown && param == (int)MousePos::Close)
			return Event::Close;

		return Event::Unknown;
	}
};