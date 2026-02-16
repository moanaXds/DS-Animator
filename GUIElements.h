// File: GUIElements.h
// Description: Custom GUI elements built with SFML shapes.
// Contains: Button, TextInput, Slider, and MessageBox classes.
// These are simple implementations suitable for a beginner-level project.

#ifndef GUI_ELEMENTS_H
#define GUI_ELEMENTS_H

#include <SFML/Graphics.hpp>
#include <string>
#include <functional>
#include "Config.h"

// ============================================================================
// BUTTON CLASS
// ============================================================================
// A clickable button with text label.
// Changes color on hover and click for visual feedback.
// ============================================================================
class Button {
private:
    sf::RectangleShape shape;       // The button rectangle
    sf::Text label;                 // Text displayed on button
    sf::Font* font;                 // Pointer to font (shared)
    
    bool isHovered;                 // Mouse is over the button
    bool isPressed;                 // Button is being clicked
    bool enabled;                   // Button can be interacted with

public:
    // Constructor: position, size, text, and font
    Button(float x, float y, float width, float height, 
           const std::string& text, sf::Font& font);
    
    // Handle SFML events (mouse hover, click)
    // Returns true if button was clicked (mouse released on button)
    bool handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    // Draw the button
    void draw(sf::RenderWindow& window);
    
    // Enable/disable the button
    void setEnabled(bool enabled);
    bool isEnabled() const;
    
    // Update button text
    void setText(const std::string& text);
};

// ============================================================================
// TEXT INPUT CLASS
// ============================================================================
// A text field where users can type.
// Click to focus, type to enter text.
// Only accepts numeric input (for this BST project).
// ============================================================================
class TextInput {
private:
    sf::RectangleShape box;         // The input box
    sf::Text displayText;           // Text shown in the box
    sf::Text placeholder;           // Placeholder text when empty
    sf::Font* font;
    
    std::string inputString;        // The actual string content
    bool isFocused;                 // Is this input currently active?
    bool numericOnly;               // Only allow numbers?
    
    // Cursor blink animation
    sf::RectangleShape cursor;
    float cursorBlinkTimer;
    bool cursorVisible;

public:
    // Constructor: position, size, placeholder text, font
    TextInput(float x, float y, float width, float height,
              const std::string& placeholderText, sf::Font& font,
              bool numericOnly = true);
    
    // Handle events (click to focus, key presses)
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    // Update cursor blink animation
    void update(float deltaTime);
    
    // Draw the input box
    void draw(sf::RenderWindow& window);
    
    // Get the current input text
    std::string getText() const;
    
    // Clear the input
    void clear();
    
    // Check if input is empty
    bool isEmpty() const;
    
    // Try to get input as integer. Returns true if valid, stores result.
    bool getAsInt(int& result) const;
};

// ============================================================================
// SLIDER CLASS
// ============================================================================
// A horizontal slider for adjusting a value (like animation speed).
// Drag the handle to change the value.
// ============================================================================
class Slider {
private:
    sf::RectangleShape track;       // The background track
    sf::RectangleShape fill;        // Filled portion (left of handle)
    sf::CircleShape handle;         // The draggable handle
    sf::Text labelText;             // Label shown above slider
    sf::Text valueText;             // Current value shown
    sf::Font* font;
    
    float minValue, maxValue;       // Value range
    float currentValue;             // Current value
    bool isDragging;                // Is user dragging the handle?
    
    // Helper to calculate handle position from value
    void updateHandlePosition();
    // Helper to calculate value from handle position
    void updateValueFromHandle(float mouseX);

public:
    // Constructor: position, size, range, label, font
    Slider(float x, float y, float width, 
           float minVal, float maxVal, float initialVal,
           const std::string& label, sf::Font& font);
    
    // Handle events (click and drag)
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window);
    
    // Draw the slider
    void draw(sf::RenderWindow& window);
    
    // Get current value
    float getValue() const;
    
    // Set value programmatically
    void setValue(float value);
};

// ============================================================================
// MESSAGE BOX CLASS
// ============================================================================
// Displays messages to the user (errors, success, info).
// Messages fade out after a set duration.
// ============================================================================
class MessageBox {
public:
    enum MessageType {
        INFO,
        SUCCESS,
        ERROR_MSG  // Named ERROR_MSG to avoid conflict with macro
    };

private:
    sf::RectangleShape background;
    sf::Text messageText;
    sf::Font* font;
    
    float displayTime;              // How long to show message
    float timer;                    // Current countdown
    bool visible;                   // Is message currently shown?
    MessageType currentType;

public:
    // Constructor: position, width, font
    MessageBox(float x, float y, float width, sf::Font& font);
    
    // Show a message for a duration (seconds)
    void show(const std::string& message, MessageType type, float duration = 3.0f);
    
    // Update fade timer
    void update(float deltaTime);
    
    // Draw if visible
    void draw(sf::RenderWindow& window);
    
    // Check if currently visible
    bool isVisible() const;
};

#endif // GUI_ELEMENTS_H

