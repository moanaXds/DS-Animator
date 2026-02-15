// File: Config.h
// Description: Contains all configuration constants for the Data Structure Visualizer.
// This makes it easy to tweak colors, sizes, and behavior in one place.

#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>

namespace Config {
    // ========================
    // WINDOW SETTINGS
    // ========================
    const unsigned int WINDOW_WIDTH = 1100;
    const unsigned int WINDOW_HEIGHT = 750;
    const std::string WINDOW_TITLE = "Data Structure Visualizer - DS Lab";

    // ========================
    // LAYOUT SETTINGS
    // ========================
    // Control panel on the left side
    const float CONTROL_PANEL_WIDTH = 220.0f;
    const float CONTROL_PANEL_PADDING = 15.0f;
    
    // Tree/structure drawing area starts after control panel
    const float TREE_AREA_X = CONTROL_PANEL_WIDTH + 20.0f;
    const float TREE_AREA_Y = 80.0f;
    const float TREE_AREA_WIDTH = WINDOW_WIDTH - TREE_AREA_X - 20.0f;
    const float TREE_AREA_HEIGHT = WINDOW_HEIGHT - TREE_AREA_Y - 20.0f;

    // ========================
    // NODE SETTINGS
    // ========================
    const float NODE_RADIUS = 25.0f;
    const float NODE_OUTLINE_THICKNESS = 3.0f;
    const float VERTICAL_SPACING = 70.0f;   // Space between tree levels
    const float MIN_HORIZONTAL_SPACING = 30.0f; // Minimum space between siblings
    
    // Linear structure settings (for LinkedList, Stack, Queue)
    const float LINEAR_NODE_WIDTH = 60.0f;
    const float LINEAR_NODE_HEIGHT = 50.0f;
    const float LINEAR_NODE_SPACING = 20.0f;

    // ========================
    // COLORS
    // ========================
    // Background colors
    const sf::Color BACKGROUND_COLOR(30, 30, 40);           // Dark blue-gray
    const sf::Color CONTROL_PANEL_COLOR(45, 45, 55);        // Slightly lighter
    const sf::Color TREE_AREA_COLOR(25, 25, 35);            // Dark area for tree
    const sf::Color MENU_BG_COLOR(35, 35, 50);              // Menu background

    // Node colors
    const sf::Color NODE_DEFAULT_FILL(70, 130, 180);        // Steel blue
    const sf::Color NODE_DEFAULT_OUTLINE(100, 160, 210);    // Lighter blue outline
    const sf::Color NODE_HIGHLIGHT_FILL(255, 200, 50);      // Yellow - currently visiting
    const sf::Color NODE_HIGHLIGHT_OUTLINE(255, 220, 100);
    const sf::Color NODE_FOUND_FILL(50, 205, 50);           // Lime green - found!
    const sf::Color NODE_FOUND_OUTLINE(100, 255, 100);
    const sf::Color NODE_DELETE_FILL(220, 50, 50);          // Red - about to delete
    const sf::Color NODE_DELETE_OUTLINE(255, 100, 100);
    const sf::Color NODE_NEW_FILL(138, 43, 226);            // Purple - newly inserted
    const sf::Color NODE_NEW_OUTLINE(180, 100, 255);
    
    // Data structure specific colors
    const sf::Color STACK_COLOR(255, 140, 0);               // Orange for stack
    const sf::Color STACK_OUTLINE(255, 180, 80);
    const sf::Color QUEUE_COLOR(0, 191, 255);               // Deep sky blue for queue
    const sf::Color QUEUE_OUTLINE(100, 210, 255);
    const sf::Color LINKEDLIST_COLOR(144, 238, 144);        // Light green for linked list
    const sf::Color LINKEDLIST_OUTLINE(180, 255, 180);

    // Edge/line colors
    const sf::Color EDGE_COLOR(120, 120, 140);              // Gray edges
    const sf::Color EDGE_HIGHLIGHT_COLOR(255, 200, 50);     // Yellow when traversing
    const sf::Color ARROW_COLOR(200, 200, 220);             // Arrow/pointer color

    // Text colors
    const sf::Color TEXT_COLOR(255, 255, 255);              // White text
    const sf::Color TEXT_SECONDARY(180, 180, 190);          // Gray secondary text
    const sf::Color ERROR_COLOR(255, 80, 80);               // Red for errors
    const sf::Color SUCCESS_COLOR(80, 255, 80);             // Green for success

    // GUI element colors
    const sf::Color BUTTON_IDLE(60, 60, 75);
    const sf::Color BUTTON_HOVER(80, 80, 100);
    const sf::Color BUTTON_PRESSED(100, 100, 130);
    const sf::Color BUTTON_SELECTED(70, 130, 180);          // Selected menu button
    const sf::Color TEXTBOX_BG(50, 50, 60);
    const sf::Color TEXTBOX_BORDER(100, 100, 120);
    const sf::Color TEXTBOX_ACTIVE_BORDER(130, 180, 255);
    const sf::Color SLIDER_TRACK(50, 50, 60);
    const sf::Color SLIDER_FILL(70, 130, 180);
    const sf::Color SLIDER_HANDLE(200, 200, 210);

    // ========================
    // ANIMATION SETTINGS
    // ========================
    const float DEFAULT_ANIMATION_DURATION = 0.5f;  // seconds per step
    const float MIN_ANIMATION_SPEED = 0.1f;         // Slowest (0.1x)
    const float MAX_ANIMATION_SPEED = 3.0f;         // Fastest (3x)
    const float DEFAULT_ANIMATION_SPEED = 1.0f;

    // ========================
    // FONT SETTINGS
    // ========================
    const unsigned int NODE_FONT_SIZE = 16;
    const unsigned int BUTTON_FONT_SIZE = 14;
    const unsigned int LABEL_FONT_SIZE = 12;
    const unsigned int TITLE_FONT_SIZE = 18;
    const unsigned int MESSAGE_FONT_SIZE = 13;
    const unsigned int MENU_TITLE_SIZE = 32;
    const unsigned int MENU_BUTTON_SIZE = 18;
}

#endif // CONFIG_H
