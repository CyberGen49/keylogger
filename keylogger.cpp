
// THIS IS AN EXPERIMENT
// Use Windows APIs to log system-wide mouse clicks and keystrokes
// Each event will be logged to the console as a JSON string

#include <iostream>
#include <windows.h>
#include <chrono>
#include <thread>

using namespace std;

void sleep(int ms) {
    // Requires <chrono> and <thread>
    this_thread::sleep_for(chrono::milliseconds(ms));
}

long getTimestamp() {
    // Requires <chrono>
    auto now = chrono::system_clock::now();
    auto now_ms = chrono::time_point_cast<chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch();
    return value.count();
}

string boolToString(bool value) {
    return value ? "true" : "false";
}

struct Key {
    int code;
    string name;
    bool isPressed = false;
    short rawState = 0;
    long x = 0;
    long y = 0;
};

Key keys[] = {
    // Letters
    Key{ 'A', "KeyA" }, Key{ 'B', "KeyB" }, Key{ 'C', "KeyC" },
    Key{ 'D', "KeyD" }, Key{ 'E', "KeyE" }, Key{ 'F', "KeyF" },
    Key{ 'G', "KeyG" }, Key{ 'H', "KeyH" }, Key{ 'I', "KeyI" },
    Key{ 'J', "KeyJ" }, Key{ 'K', "KeyK" }, Key{ 'L', "KeyL" },
    Key{ 'M', "KeyM" }, Key{ 'N', "KeyN" }, Key{ 'O', "KeyO" },
    Key{ 'P', "KeyP" }, Key{ 'Q', "KeyQ" }, Key{ 'R', "KeyR" },
    Key{ 'S', "KeyS" }, Key{ 'T', "KeyT" }, Key{ 'U', "KeyU" },
    Key{ 'V', "KeyV" }, Key{ 'W', "KeyW" }, Key{ 'X', "KeyX" },
    Key{ 'Y', "KeyY" }, Key{ 'Z', "KeyZ" },
    // Numbers
    Key{ '0', "Key0" }, Key{ '1', "Key1" }, Key{ '2', "Key2" },
    Key{ '3', "Key3" }, Key{ '4', "Key4" }, Key{ '5', "Key5" },
    Key{ '6', "Key6" }, Key{ '7', "Key7" }, Key{ '8', "Key8" },
    Key{ '9', "Key9" },
    // Symbols
    Key{ VK_OEM_1, "Semicolon" }, Key{ VK_OEM_PLUS, "Equal" },
    Key{ VK_OEM_COMMA, "Comma" }, Key{ VK_OEM_MINUS, "Minus" },
    Key{ VK_OEM_PERIOD, "Period" }, Key{ VK_OEM_2, "Slash" },
    Key{ VK_OEM_3, "Backtick" }, Key{ VK_OEM_4, "BracketLeft" },
    Key{ VK_OEM_5, "Backslash" }, Key{ VK_OEM_6, "BracketRight" },
    Key{ VK_OEM_7, "Quote" },
    // Function keys
    Key{ VK_F1, "KeyF1" }, Key{ VK_F2, "KeyF2" }, Key{ VK_F3, "KeyF3" },
    Key{ VK_F4, "KeyF4" }, Key{ VK_F5, "KeyF5" }, Key{ VK_F6, "KeyF6" },
    Key{ VK_F7, "KeyF7" }, Key{ VK_F8, "KeyF8" }, Key{ VK_F9, "KeyF9" },
    Key{ VK_F10, "KeyF10" }, Key{ VK_F11, "KeyF11" }, Key{ VK_F12, "KeyF12" },
    // Special keys
    Key{ VK_ESCAPE, "Escape" }, Key{ VK_TAB, "Tab" }, Key{ VK_CAPITAL, "CapsLock" },
    Key{ VK_SPACE, "Space" }, Key{ VK_RETURN, "Enter" }, Key{ VK_BACK, "Backspace" },
    Key{ VK_INSERT, "Insert" }, Key{ VK_DELETE, "Delete" }, Key{ VK_HOME, "Home" },
    Key{ VK_END, "End" }, Key{ VK_PRIOR, "PageUp" }, Key{ VK_NEXT, "PageDown" },
    Key{ VK_LEFT, "ArrowLeft" }, Key{ VK_RIGHT, "ArrowRight" },
    Key{ VK_UP, "ArrowUp" }, Key{ VK_DOWN, "ArrowDown" },
    Key{ VK_SNAPSHOT, "PrintScreen" }, Key{ VK_SCROLL, "ScrollLock" },
    Key{ VK_PAUSE, "Pause" }, Key{ VK_NUMLOCK, "NumLock" },
    Key{ VK_APPS, "ContextMenu" },
    // Windows key
    Key{ VK_LWIN, "Win" }, Key{ VK_RWIN, "Win" },
    // Numpad keys
    Key{ VK_NUMPAD0, "Numpad0" }, Key{ VK_NUMPAD1, "Numpad1" },
    Key{ VK_NUMPAD2, "Numpad2" }, Key{ VK_NUMPAD3, "Numpad3" },
    Key{ VK_NUMPAD4, "Numpad4" }, Key{ VK_NUMPAD5, "Numpad5" },
    Key{ VK_NUMPAD6, "Numpad6" }, Key{ VK_NUMPAD7, "Numpad7" },
    Key{ VK_NUMPAD8, "Numpad8" }, Key{ VK_NUMPAD9, "Numpad9" },
    Key{ VK_MULTIPLY, "NumpadMultiply" }, Key{ VK_ADD, "NumpadAdd" },
    Key{ VK_SUBTRACT, "NumpadSubtract" }, Key{ VK_DECIMAL, "NumpadDecimal" },
    Key{ VK_DIVIDE, "NumpadDivide" },
    // Media keys
    Key{ VK_VOLUME_MUTE, "VolumeMute" }, Key{ VK_VOLUME_DOWN, "VolumeDown" },
    Key{ VK_VOLUME_UP, "VolumeUp" }, Key{ VK_MEDIA_NEXT_TRACK, "MediaNext" },
    Key{ VK_MEDIA_PREV_TRACK, "MediaPrevious" }, Key{ VK_MEDIA_STOP, "MediaStop" },
    Key{ VK_MEDIA_PLAY_PAUSE, "MediaPlayPause" },
    // Modifier keys
    Key{ VK_SHIFT, "Shift" }, Key{ VK_CONTROL, "Control" },
    Key{ VK_MENU, "Alt" },
    // Mouse buttons
    Key{ VK_LBUTTON, "MouseLeft" }, Key{ VK_RBUTTON, "MouseRight" },
    Key{ VK_MBUTTON, "MouseMiddle" },
    Key{ VK_XBUTTON1, "MouseBack" }, Key{ VK_XBUTTON2, "MouseForward" }
};
const int keyCount = sizeof(keys) / sizeof(keys[0]);

