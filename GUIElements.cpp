// File: GUIElements.cpp
// Description: Implementation of custom GUI elements.
// Contains Button, TextInput, Slider, and MessageBox implementations.

#include "GUIElements.h"
#include <sstream>
#include <algorithm>

// ============================================================================
// BUTTON IMPLEMENTATION
// ============================================================================

Button::Button(float x, float y, float width, float height,
               const std::string& text, sf::Font& fontRef)
    : font(&fontRef), isHovered(false), isPressed(false), enabled(true)
{
    // Setup the rectangle shape
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(Config::BUTTON_IDLE);
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color(100, 100, 120));
    
    // Setup the text label
    label.setFont(*font);
    label.setString(text);
    label.setCharacterSize(Config::BUTTON_FONT_SIZE);
    label.setFillColor(Config::TEXT_COLOR);
    
    // Center the text on the button
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);
    label.setPosition(x + width / 2.0f, y + height / 2.0f);
}

bool Button::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!enabled) return false;
    
    // Get mouse position relative to window
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), 
                           static_cast<float>(mousePos.y));
    
    // Check if mouse is over the button
    bool wasHovered = isHovered;
    isHovered = shape.getGlobalBounds().contains(mousePosF);
    
    // Update button color based on state
    if (isHovered) {
        shape.setFillColor(isPressed ? Config::BUTTON_PRESSED : Config::BUTTON_HOVER);
    } else {
        shape.setFillColor(Config::BUTTON_IDLE);
        isPressed = false;
    }
    
    // Handle mouse button events
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left && isHovered) {
            isPressed = true;
            shape.setFillColor(Config::BUTTON_PRESSED);
        }
    }
    
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            if (isPressed && isHovered) {
                isPressed = false;
                return true;  // Button was clicked!
            }
            isPressed = false;
        }
    }
    
    return false;
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(label);
}

void Button::setEnabled(bool isEnabled) {
    enabled = isEnabled;
    if (!enabled) {
        shape.setFillColor(sf::Color(50, 50, 55));
        label.setFillColor(sf::Color(100, 100, 100));
    } else {
        shape.setFillColor(Config::BUTTON_IDLE);
        label.setFillColor(Config::TEXT_COLOR);
    }
}

bool Button::isEnabled() const {
    return enabled;
}

void Button::setText(const std::string& text) {
    label.setString(text);
    // Re-center
    sf::FloatRect textBounds = label.getLocalBounds();
    label.setOrigin(textBounds.left + textBounds.width / 2.0f,
                    textBounds.top + textBounds.height / 2.0f);
}

// ============================================================================
// TEXT INPUT IMPLEMENTATION
// ============================================================================

TextInput::TextInput(float x, float y, float width, float height,
                     const std::string& placeholderText, sf::Font& fontRef,
                     bool numOnly)
    : font(&fontRef), isFocused(false), numericOnly(numOnly),
      cursorBlinkTimer(0), cursorVisible(true)
{
    // Setup the input box
    box.setPosition(x, y);
    box.setSize(sf::Vector2f(width, height));
    box.setFillColor(Config::TEXTBOX_BG);
    box.setOutlineThickness(2.0f);
    box.setOutlineColor(Config::TEXTBOX_BORDER);
    
    // Setup display text
    displayText.setFont(*font);
    displayText.setCharacterSize(Config::BUTTON_FONT_SIZE);
    displayText.setFillColor(Config::TEXT_COLOR);
    displayText.setPosition(x + 8, y + (height - Config::BUTTON_FONT_SIZE) / 2 - 2);
    
    // Setup placeholder text
    placeholder.setFont(*font);
    placeholder.setString(placeholderText);
    placeholder.setCharacterSize(Config::BUTTON_FONT_SIZE);
    placeholder.setFillColor(sf::Color(120, 120, 130));
    placeholder.setPosition(x + 8, y + (height - Config::BUTTON_FONT_SIZE) / 2 - 2);
    
    // Setup cursor
    cursor.setSize(sf::Vector2f(2, height - 10));
    cursor.setFillColor(Config::TEXT_COLOR);
    cursor.setPosition(x + 8, y + 5);
}

