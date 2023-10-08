# keylogger
A simple C++ program for Windows that outputs JSON data to stdout for key presses, mouse clicks, and mouse movement.

![Screenshot](./screenshot.png)

Download [keylogger.exe](./keylogger.exe).

The idea is that this program can be run as a subprocess from another script and its output can be parsed and used in other code, similar to a proper API.

## Outputs
All output from the program is in JSON format, and each object has a `type` property that indicates the structure of the object.

### Type `InitEvent`
* string[] `key_codes`: An array of all registered key codes.
* number `key_count`: The number of registered keys.

### Type `KeyEvent`
* string `code`: The code for the affected key.
* boolean `is_pressed`: `true` if the key is pressed, `false` otherwise.
* number `raw_state`: The internal value representing the key's current state.
* object `modifiers`: Contains properties for the state of modifier keys.
    * boolean `shift`: `true` if Shift is down, `false` otherwise.
    * boolean `ctrl`: `true` if Control is down, `false` otherwise.
    * boolean `alt`: `true` if Alt is down, `false` otherwise.
* object `coords`: Contains properties for the current mouse coordinates. These are set to `0` unless the key is `MouseLeft`, `MouseRight`, or `MouseMiddle`.
    * number `x`: The current X coordinate.
    * number `y`: The current Y coordinate.

### Type `MouseMoveEvent`
* number `x`: The current X coordinate.
* number `y`: The current Y coordinate.