void logInitEvent() {
    string keyNames = "";
    for (int i = 0; i < keyCount; i++) {
        keyNames += "\"" + keys[i].name + "\"";
        if (i < keyCount-1) {
            keyNames += ", ";
        }
    }
    printf(
        "{ \"type\": \"InitEvent\", \"key_codes\": [ %s ], \"key_count\": %d }\n",
        keyNames.c_str(),
        keyCount
    );
}

void logKeyEvent(Key key) {
    printf(
        "{ \"type\": \"KeyEvent\", \"code\": \"%s\", \"is_pressed\": %s, \"raw_state\": %d, \"modifiers\": { \"shift\": %s, \"ctrl\": %s, \"alt\": %s }, \"coords\": { \"x\": %ld, \"y\": %ld } }\n",
        key.name.c_str(),
        boolToString(key.isPressed).c_str(),
        key.rawState,
        boolToString(GetAsyncKeyState(VK_SHIFT)).c_str(),
        boolToString(GetAsyncKeyState(VK_CONTROL)).c_str(),
        boolToString(GetAsyncKeyState(VK_MENU)).c_str(),
        key.x,
        key.y
    );
}

void logMouseMoveEvent(long x, long y) {
    printf(
        "{ \"type\": \"MouseMoveEvent\", \"x\": %ld, \"y\": %ld }\n",
        x, y
    );
}

int main() {
    long startTime = getTimestamp();
    logInitEvent();
    long mouseX = 0;
    long mouseY = 0;
    while (true) {
        // Get updated mouse coordinates
        POINT point;
        GetCursorPos(&point);
        bool hasCoordsChanged = point.x != mouseX || point.y != mouseY;
        mouseX = point.x;
        mouseY = point.y;
        // Log mouse movements
        if (hasCoordsChanged) {
            logMouseMoveEvent(mouseX, mouseY);
        }
        // Loop through keys
        for (Key &key : keys) {
            // Get and update the current key state
            bool wasPressed = key.isPressed;
            key.rawState = GetAsyncKeyState(key.code);
            key.isPressed = key.rawState != 0;
            // Skip logging during the first second to stop
            // initialization events from being logged
            if ((getTimestamp()-startTime) < 1000)
                continue;
            // Log state changes
            bool isNewlyDown = key.isPressed && !wasPressed;
            bool isNewlyUp = !key.isPressed && wasPressed;
            bool hasChanged = isNewlyDown || isNewlyUp;
            if (hasChanged) {
                // Set coordinates if key is a mouse button
                if (key.name == "MouseLeft" || key.name == "MouseRight" || key.name == "MouseMiddle") {
                    key.x = mouseX;
                    key.y = mouseY;
                }
                logKeyEvent(key);
            }
        }
        sleep(17); // Cap at 60Hz
    }
    return 0;
}