void TextInput::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // Check for click to focus/unfocus
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), 
                                   static_cast<float>(mousePos.y));
            
            bool wasFocused = isFocused;
            isFocused = box.getGlobalBounds().contains(mousePosF);
            
            // Update border color
            box.setOutlineColor(isFocused ? Config::TEXTBOX_ACTIVE_BORDER 
                                          : Config::TEXTBOX_BORDER);
            
            // Reset cursor when focused
            if (isFocused && !wasFocused) {
                cursorBlinkTimer = 0;
                cursorVisible = true;
            }
        }
    }
    
    // Handle text input when focused
    if (isFocused && event.type == sf::Event::TextEntered) {
        // Handle backspace
        if (event.text.unicode == 8) {  // Backspace
            if (!inputString.empty()) {
                inputString.pop_back();
                displayText.setString(inputString);
            }
        }
        // Handle regular characters
        else if (event.text.unicode >= 32 && event.text.unicode < 128) {
            char c = static_cast<char>(event.text.unicode);
            
            // Check if we should accept this character
            bool accept = true;
            if (numericOnly) {
                // Only accept digits and minus sign (at start)
                accept = (c >= '0' && c <= '9') || 
                         (c == '-' && inputString.empty());
            }
            
            if (accept && inputString.length() < 10) {  // Max 10 chars
                inputString += c;
                displayText.setString(inputString);
            }
        }
        
        // Update cursor position
        sf::FloatRect textBounds = displayText.getLocalBounds();
        cursor.setPosition(box.getPosition().x + 8 + textBounds.width + 2, 
                          box.getPosition().y + 5);
    }
    
    // Handle Enter key to unfocus
    if (isFocused && event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter || 
            event.key.code == sf::Keyboard::Return) {
            isFocused = false;
            box.setOutlineColor(Config::TEXTBOX_BORDER);
        }
    }
}

void TextInput::update(float deltaTime) {
    if (isFocused) {
        cursorBlinkTimer += deltaTime;
        if (cursorBlinkTimer >= 0.5f) {  // Blink every 0.5 seconds
            cursorBlinkTimer = 0;
            cursorVisible = !cursorVisible;
        }
    }
}

void TextInput::draw(sf::RenderWindow& window) {
    window.draw(box);
    
    if (inputString.empty()) {
        window.draw(placeholder);
    } else {
        window.draw(displayText);
    }
    
    // Draw cursor if focused and visible
    if (isFocused && cursorVisible) {
        window.draw(cursor);
    }
}

std::string TextInput::getText() const {
    return inputString;
}

void TextInput::clear() {
    inputString.clear();
    displayText.setString("");
    cursor.setPosition(box.getPosition().x + 8, box.getPosition().y + 5);
}

bool TextInput::isEmpty() const {
    return inputString.empty();
}

bool TextInput::getAsInt(int& result) const {
    if (inputString.empty()) return false;
    
    try {
        result = std::stoi(inputString);
        return true;
    } catch (...) {
        return false;
    }
}

// ============================================================================
// SLIDER IMPLEMENTATION
// ============================================================================

Slider::Slider(float x, float y, float width,
               float minVal, float maxVal, float initialVal,
               const std::string& label, sf::Font& fontRef)
    : font(&fontRef), minValue(minVal), maxValue(maxVal), 
      currentValue(initialVal), isDragging(false)
{
    float trackHeight = 6.0f;
    float handleRadius = 8.0f;
    
    // Setup track (background)
    track.setPosition(x, y + 20);
    track.setSize(sf::Vector2f(width, trackHeight));
    track.setFillColor(Config::SLIDER_TRACK);
    
    // Setup fill (left portion)
    fill.setPosition(x, y + 20);
    fill.setSize(sf::Vector2f(0, trackHeight));
    fill.setFillColor(Config::SLIDER_FILL);
    
    // Setup handle
    handle.setRadius(handleRadius);
    handle.setFillColor(Config::SLIDER_HANDLE);
    handle.setOrigin(handleRadius, handleRadius);
    handle.setPosition(x, y + 20 + trackHeight / 2);
    
    // Setup label text
    labelText.setFont(*font);
    labelText.setString(label);
    labelText.setCharacterSize(Config::LABEL_FONT_SIZE);
    labelText.setFillColor(Config::TEXT_SECONDARY);
    labelText.setPosition(x, y);
    
    // Setup value text
    valueText.setFont(*font);
    valueText.setCharacterSize(Config::LABEL_FONT_SIZE);
    valueText.setFillColor(Config::TEXT_COLOR);
    valueText.setPosition(x + width - 30, y);
    
    // Set initial position
    updateHandlePosition();
}

