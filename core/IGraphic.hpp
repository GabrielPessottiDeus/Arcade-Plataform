#pragma once

// include/IGraphic.hpp
struct InputEvent {
    enum InputKey {
        UP, DOWN, LEFT, RIGHT,
        ESCAPE, SPACE,RETURN, BUTTON1, BUTTON2, UNKNOWN, F_KEY
    };
    
    InputKey key;
    int x, y;
    bool pressed;
};

class IGraphic {
public:
    virtual ~IGraphic() = default;
    
    virtual void init() = 0;
    virtual void close() = 0;
    virtual bool pollEvent(InputEvent& event) = 0;
    virtual void draw(int x, int y, char c) = 0;
    virtual void clear() = 0;
    virtual void display() = 0;
};