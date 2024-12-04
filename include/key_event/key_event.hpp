#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/mouse.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

class KeyEvent {
private:
  int key;

public:
};

std::string Code(ftxui::Event event);

#endif // !KEY_EVENT_H