void Slider::updateHandlePosition() {
    float trackWidth = track.getSize().x;
    float trackX = track.getPosition().x;
    
    // Calculate position based on value
    float ratio = (currentValue - minValue) / (maxValue - minValue);
    float handleX = trackX + ratio * trackWidth;
    
    handle.setPosition(handleX, handle.getPosition().y);
    fill.setSize(sf::Vector2f(handleX - trackX, fill.getSize().y));
    
    // Update value text
    std::ostringstream ss;
    ss.precision(1);
    ss << std::fixed << currentValue << "x";
    valueText.setString(ss.str());
}

void Slider::updateValueFromHandle(float mouseX) {
    float trackX = track.getPosition().x;
    float trackWidth = track.getSize().x;
    
    // Clamp mouse position to track bounds
    float clampedX = std::max(trackX, std::min(mouseX, trackX + trackWidth));
    
    // Calculate value from position
    float ratio = (clampedX - trackX) / trackWidth;
    currentValue = minValue + ratio * (maxValue - minValue);
    
    // Update visual
    updateHandlePosition();
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f mousePosF(static_cast<float>(mousePos.x), 
                           static_cast<float>(mousePos.y));
    
    // Check for click on handle or track
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            // Check if clicking on handle
            float dx = mousePosF.x - handle.getPosition().x;
            float dy = mousePosF.y - handle.getPosition().y;
            float dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist <= handle.getRadius() + 5) {
                isDragging = true;
            }
            // Or clicking on track
            else if (track.getGlobalBounds().contains(mousePosF)) {
                isDragging = true;
                updateValueFromHandle(mousePosF.x);
            }
        }
    }
    
    // Handle dragging
    if (event.type == sf::Event::MouseMoved && isDragging) {
        updateValueFromHandle(mousePosF.x);
    }
    
    // Release drag
    if (event.type == sf::Event::MouseButtonReleased) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            isDragging = false;
        }
    }
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(labelText);
    window.draw(valueText);
    window.draw(track);
    window.draw(fill);
    window.draw(handle);
}

float Slider::getValue() const {
    return currentValue;
}

void Slider::setValue(float value) {
    currentValue = std::max(minValue, std::min(value, maxValue));
    updateHandlePosition();
}

// ============================================================================
// MESSAGE BOX IMPLEMENTATION
// ============================================================================

MessageBox::MessageBox(float x, float y, float width, sf::Font& fontRef)
    : font(&fontRef), displayTime(3.0f), timer(0), visible(false),
      currentType(INFO)
{
    // Setup background
    background.setPosition(x, y);
    background.setSize(sf::Vector2f(width, 40));
    background.setFillColor(sf::Color(50, 50, 60, 200));
    
    // Setup text
    messageText.setFont(*font);
    messageText.setCharacterSize(Config::MESSAGE_FONT_SIZE);
    messageText.setPosition(x + 10, y + 10);
}

void MessageBox::show(const std::string& message, MessageType type, float duration) {
    messageText.setString(message);
    currentType = type;
    displayTime = duration;
    timer = duration;
    visible = true;
    
    // Set color based on type
    switch (type) {
        case SUCCESS:
            messageText.setFillColor(Config::SUCCESS_COLOR);
            background.setFillColor(sf::Color(30, 60, 30, 220));
            break;
        case ERROR_MSG:
            messageText.setFillColor(Config::ERROR_COLOR);
            background.setFillColor(sf::Color(60, 30, 30, 220));
            break;
        case INFO:
        default:
            messageText.setFillColor(Config::TEXT_COLOR);
            background.setFillColor(sf::Color(50, 50, 60, 220));
            break;
    }
}

void MessageBox::update(float deltaTime) {
    if (visible) {
        timer -= deltaTime;
        if (timer <= 0) {
            visible = false;
        }
        // Fade out effect in last 0.5 seconds
        else if (timer < 0.5f) {
            float alpha = (timer / 0.5f) * 220;
            sf::Color bgColor = background.getFillColor();
            bgColor.a = static_cast<sf::Uint8>(alpha);
            background.setFillColor(bgColor);
            
            sf::Color textColor = messageText.getFillColor();
            textColor.a = static_cast<sf::Uint8>((timer / 0.5f) * 255);
            messageText.setFillColor(textColor);
        }
    }
}

void MessageBox::draw(sf::RenderWindow& window) {
    if (visible) {
        window.draw(background);
        window.draw(messageText);
    }
}

bool MessageBox::isVisible() const {
    return visible;
}

