// ============================================================================
// FILE: main.cpp
// PROJECT: Data Structure Visualizer - Lab 16
// AUTHOR: [Your Name]
// DATE: December 2024
// 
// DESCRIPTION:
// This is the main entry point for the Data Structure Visualizer application.
// It provides an interactive GUI to visualize four data structures:
//   1. Binary Search Tree (BST) - hierarchical, sorted structure
//   2. Linked List - linear, dynamic sequence
//   3. Stack - LIFO (Last In First Out) 
//   4. Queue - FIFO (First In First Out)
//
// KEY FEATURES:
// - Animated insert, delete, search operations
// - Speed control slider for animations
// - Export visualization to PNG
// - Error handling with user feedback
// - Clean, modern GUI using SFML
//
// HOW IT WORKS:
// 1. Main menu lets user select a data structure
// 2. Each mode has its own visualization and controls
// 3. Press ESC or "Back to Menu" to return
// ============================================================================

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include "Config.h"
#include "BST.h"
#include "LinkedList.h"
#include "Stack.h"
#include "Queue.h"
#include "Visualizer.h"
#include "GUIElements.h"

// ============================================================================
// DATA STRUCTURE TYPE ENUMERATION
// Purpose: Track which visualization mode is active
// ============================================================================
enum class DataStructureType {
    NONE,           // Main menu screen
    BST,            // Binary Search Tree mode
    LINKED_LIST,    // Singly Linked List mode
    STACK,          // Stack (LIFO) mode
    QUEUE           // Queue (FIFO) mode
};

// ============================================================================
// FORWARD DECLARATIONS
// Each mode runs in its own function for clean separation
// ============================================================================
void runBSTMode(sf::RenderWindow& window, sf::Font& font);
void runLinkedListMode(sf::RenderWindow& window, sf::Font& font);
void runStackMode(sf::RenderWindow& window, sf::Font& font);
void runQueueMode(sf::RenderWindow& window, sf::Font& font);

// Helper function to export any visualization to PNG
bool exportVisualizationToPNG(sf::RenderWindow& window, const std::string& filename, 
                               float areaX, float areaY, float areaW, float areaH);

// ============================================================================
// FONT LOADER
// Tries multiple paths for cross-platform compatibility
// ============================================================================
bool loadFont(sf::Font& font) {
    std::vector<std::string> fontPaths = {
        "arial.ttf",                    // Current directory (most common)
        "fonts/arial.ttf",              // fonts subfolder
        "C:/Windows/Fonts/arial.ttf",   // Windows system fonts
        "C:/Windows/Fonts/segoeui.ttf", // Alternative Windows font
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",  // Linux
        "/System/Library/Fonts/Helvetica.ttc"  // macOS
    };
    
    for (const std::string& path : fontPaths) {
        if (font.loadFromFile(path)) {
            std::cout << "Font loaded successfully from: " << path << std::endl;
            return true;
        }
    }
    return false;
}

// ============================================================================
// MAIN FUNCTION
// Entry point - creates window, shows menu, delegates to mode functions
// ============================================================================
int main() {
    // Create the main application window
    sf::RenderWindow window(
        sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT),
        Config::WINDOW_TITLE,
        sf::Style::Close | sf::Style::Titlebar
    );
    window.setFramerateLimit(60);  // Smooth 60 FPS animation
    
    // Load font (required for all text rendering)
    sf::Font font;
    if (!loadFont(font)) {
        std::cerr << "CRITICAL ERROR: Could not load font file!" << std::endl;
        std::cerr << "Please ensure arial.ttf is in the executable directory." << std::endl;
        return -1;
    }
    
    // Track current mode (starts at menu)
    DataStructureType currentMode = DataStructureType::NONE;
    
    // ========================================================================
    // MAIN MENU SETUP
    // ========================================================================
    float menuCenterX = Config::WINDOW_WIDTH / 2.0f;
    float menuStartY = 220.0f;
    float buttonWidth = 320.0f;
    float buttonHeight = 55.0f;
    float buttonSpacing = 18.0f;
    
    // Create menu buttons for each data structure
    std::vector<Button> menuButtons;
    menuButtons.push_back(Button(menuCenterX - buttonWidth/2, menuStartY, 
                                  buttonWidth, buttonHeight, "Binary Search Tree (BST)", font));
    menuButtons.push_back(Button(menuCenterX - buttonWidth/2, menuStartY + (buttonHeight + buttonSpacing), 
                                  buttonWidth, buttonHeight, "Linked List", font));
    menuButtons.push_back(Button(menuCenterX - buttonWidth/2, menuStartY + 2*(buttonHeight + buttonSpacing), 
                                  buttonWidth, buttonHeight, "Stack (LIFO)", font));
    menuButtons.push_back(Button(menuCenterX - buttonWidth/2, menuStartY + 3*(buttonHeight + buttonSpacing), 
                                  buttonWidth, buttonHeight, "Queue (FIFO)", font));
    
    // Menu title text
    sf::Text menuTitle;
    menuTitle.setFont(font);
    menuTitle.setString("Data Structure Visualizer");
    menuTitle.setCharacterSize(36);
    menuTitle.setFillColor(Config::TEXT_COLOR);
    menuTitle.setStyle(sf::Text::Bold);
    sf::FloatRect titleBounds = menuTitle.getLocalBounds();
    menuTitle.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
    menuTitle.setPosition(menuCenterX, 80);
    
    // Subtitle
    sf::Text menuSubtitle;
    menuSubtitle.setFont(font);
    menuSubtitle.setString("Select a data structure to visualize:");
    menuSubtitle.setCharacterSize(18);
    menuSubtitle.setFillColor(Config::TEXT_SECONDARY);
    sf::FloatRect subtitleBounds = menuSubtitle.getLocalBounds();
    menuSubtitle.setOrigin(subtitleBounds.width / 2, subtitleBounds.height / 2);
    menuSubtitle.setPosition(menuCenterX, 150);
    
    // Instructions
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setString("Features: Insert | Delete | Search | Clear | Animation Speed | Export PNG");
    instructions.setCharacterSize(13);
    instructions.setFillColor(sf::Color(100, 140, 180));
    sf::FloatRect instrBounds = instructions.getLocalBounds();
    instructions.setOrigin(instrBounds.width / 2, instrBounds.height / 2);
    instructions.setPosition(menuCenterX, menuStartY + 4*(buttonHeight + buttonSpacing) + 40);
    
    // Footer
    sf::Text footer;
    footer.setFont(font);
    footer.setString("Lab 16 - Data Structures | Press ESC to return to menu");
    footer.setCharacterSize(12);
    footer.setFillColor(sf::Color(90, 90, 100));
    sf::FloatRect footerBounds = footer.getLocalBounds();
    footer.setOrigin(footerBounds.width / 2, footerBounds.height / 2);
    footer.setPosition(menuCenterX, Config::WINDOW_HEIGHT - 30);
    
    // ========================================================================
    // MAIN APPLICATION LOOP
    // ========================================================================
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Handle menu button clicks
            if (currentMode == DataStructureType::NONE) {
                for (size_t i = 0; i < menuButtons.size(); i++) {
                    if (menuButtons[i].handleEvent(event, window)) {
                        switch (i) {
                            case 0: currentMode = DataStructureType::BST; break;
                            case 1: currentMode = DataStructureType::LINKED_LIST; break;
                            case 2: currentMode = DataStructureType::STACK; break;
                            case 3: currentMode = DataStructureType::QUEUE; break;
                        }
                    }
                }
            }
        }
        
        // Render based on current mode
        if (currentMode == DataStructureType::NONE) {
            // Draw main menu
            window.clear(Config::MENU_BG_COLOR);
            window.draw(menuTitle);
            window.draw(menuSubtitle);
            window.draw(instructions);
            for (auto& btn : menuButtons) {
                btn.draw(window);
            }
            window.draw(footer);
            window.display();
        }
        else {
            // Run selected visualization mode
            switch (currentMode) {
                case DataStructureType::BST:
                    runBSTMode(window, font);
                    break;
                case DataStructureType::LINKED_LIST:
                    runLinkedListMode(window, font);
                    break;
                case DataStructureType::STACK:
                    runStackMode(window, font);
                    break;
                case DataStructureType::QUEUE:
                    runQueueMode(window, font);
                    break;
                default:
                    break;
            }
            currentMode = DataStructureType::NONE;  // Return to menu after mode exits
        }
    }
    
    return 0;
}

// ============================================================================
// EXPORT HELPER FUNCTION
// Captures the visualization area and saves to PNG
// ============================================================================
bool exportVisualizationToPNG(sf::RenderWindow& window, const std::string& filename,
                               float areaX, float areaY, float areaW, float areaH) {
    sf::Texture texture;
    texture.create(window.getSize().x, window.getSize().y);
    texture.update(window);
    
    sf::Image screenshot = texture.copyToImage();
    
    // Create a cropped image of just the visualization area
    sf::Image cropped;
    cropped.create(static_cast<unsigned int>(areaW), static_cast<unsigned int>(areaH));
    
    for (unsigned int x = 0; x < static_cast<unsigned int>(areaW); x++) {
        for (unsigned int y = 0; y < static_cast<unsigned int>(areaH); y++) {
            unsigned int srcX = static_cast<unsigned int>(areaX) + x;
            unsigned int srcY = static_cast<unsigned int>(areaY) + y;
            if (srcX < screenshot.getSize().x && srcY < screenshot.getSize().y) {
                cropped.setPixel(x, y, screenshot.getPixel(srcX, srcY));
            }
        }
    }
    
    return cropped.saveToFile(filename);
}

// ============================================================================
// BST MODE
// Binary Search Tree visualization with full animation system
// ============================================================================
void runBSTMode(sf::RenderWindow& window, sf::Font& font) {
    // Create BST and its visualizer
    BST bst;
    Visualizer visualizer(&bst, &font);
    
    // GUI Layout
    float panelX = Config::CONTROL_PANEL_PADDING;
    float controlWidth = Config::CONTROL_PANEL_WIDTH - 2 * Config::CONTROL_PANEL_PADDING;
    float buttonHeight = 32.0f;
    float spacing = 8.0f;
    float currentY = Config::CONTROL_PANEL_PADDING;
    
    // Title
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Binary Search Tree");
    titleText.setCharacterSize(Config::TITLE_FONT_SIZE);
    titleText.setFillColor(Config::TEXT_COLOR);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(panelX, currentY);
    currentY += 30;
    
    // Input label and field
    sf::Text inputLabel;
    inputLabel.setFont(font);
    inputLabel.setString("Enter value:");
    inputLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    inputLabel.setFillColor(Config::TEXT_SECONDARY);
    inputLabel.setPosition(panelX, currentY);
    currentY += 18;
    
    TextInput valueInput(panelX, currentY, controlWidth, 32, "Integer...", font, true);
    currentY += 40;
    
    // Operation buttons
    Button insertBtn(panelX, currentY, controlWidth, buttonHeight, "Insert", font);
    currentY += buttonHeight + spacing;
    
    Button deleteBtn(panelX, currentY, controlWidth, buttonHeight, "Delete", font);
    currentY += buttonHeight + spacing;
    
    Button searchBtn(panelX, currentY, controlWidth, buttonHeight, "Search", font);
    currentY += buttonHeight + spacing;
    
    Button clearBtn(panelX, currentY, controlWidth, buttonHeight, "Clear All", font);
    currentY += buttonHeight + spacing + 10;
    
    // Speed slider
    Slider speedSlider(panelX, currentY, controlWidth, 
                       Config::MIN_ANIMATION_SPEED, Config::MAX_ANIMATION_SPEED, 
                       Config::DEFAULT_ANIMATION_SPEED, "Animation Speed", font);
    currentY += 45;
    
    // Export button
    Button exportBtn(panelX, currentY, controlWidth, buttonHeight, "Export PNG", font);
    currentY += buttonHeight + spacing;
    
    // Back button
    Button backBtn(panelX, currentY, controlWidth, buttonHeight, "<< Back to Menu", font);
    currentY += buttonHeight + spacing + 10;
    
    // Traversal display
    sf::Text traversalLabel;
    traversalLabel.setFont(font);
    traversalLabel.setString("In-order traversal:");
    traversalLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    traversalLabel.setFillColor(Config::TEXT_SECONDARY);
    traversalLabel.setPosition(panelX, currentY);
    currentY += 16;
    
    sf::Text traversalText;
    traversalText.setFont(font);
    traversalText.setString("[ Empty ]");
    traversalText.setCharacterSize(10);
    traversalText.setFillColor(Config::TEXT_COLOR);
    traversalText.setPosition(panelX, currentY);
    
    // Message box for feedback
    MessageBox messageBox(panelX, Config::WINDOW_HEIGHT - 55, controlWidth, font);
    
    // Control panel background
    sf::RectangleShape controlPanel;
    controlPanel.setPosition(0, 0);
    controlPanel.setSize(sf::Vector2f(Config::CONTROL_PANEL_WIDTH, Config::WINDOW_HEIGHT));
    controlPanel.setFillColor(Config::CONTROL_PANEL_COLOR);
    
    sf::Clock clock;
    bool running = true;
    
    while (running && window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        visualizer.setSpeed(speedSlider.getValue());
        
        // Disable buttons during animation
        bool canInteract = !visualizer.isCurrentlyAnimating();
        insertBtn.setEnabled(canInteract);
        deleteBtn.setEnabled(canInteract);
        searchBtn.setEnabled(canInteract);
        clearBtn.setEnabled(canInteract);
        exportBtn.setEnabled(canInteract);
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                running = false;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                running = false;
            }
            
            valueInput.handleEvent(event, window);
            speedSlider.handleEvent(event, window);
            
            if (backBtn.handleEvent(event, window)) {
                running = false;
            }
            
            // INSERT operation
            if (insertBtn.handleEvent(event, window)) {
                int value;
                if (valueInput.isEmpty()) {
                    messageBox.show("Error: Please enter a value!", MessageBox::ERROR_MSG, 3.0f);
                }
                else if (!valueInput.getAsInt(value)) {
                    messageBox.show("Error: Invalid integer!", MessageBox::ERROR_MSG, 3.0f);
                }
                else {
                    std::vector<Node*> path;
                    bool success = bst.insert(value, path);
                    if (success) {
                        visualizer.animateInsert(path, path.empty() ? nullptr : path.back());
                        messageBox.show("Inserted: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    } else {
                        visualizer.animateDuplicateInsert(path);
                        messageBox.show("Error: " + std::to_string(value) + " already exists!", MessageBox::ERROR_MSG, 3.0f);
                    }
                    valueInput.clear();
                }
            }
            
            // DELETE operation
            if (deleteBtn.handleEvent(event, window)) {
                int value;
                if (valueInput.isEmpty()) {
                    messageBox.show("Error: Enter value to delete!", MessageBox::ERROR_MSG, 3.0f);
                }
                else if (!valueInput.getAsInt(value)) {
                    messageBox.show("Error: Invalid integer!", MessageBox::ERROR_MSG, 3.0f);
                }
                else {
                    std::vector<Node*> path;
                    Node* deletedNode = nullptr;
                    Node* successor = nullptr;
                    if (bst.remove(value, path, deletedNode, successor)) {
                        visualizer.animateDelete(path, deletedNode, successor);
                        messageBox.show("Deleted: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    } else {
                        visualizer.animateNotFound(path);
                        messageBox.show("Error: " + std::to_string(value) + " not found!", MessageBox::ERROR_MSG, 3.0f);
                    }
                    valueInput.clear();
                }
            }
            
            // SEARCH operation
            if (searchBtn.handleEvent(event, window)) {
                int value;
                if (valueInput.isEmpty()) {
                    messageBox.show("Error: Enter value to search!", MessageBox::ERROR_MSG, 3.0f);
                }
                else if (!valueInput.getAsInt(value)) {
                    messageBox.show("Error: Invalid integer!", MessageBox::ERROR_MSG, 3.0f);
                }
                else {
                    std::vector<Node*> path;
                    Node* result = bst.search(value, path);
                    visualizer.animateSearch(path, result != nullptr);
                    if (result) {
                        messageBox.show("Found: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    } else {
                        messageBox.show(std::to_string(value) + " not found.", MessageBox::INFO, 2.0f);
                    }
                    valueInput.clear();
                }
            }
            
            // CLEAR operation
            if (clearBtn.handleEvent(event, window)) {
                if (!bst.isEmpty()) {
                    visualizer.animateClear();
                    bst.clear();
                    messageBox.show("Tree cleared!", MessageBox::INFO, 2.0f);
                } else {
                    messageBox.show("Tree is already empty.", MessageBox::INFO, 2.0f);
                }
            }
            
            // EXPORT PNG
            if (exportBtn.handleEvent(event, window)) {
                if (bst.isEmpty()) {
                    messageBox.show("Cannot export empty tree!", MessageBox::ERROR_MSG, 2.0f);
                } else {
                    if (visualizer.exportToPNG("bst_export.png")) {
                        messageBox.show("Exported to bst_export.png", MessageBox::SUCCESS, 3.0f);
                    } else {
                        messageBox.show("Export failed!", MessageBox::ERROR_MSG, 3.0f);
                    }
                }
            }
        }
        
        // Update
        valueInput.update(deltaTime);
        visualizer.update(deltaTime);
        messageBox.update(deltaTime);
        traversalText.setString(visualizer.getInorderString());
        
        // Draw
        window.clear(Config::BACKGROUND_COLOR);
        window.draw(controlPanel);
        window.draw(titleText);
        window.draw(inputLabel);
        window.draw(traversalLabel);
        window.draw(traversalText);
        valueInput.draw(window);
        insertBtn.draw(window);
        deleteBtn.draw(window);
        searchBtn.draw(window);
        clearBtn.draw(window);
        speedSlider.draw(window);
        exportBtn.draw(window);
        backBtn.draw(window);
        visualizer.draw(window);
        messageBox.draw(window);
        window.display();
    }
}

// ============================================================================
// LINKED LIST MODE
// Singly Linked List visualization with animated traversal
// ============================================================================
void runLinkedListMode(sf::RenderWindow& window, sf::Font& font) {
    LinkedList list;
    
    // GUI Layout
    float panelX = Config::CONTROL_PANEL_PADDING;
    float controlWidth = Config::CONTROL_PANEL_WIDTH - 2 * Config::CONTROL_PANEL_PADDING;
    float buttonHeight = 32.0f;
    float spacing = 7.0f;
    float currentY = Config::CONTROL_PANEL_PADDING;
    
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Linked List");
    titleText.setCharacterSize(Config::TITLE_FONT_SIZE);
    titleText.setFillColor(Config::TEXT_COLOR);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(panelX, currentY);
    currentY += 30;
    
    sf::Text inputLabel;
    inputLabel.setFont(font);
    inputLabel.setString("Enter value:");
    inputLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    inputLabel.setFillColor(Config::TEXT_SECONDARY);
    inputLabel.setPosition(panelX, currentY);
    currentY += 18;
    
    TextInput valueInput(panelX, currentY, controlWidth, 32, "Integer...", font, true);
    currentY += 40;
    
    Button insertHeadBtn(panelX, currentY, controlWidth, buttonHeight, "Insert at Head", font);
    currentY += buttonHeight + spacing;
    
    Button insertTailBtn(panelX, currentY, controlWidth, buttonHeight, "Insert at Tail", font);
    currentY += buttonHeight + spacing;
    
    Button deleteBtn(panelX, currentY, controlWidth, buttonHeight, "Delete", font);
    currentY += buttonHeight + spacing;
    
    Button searchBtn(panelX, currentY, controlWidth, buttonHeight, "Search", font);
    currentY += buttonHeight + spacing;
    
    Button clearBtn(panelX, currentY, controlWidth, buttonHeight, "Clear All", font);
    currentY += buttonHeight + spacing + 8;
    
    // Speed slider (ADDED)
    Slider speedSlider(panelX, currentY, controlWidth, 
                       Config::MIN_ANIMATION_SPEED, Config::MAX_ANIMATION_SPEED, 
                       Config::DEFAULT_ANIMATION_SPEED, "Animation Speed", font);
    currentY += 45;
    
    // Export button (ADDED)
    Button exportBtn(panelX, currentY, controlWidth, buttonHeight, "Export PNG", font);
    currentY += buttonHeight + spacing;
    
    Button backBtn(panelX, currentY, controlWidth, buttonHeight, "<< Back to Menu", font);
    currentY += buttonHeight + spacing + 8;
    
    sf::Text contentLabel;
    contentLabel.setFont(font);
    contentLabel.setString("Contents:");
    contentLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    contentLabel.setFillColor(Config::TEXT_SECONDARY);
    contentLabel.setPosition(panelX, currentY);
    currentY += 16;
    
    sf::Text contentText;
    contentText.setFont(font);
    contentText.setString("[ Empty ]");
    contentText.setCharacterSize(10);
    contentText.setFillColor(Config::TEXT_COLOR);
    contentText.setPosition(panelX, currentY);
    
    MessageBox messageBox(panelX, Config::WINDOW_HEIGHT - 55, controlWidth, font);
    
    sf::RectangleShape controlPanel;
    controlPanel.setPosition(0, 0);
    controlPanel.setSize(sf::Vector2f(Config::CONTROL_PANEL_WIDTH, Config::WINDOW_HEIGHT));
    controlPanel.setFillColor(Config::CONTROL_PANEL_COLOR);
    
    // Animation state
    std::vector<ListNode*> highlightPath;
    int highlightIndex = -1;
    float highlightTimer = 0;
    float animSpeed = 1.0f;
    bool isAnimating = false;
    ListNode* foundNode = nullptr;
    
    sf::Clock clock;
    bool running = true;
    
    while (running && window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        animSpeed = speedSlider.getValue();
        
        // Update animation with speed control
        if (isAnimating && highlightIndex < (int)highlightPath.size()) {
            highlightTimer += deltaTime * animSpeed;
            if (highlightTimer >= 0.3f) {
                highlightTimer = 0;
                highlightIndex++;
                if (highlightIndex >= (int)highlightPath.size()) {
                    isAnimating = false;
                }
            }
        }
        
        bool canInteract = !isAnimating;
        insertHeadBtn.setEnabled(canInteract);
        insertTailBtn.setEnabled(canInteract);
        deleteBtn.setEnabled(canInteract);
        searchBtn.setEnabled(canInteract);
        clearBtn.setEnabled(canInteract);
        exportBtn.setEnabled(canInteract);
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                running = false;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                running = false;
            }
            
            valueInput.handleEvent(event, window);
            speedSlider.handleEvent(event, window);
            
            if (backBtn.handleEvent(event, window)) running = false;
            
            if (insertHeadBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    highlightPath.clear();
                    list.insertAtHead(value, highlightPath);
                    highlightIndex = 0;
                    highlightTimer = 0;
                    isAnimating = true;
                    foundNode = highlightPath.empty() ? nullptr : highlightPath.back();
                    messageBox.show("Inserted at head: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (insertTailBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    highlightPath.clear();
                    list.insertAtTail(value, highlightPath);
                    highlightIndex = 0;
                    highlightTimer = 0;
                    isAnimating = true;
                    foundNode = highlightPath.empty() ? nullptr : highlightPath.back();
                    messageBox.show("Inserted at tail: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (deleteBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    highlightPath.clear();
                    ListNode* deleted = nullptr;
                    if (list.remove(value, highlightPath, deleted)) {
                        highlightIndex = 0;
                        highlightTimer = 0;
                        isAnimating = true;
                        foundNode = deleted;
                        messageBox.show("Deleted: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                        if (deleted) delete deleted;
                    } else {
                        highlightIndex = 0;
                        isAnimating = true;
                        foundNode = nullptr;
                        messageBox.show("Error: Value not found!", MessageBox::ERROR_MSG, 2.0f);
                    }
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (searchBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    highlightPath.clear();
                    ListNode* result = list.search(value, highlightPath);
                    highlightIndex = 0;
                    highlightTimer = 0;
                    isAnimating = true;
                    foundNode = result;
                    if (result) {
                        messageBox.show("Found: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    } else {
                        messageBox.show("Value not found.", MessageBox::INFO, 2.0f);
                    }
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (clearBtn.handleEvent(event, window)) {
                list.clear();
                highlightPath.clear();
                isAnimating = false;
                messageBox.show("List cleared!", MessageBox::INFO, 2.0f);
            }
            
            // EXPORT PNG (ADDED)
            if (exportBtn.handleEvent(event, window) && canInteract) {
                if (list.isEmpty()) {
                    messageBox.show("Cannot export empty list!", MessageBox::ERROR_MSG, 2.0f);
                } else {
                    // Draw first, then export
                    window.display();
                    if (exportVisualizationToPNG(window, "linkedlist_export.png", 
                                                  Config::TREE_AREA_X - 10, Config::TREE_AREA_Y - 40,
                                                  Config::TREE_AREA_WIDTH + 20, Config::TREE_AREA_HEIGHT + 50)) {
                        messageBox.show("Exported to linkedlist_export.png", MessageBox::SUCCESS, 3.0f);
                    } else {
                        messageBox.show("Export failed!", MessageBox::ERROR_MSG, 3.0f);
                    }
                }
            }
        }
        
        valueInput.update(deltaTime);
        messageBox.update(deltaTime);
        contentText.setString(list.toString());
        
        // Drawing
        window.clear(Config::BACKGROUND_COLOR);
        window.draw(controlPanel);
        window.draw(titleText);
        window.draw(inputLabel);
        window.draw(contentLabel);
        window.draw(contentText);
        valueInput.draw(window);
        insertHeadBtn.draw(window);
        insertTailBtn.draw(window);
        deleteBtn.draw(window);
        searchBtn.draw(window);
        clearBtn.draw(window);
        speedSlider.draw(window);
        exportBtn.draw(window);
        backBtn.draw(window);
        
        // Draw visualization area
        sf::RectangleShape treeArea;
        treeArea.setPosition(Config::TREE_AREA_X - 10, Config::TREE_AREA_Y - 10);
        treeArea.setSize(sf::Vector2f(Config::TREE_AREA_WIDTH + 20, Config::TREE_AREA_HEIGHT + 20));
        treeArea.setFillColor(Config::TREE_AREA_COLOR);
        window.draw(treeArea);
        
        sf::Text areaTitle;
        areaTitle.setFont(font);
        areaTitle.setString("Linked List Visualization");
        areaTitle.setCharacterSize(Config::TITLE_FONT_SIZE);
        areaTitle.setFillColor(Config::TEXT_SECONDARY);
        areaTitle.setPosition(Config::TREE_AREA_X, Config::TREE_AREA_Y - 35);
        window.draw(areaTitle);
        
        // Draw nodes
        auto nodes = list.getAllNodes();
        float startX = Config::TREE_AREA_X + 50;
        float startY = Config::TREE_AREA_Y + Config::TREE_AREA_HEIGHT / 2;
        float nodeWidth = Config::LINEAR_NODE_WIDTH;
        float nodeHeight = Config::LINEAR_NODE_HEIGHT;
        float nodeSpacing = Config::LINEAR_NODE_SPACING + 30;
        
        for (size_t i = 0; i < nodes.size(); i++) {
            float x = startX + i * (nodeWidth + nodeSpacing);
            float y = startY;
            
            sf::Color fillColor = Config::LINKEDLIST_COLOR;
            sf::Color outlineColor = Config::LINKEDLIST_OUTLINE;
            
            if (isAnimating && (int)i <= highlightIndex) {
                if (nodes[i] == foundNode && (int)i == highlightIndex) {
                    fillColor = Config::NODE_FOUND_FILL;
                    outlineColor = Config::NODE_FOUND_OUTLINE;
                } else {
                    fillColor = Config::NODE_HIGHLIGHT_FILL;
                    outlineColor = Config::NODE_HIGHLIGHT_OUTLINE;
                }
            }
            
            sf::RectangleShape nodeBox;
            nodeBox.setPosition(x, y - nodeHeight/2);
            nodeBox.setSize(sf::Vector2f(nodeWidth, nodeHeight));
            nodeBox.setFillColor(fillColor);
            nodeBox.setOutlineColor(outlineColor);
            nodeBox.setOutlineThickness(3);
            window.draw(nodeBox);
            
            sf::Text valueText;
            valueText.setFont(font);
            valueText.setString(std::to_string(nodes[i]->value));
            valueText.setCharacterSize(Config::NODE_FONT_SIZE);
            valueText.setFillColor(Config::TEXT_COLOR);
            sf::FloatRect bounds = valueText.getLocalBounds();
            valueText.setOrigin(bounds.width/2, bounds.height/2);
            valueText.setPosition(x + nodeWidth/2, y - 5);
            window.draw(valueText);
            
            // Draw arrow
            if (i < nodes.size() - 1) {
                float arrowStartX = x + nodeWidth;
                float arrowEndX = x + nodeWidth + nodeSpacing;
                sf::Vertex line[] = {
                    sf::Vertex(sf::Vector2f(arrowStartX, y), Config::ARROW_COLOR),
                    sf::Vertex(sf::Vector2f(arrowEndX - 10, y), Config::ARROW_COLOR)
                };
                window.draw(line, 2, sf::Lines);
                
                sf::ConvexShape arrow;
                arrow.setPointCount(3);
                arrow.setPoint(0, sf::Vector2f(arrowEndX, y));
                arrow.setPoint(1, sf::Vector2f(arrowEndX - 12, y - 6));
                arrow.setPoint(2, sf::Vector2f(arrowEndX - 12, y + 6));
                arrow.setFillColor(Config::ARROW_COLOR);
                window.draw(arrow);
            }
        }
        
        if (!nodes.empty()) {
            sf::Text headLabel;
            headLabel.setFont(font);
            headLabel.setString("HEAD");
            headLabel.setCharacterSize(12);
            headLabel.setFillColor(Config::SUCCESS_COLOR);
            headLabel.setPosition(startX + nodeWidth/2 - 18, startY - nodeHeight/2 - 25);
            window.draw(headLabel);
            
            float lastX = startX + (nodes.size()-1) * (nodeWidth + nodeSpacing);
            sf::Text nullLabel;
            nullLabel.setFont(font);
            nullLabel.setString("-> NULL");
            nullLabel.setCharacterSize(14);
            nullLabel.setFillColor(Config::TEXT_SECONDARY);
            nullLabel.setPosition(lastX + nodeWidth + 10, startY - 8);
            window.draw(nullLabel);
        } else {
            sf::Text emptyText;
            emptyText.setFont(font);
            emptyText.setString("List is empty\nInsert values to visualize!");
            emptyText.setCharacterSize(16);
            emptyText.setFillColor(sf::Color(120, 120, 130));
            sf::FloatRect bounds = emptyText.getLocalBounds();
            emptyText.setOrigin(bounds.width / 2, bounds.height / 2);
            emptyText.setPosition(Config::TREE_AREA_X + Config::TREE_AREA_WIDTH / 2, startY);
            window.draw(emptyText);
        }
        
        messageBox.draw(window);
        window.display();
    }
}

// ============================================================================
// STACK MODE
// Stack (LIFO) visualization - vertical representation
// ============================================================================
void runStackMode(sf::RenderWindow& window, sf::Font& font) {
    Stack stack;
    
    float panelX = Config::CONTROL_PANEL_PADDING;
    float controlWidth = Config::CONTROL_PANEL_WIDTH - 2 * Config::CONTROL_PANEL_PADDING;
    float buttonHeight = 32.0f;
    float spacing = 7.0f;
    float currentY = Config::CONTROL_PANEL_PADDING;
    
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Stack (LIFO)");
    titleText.setCharacterSize(Config::TITLE_FONT_SIZE);
    titleText.setFillColor(Config::TEXT_COLOR);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(panelX, currentY);
    currentY += 30;
    
    sf::Text inputLabel;
    inputLabel.setFont(font);
    inputLabel.setString("Enter value:");
    inputLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    inputLabel.setFillColor(Config::TEXT_SECONDARY);
    inputLabel.setPosition(panelX, currentY);
    currentY += 18;
    
    TextInput valueInput(panelX, currentY, controlWidth, 32, "Integer...", font, true);
    currentY += 40;
    
    Button pushBtn(panelX, currentY, controlWidth, buttonHeight, "Push", font);
    currentY += buttonHeight + spacing;
    
    Button popBtn(panelX, currentY, controlWidth, buttonHeight, "Pop", font);
    currentY += buttonHeight + spacing;
    
    Button peekBtn(panelX, currentY, controlWidth, buttonHeight, "Peek (Top)", font);
    currentY += buttonHeight + spacing;
    
    Button searchBtn(panelX, currentY, controlWidth, buttonHeight, "Search", font);
    currentY += buttonHeight + spacing;
    
    Button clearBtn(panelX, currentY, controlWidth, buttonHeight, "Clear All", font);
    currentY += buttonHeight + spacing + 8;
    
    // Speed slider (ADDED)
    Slider speedSlider(panelX, currentY, controlWidth, 
                       Config::MIN_ANIMATION_SPEED, Config::MAX_ANIMATION_SPEED, 
                       Config::DEFAULT_ANIMATION_SPEED, "Animation Speed", font);
    currentY += 45;
    
    // Export button (ADDED)
    Button exportBtn(panelX, currentY, controlWidth, buttonHeight, "Export PNG", font);
    currentY += buttonHeight + spacing;
    
    Button backBtn(panelX, currentY, controlWidth, buttonHeight, "<< Back to Menu", font);
    currentY += buttonHeight + spacing + 8;
    
    sf::Text contentLabel;
    contentLabel.setFont(font);
    contentLabel.setString("Contents:");
    contentLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    contentLabel.setFillColor(Config::TEXT_SECONDARY);
    contentLabel.setPosition(panelX, currentY);
    currentY += 16;
    
    sf::Text contentText;
    contentText.setFont(font);
    contentText.setString("[ Empty ]");
    contentText.setCharacterSize(10);
    contentText.setFillColor(Config::TEXT_COLOR);
    contentText.setPosition(panelX, currentY);
    
    MessageBox messageBox(panelX, Config::WINDOW_HEIGHT - 55, controlWidth, font);
    
    sf::RectangleShape controlPanel;
    controlPanel.setPosition(0, 0);
    controlPanel.setSize(sf::Vector2f(Config::CONTROL_PANEL_WIDTH, Config::WINDOW_HEIGHT));
    controlPanel.setFillColor(Config::CONTROL_PANEL_COLOR);
    
    int highlightIndex = -1;
    float highlightTimer = 0;
    float animSpeed = 1.0f;
    bool isAnimating = false;
    
    sf::Clock clock;
    bool running = true;
    
    while (running && window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        animSpeed = speedSlider.getValue();
        
        if (isAnimating) {
            highlightTimer += deltaTime * animSpeed;
            if (highlightTimer >= 1.0f) {
                isAnimating = false;
                highlightIndex = -1;
            }
        }
        
        bool canInteract = !isAnimating;
        pushBtn.setEnabled(canInteract);
        popBtn.setEnabled(canInteract);
        peekBtn.setEnabled(canInteract);
        searchBtn.setEnabled(canInteract);
        clearBtn.setEnabled(canInteract);
        exportBtn.setEnabled(canInteract);
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                running = false;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                running = false;
            }
            
            valueInput.handleEvent(event, window);
            speedSlider.handleEvent(event, window);
            
            if (backBtn.handleEvent(event, window)) running = false;
            
            if (pushBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    stack.push(value);
                    highlightIndex = stack.getSize() - 1;
                    highlightTimer = 0;
                    isAnimating = true;
                    messageBox.show("Pushed: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (popBtn.handleEvent(event, window) && canInteract) {
                StackNode* popped = stack.pop();
                if (popped) {
                    messageBox.show("Popped: " + std::to_string(popped->value), MessageBox::SUCCESS, 2.0f);
                    delete popped;
                } else {
                    messageBox.show("Error: Stack is empty!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (peekBtn.handleEvent(event, window) && canInteract) {
                StackNode* top = stack.peek();
                if (top) {
                    highlightIndex = stack.getSize() - 1;
                    highlightTimer = 0;
                    isAnimating = true;
                    messageBox.show("Top element: " + std::to_string(top->value), MessageBox::INFO, 2.0f);
                } else {
                    messageBox.show("Error: Stack is empty!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (searchBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    std::vector<StackNode*> path;
                    StackNode* result = stack.search(value, path);
                    if (result) {
                        auto nodes = stack.getAllNodes();
                        for (size_t i = 0; i < nodes.size(); i++) {
                            if (nodes[i] == result) {
                                highlightIndex = static_cast<int>(i);
                                break;
                            }
                        }
                        highlightTimer = 0;
                        isAnimating = true;
                        messageBox.show("Found at position " + std::to_string(stack.getSize() - highlightIndex) + " from top", MessageBox::SUCCESS, 2.0f);
                    } else {
                        messageBox.show("Value not found.", MessageBox::INFO, 2.0f);
                    }
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (clearBtn.handleEvent(event, window)) {
                stack.clear();
                isAnimating = false;
                highlightIndex = -1;
                messageBox.show("Stack cleared!", MessageBox::INFO, 2.0f);
            }
            
            // EXPORT PNG (ADDED)
            if (exportBtn.handleEvent(event, window) && canInteract) {
                if (stack.isEmpty()) {
                    messageBox.show("Cannot export empty stack!", MessageBox::ERROR_MSG, 2.0f);
                } else {
                    window.display();
                    if (exportVisualizationToPNG(window, "stack_export.png", 
                                                  Config::TREE_AREA_X - 10, Config::TREE_AREA_Y - 40,
                                                  Config::TREE_AREA_WIDTH + 20, Config::TREE_AREA_HEIGHT + 50)) {
                        messageBox.show("Exported to stack_export.png", MessageBox::SUCCESS, 3.0f);
                    } else {
                        messageBox.show("Export failed!", MessageBox::ERROR_MSG, 3.0f);
                    }
                }
            }
        }
        
        valueInput.update(deltaTime);
        messageBox.update(deltaTime);
        contentText.setString(stack.toString());
        
        window.clear(Config::BACKGROUND_COLOR);
        window.draw(controlPanel);
        window.draw(titleText);
        window.draw(inputLabel);
        window.draw(contentLabel);
        window.draw(contentText);
        valueInput.draw(window);
        pushBtn.draw(window);
        popBtn.draw(window);
        peekBtn.draw(window);
        searchBtn.draw(window);
        clearBtn.draw(window);
        speedSlider.draw(window);
        exportBtn.draw(window);
        backBtn.draw(window);
        
        // Draw visualization area
        sf::RectangleShape treeArea;
        treeArea.setPosition(Config::TREE_AREA_X - 10, Config::TREE_AREA_Y - 10);
        treeArea.setSize(sf::Vector2f(Config::TREE_AREA_WIDTH + 20, Config::TREE_AREA_HEIGHT + 20));
        treeArea.setFillColor(Config::TREE_AREA_COLOR);
        window.draw(treeArea);
        
        sf::Text areaTitle;
        areaTitle.setFont(font);
        areaTitle.setString("Stack Visualization (LIFO - Last In First Out)");
        areaTitle.setCharacterSize(Config::TITLE_FONT_SIZE);
        areaTitle.setFillColor(Config::TEXT_SECONDARY);
        areaTitle.setPosition(Config::TREE_AREA_X, Config::TREE_AREA_Y - 35);
        window.draw(areaTitle);
        
        // Draw stack (vertical)
        auto nodes = stack.getAllNodes();
        float centerX = Config::TREE_AREA_X + Config::TREE_AREA_WIDTH / 2;
        float bottomY = Config::TREE_AREA_Y + Config::TREE_AREA_HEIGHT - 50;
        float nodeWidth = 120;
        float nodeHeight = 40;
        float nodeSpacing = 5;
        
        for (size_t i = 0; i < nodes.size(); i++) {
            float x = centerX - nodeWidth/2;
            float y = bottomY - i * (nodeHeight + nodeSpacing);
            
            sf::Color fillColor = Config::STACK_COLOR;
            sf::Color outlineColor = Config::STACK_OUTLINE;
            
            if (isAnimating && (int)i == highlightIndex) {
                fillColor = Config::NODE_FOUND_FILL;
                outlineColor = Config::NODE_FOUND_OUTLINE;
            }
            
            sf::RectangleShape nodeBox;
            nodeBox.setPosition(x, y - nodeHeight);
            nodeBox.setSize(sf::Vector2f(nodeWidth, nodeHeight));
            nodeBox.setFillColor(fillColor);
            nodeBox.setOutlineColor(outlineColor);
            nodeBox.setOutlineThickness(3);
            window.draw(nodeBox);
            
            sf::Text valueText;
            valueText.setFont(font);
            valueText.setString(std::to_string(nodes[i]->value));
            valueText.setCharacterSize(Config::NODE_FONT_SIZE);
            valueText.setFillColor(Config::TEXT_COLOR);
            sf::FloatRect bounds = valueText.getLocalBounds();
            valueText.setOrigin(bounds.width/2, bounds.height/2);
            valueText.setPosition(centerX, y - nodeHeight/2 - 3);
            window.draw(valueText);
        }
        
        if (!nodes.empty()) {
            float topY = bottomY - (nodes.size()-1) * (nodeHeight + nodeSpacing);
            sf::Text topLabel;
            topLabel.setFont(font);
            topLabel.setString("<-- TOP");
            topLabel.setCharacterSize(14);
            topLabel.setFillColor(Config::SUCCESS_COLOR);
            topLabel.setPosition(centerX + nodeWidth/2 + 15, topY - nodeHeight/2 - 10);
            window.draw(topLabel);
            
            sf::Text bottomLabel;
            bottomLabel.setFont(font);
            bottomLabel.setString("<-- BOTTOM");
            bottomLabel.setCharacterSize(12);
            bottomLabel.setFillColor(Config::TEXT_SECONDARY);
            bottomLabel.setPosition(centerX + nodeWidth/2 + 15, bottomY - nodeHeight/2 - 10);
            window.draw(bottomLabel);
        } else {
            sf::Text emptyText;
            emptyText.setFont(font);
            emptyText.setString("Stack is empty\nPush values to visualize!");
            emptyText.setCharacterSize(16);
            emptyText.setFillColor(sf::Color(120, 120, 130));
            sf::FloatRect bounds = emptyText.getLocalBounds();
            emptyText.setOrigin(bounds.width / 2, bounds.height / 2);
            emptyText.setPosition(centerX, Config::TREE_AREA_Y + Config::TREE_AREA_HEIGHT / 2);
            window.draw(emptyText);
        }
        
        messageBox.draw(window);
        window.display();
    }
}

// ============================================================================
// QUEUE MODE
// Queue (FIFO) visualization - horizontal representation
// ============================================================================
void runQueueMode(sf::RenderWindow& window, sf::Font& font) {
    Queue queue;
    
    float panelX = Config::CONTROL_PANEL_PADDING;
    float controlWidth = Config::CONTROL_PANEL_WIDTH - 2 * Config::CONTROL_PANEL_PADDING;
    float buttonHeight = 32.0f;
    float spacing = 7.0f;
    float currentY = Config::CONTROL_PANEL_PADDING;
    
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Queue (FIFO)");
    titleText.setCharacterSize(Config::TITLE_FONT_SIZE);
    titleText.setFillColor(Config::TEXT_COLOR);
    titleText.setStyle(sf::Text::Bold);
    titleText.setPosition(panelX, currentY);
    currentY += 30;
    
    sf::Text inputLabel;
    inputLabel.setFont(font);
    inputLabel.setString("Enter value:");
    inputLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    inputLabel.setFillColor(Config::TEXT_SECONDARY);
    inputLabel.setPosition(panelX, currentY);
    currentY += 18;
    
    TextInput valueInput(panelX, currentY, controlWidth, 32, "Integer...", font, true);
    currentY += 40;
    
    Button enqueueBtn(panelX, currentY, controlWidth, buttonHeight, "Enqueue (Add)", font);
    currentY += buttonHeight + spacing;
    
    Button dequeueBtn(panelX, currentY, controlWidth, buttonHeight, "Dequeue (Remove)", font);
    currentY += buttonHeight + spacing;
    
    Button peekBtn(panelX, currentY, controlWidth, buttonHeight, "Peek Front", font);
    currentY += buttonHeight + spacing;
    
    Button searchBtn(panelX, currentY, controlWidth, buttonHeight, "Search", font);
    currentY += buttonHeight + spacing;
    
    Button clearBtn(panelX, currentY, controlWidth, buttonHeight, "Clear All", font);
    currentY += buttonHeight + spacing + 8;
    
    // Speed slider (ADDED)
    Slider speedSlider(panelX, currentY, controlWidth, 
                       Config::MIN_ANIMATION_SPEED, Config::MAX_ANIMATION_SPEED, 
                       Config::DEFAULT_ANIMATION_SPEED, "Animation Speed", font);
    currentY += 45;
    
    // Export button (ADDED)
    Button exportBtn(panelX, currentY, controlWidth, buttonHeight, "Export PNG", font);
    currentY += buttonHeight + spacing;
    
    Button backBtn(panelX, currentY, controlWidth, buttonHeight, "<< Back to Menu", font);
    currentY += buttonHeight + spacing + 8;
    
    sf::Text contentLabel;
    contentLabel.setFont(font);
    contentLabel.setString("Contents:");
    contentLabel.setCharacterSize(Config::LABEL_FONT_SIZE);
    contentLabel.setFillColor(Config::TEXT_SECONDARY);
    contentLabel.setPosition(panelX, currentY);
    currentY += 16;
    
    sf::Text contentText;
    contentText.setFont(font);
    contentText.setString("[ Empty ]");
    contentText.setCharacterSize(10);
    contentText.setFillColor(Config::TEXT_COLOR);
    contentText.setPosition(panelX, currentY);
    
    MessageBox messageBox(panelX, Config::WINDOW_HEIGHT - 55, controlWidth, font);
    
    sf::RectangleShape controlPanel;
    controlPanel.setPosition(0, 0);
    controlPanel.setSize(sf::Vector2f(Config::CONTROL_PANEL_WIDTH, Config::WINDOW_HEIGHT));
    controlPanel.setFillColor(Config::CONTROL_PANEL_COLOR);
    
    int highlightIndex = -1;
    float highlightTimer = 0;
    float animSpeed = 1.0f;
    bool isAnimating = false;
    
    sf::Clock clock;
    bool running = true;
    
    while (running && window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        animSpeed = speedSlider.getValue();
        
        if (isAnimating) {
            highlightTimer += deltaTime * animSpeed;
            if (highlightTimer >= 1.0f) {
                isAnimating = false;
                highlightIndex = -1;
            }
        }
        
        bool canInteract = !isAnimating;
        enqueueBtn.setEnabled(canInteract);
        dequeueBtn.setEnabled(canInteract);
        peekBtn.setEnabled(canInteract);
        searchBtn.setEnabled(canInteract);
        clearBtn.setEnabled(canInteract);
        exportBtn.setEnabled(canInteract);
        
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                running = false;
            }
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                running = false;
            }
            
            valueInput.handleEvent(event, window);
            speedSlider.handleEvent(event, window);
            
            if (backBtn.handleEvent(event, window)) running = false;
            
            if (enqueueBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    queue.enqueue(value);
                    highlightIndex = queue.getSize() - 1;
                    highlightTimer = 0;
                    isAnimating = true;
                    messageBox.show("Enqueued: " + std::to_string(value), MessageBox::SUCCESS, 2.0f);
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (dequeueBtn.handleEvent(event, window) && canInteract) {
                QueueNode* dequeued = queue.dequeue();
                if (dequeued) {
                    messageBox.show("Dequeued: " + std::to_string(dequeued->value), MessageBox::SUCCESS, 2.0f);
                    delete dequeued;
                } else {
                    messageBox.show("Error: Queue is empty!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (peekBtn.handleEvent(event, window) && canInteract) {
                QueueNode* front = queue.peekFront();
                if (front) {
                    highlightIndex = 0;
                    highlightTimer = 0;
                    isAnimating = true;
                    messageBox.show("Front element: " + std::to_string(front->value), MessageBox::INFO, 2.0f);
                } else {
                    messageBox.show("Error: Queue is empty!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (searchBtn.handleEvent(event, window) && canInteract) {
                int value;
                if (!valueInput.isEmpty() && valueInput.getAsInt(value)) {
                    std::vector<QueueNode*> path;
                    QueueNode* result = queue.search(value, path);
                    if (result) {
                        auto nodes = queue.getAllNodes();
                        for (size_t i = 0; i < nodes.size(); i++) {
                            if (nodes[i] == result) {
                                highlightIndex = static_cast<int>(i);
                                break;
                            }
                        }
                        highlightTimer = 0;
                        isAnimating = true;
                        messageBox.show("Found at position " + std::to_string(highlightIndex + 1) + " from front", MessageBox::SUCCESS, 2.0f);
                    } else {
                        messageBox.show("Value not found.", MessageBox::INFO, 2.0f);
                    }
                    valueInput.clear();
                } else {
                    messageBox.show("Error: Enter a valid integer!", MessageBox::ERROR_MSG, 2.0f);
                }
            }
            
            if (clearBtn.handleEvent(event, window)) {
                queue.clear();
                isAnimating = false;
                highlightIndex = -1;
                messageBox.show("Queue cleared!", MessageBox::INFO, 2.0f);
            }
            
            // EXPORT PNG (ADDED)
            if (exportBtn.handleEvent(event, window) && canInteract) {
                if (queue.isEmpty()) {
                    messageBox.show("Cannot export empty queue!", MessageBox::ERROR_MSG, 2.0f);
                } else {
                    window.display();
                    if (exportVisualizationToPNG(window, "queue_export.png", 
                                                  Config::TREE_AREA_X - 10, Config::TREE_AREA_Y - 40,
                                                  Config::TREE_AREA_WIDTH + 20, Config::TREE_AREA_HEIGHT + 50)) {
                        messageBox.show("Exported to queue_export.png", MessageBox::SUCCESS, 3.0f);
                    } else {
                        messageBox.show("Export failed!", MessageBox::ERROR_MSG, 3.0f);
                    }
                }
            }
        }
        
        valueInput.update(deltaTime);
        messageBox.update(deltaTime);
        contentText.setString(queue.toString());
        
        window.clear(Config::BACKGROUND_COLOR);
        window.draw(controlPanel);
        window.draw(titleText);
        window.draw(inputLabel);
        window.draw(contentLabel);
        window.draw(contentText);
        valueInput.draw(window);
        enqueueBtn.draw(window);
        dequeueBtn.draw(window);
        peekBtn.draw(window);
        searchBtn.draw(window);
        clearBtn.draw(window);
        speedSlider.draw(window);
        exportBtn.draw(window);
        backBtn.draw(window);
        
        // Draw visualization area
        sf::RectangleShape treeArea;
        treeArea.setPosition(Config::TREE_AREA_X - 10, Config::TREE_AREA_Y - 10);
        treeArea.setSize(sf::Vector2f(Config::TREE_AREA_WIDTH + 20, Config::TREE_AREA_HEIGHT + 20));
        treeArea.setFillColor(Config::TREE_AREA_COLOR);
        window.draw(treeArea);
        
        sf::Text areaTitle;
        areaTitle.setFont(font);
        areaTitle.setString("Queue Visualization (FIFO - First In First Out)");
        areaTitle.setCharacterSize(Config::TITLE_FONT_SIZE);
        areaTitle.setFillColor(Config::TEXT_SECONDARY);
        areaTitle.setPosition(Config::TREE_AREA_X, Config::TREE_AREA_Y - 35);
        window.draw(areaTitle);
        
        // Draw queue (horizontal)
        auto nodes = queue.getAllNodes();
        float startX = Config::TREE_AREA_X + 80;
        float centerY = Config::TREE_AREA_Y + Config::TREE_AREA_HEIGHT / 2;
        float nodeWidth = 80;
        float nodeHeight = 50;
        float nodeSpacing = 20;
        
        for (size_t i = 0; i < nodes.size(); i++) {
            float x = startX + i * (nodeWidth + nodeSpacing);
            float y = centerY - nodeHeight/2;
            
            sf::Color fillColor = Config::QUEUE_COLOR;
            sf::Color outlineColor = Config::QUEUE_OUTLINE;
            
            if (isAnimating && (int)i == highlightIndex) {
                fillColor = Config::NODE_FOUND_FILL;
                outlineColor = Config::NODE_FOUND_OUTLINE;
            }
            
            sf::RectangleShape nodeBox;
            nodeBox.setPosition(x, y);
            nodeBox.setSize(sf::Vector2f(nodeWidth, nodeHeight));
            nodeBox.setFillColor(fillColor);
            nodeBox.setOutlineColor(outlineColor);
            nodeBox.setOutlineThickness(3);
            window.draw(nodeBox);
            
            sf::Text valueText;
            valueText.setFont(font);
            valueText.setString(std::to_string(nodes[i]->value));
            valueText.setCharacterSize(Config::NODE_FONT_SIZE);
            valueText.setFillColor(Config::TEXT_COLOR);
            sf::FloatRect bounds = valueText.getLocalBounds();
            valueText.setOrigin(bounds.width/2, bounds.height/2);
            valueText.setPosition(x + nodeWidth/2, centerY - 3);
            window.draw(valueText);
        }
        
        if (!nodes.empty()) {
            sf::Text frontLabel;
            frontLabel.setFont(font);
            frontLabel.setString("FRONT");
            frontLabel.setCharacterSize(13);
            frontLabel.setFillColor(Config::SUCCESS_COLOR);
            frontLabel.setPosition(startX + nodeWidth/2 - 22, centerY - nodeHeight/2 - 28);
            window.draw(frontLabel);
            
            float rearX = startX + (nodes.size()-1) * (nodeWidth + nodeSpacing);
            sf::Text rearLabel;
            rearLabel.setFont(font);
            rearLabel.setString("REAR");
            rearLabel.setCharacterSize(13);
            rearLabel.setFillColor(Config::ERROR_COLOR);
            rearLabel.setPosition(rearX + nodeWidth/2 - 17, centerY - nodeHeight/2 - 28);
            window.draw(rearLabel);
            
            // Direction arrows
            sf::Text dirLabel;
            dirLabel.setFont(font);
            dirLabel.setString("Dequeue <<                              >> Enqueue");
            dirLabel.setCharacterSize(12);
            dirLabel.setFillColor(Config::TEXT_SECONDARY);
            dirLabel.setPosition(startX - 30, centerY + nodeHeight/2 + 35);
            window.draw(dirLabel);
        } else {
            sf::Text emptyText;
            emptyText.setFont(font);
            emptyText.setString("Queue is empty\nEnqueue values to visualize!");
            emptyText.setCharacterSize(16);
            emptyText.setFillColor(sf::Color(120, 120, 130));
            sf::FloatRect bounds = emptyText.getLocalBounds();
            emptyText.setOrigin(bounds.width / 2, bounds.height / 2);
            emptyText.setPosition(Config::TREE_AREA_X + Config::TREE_AREA_WIDTH / 2, centerY);
            window.draw(emptyText);
        }
        
        messageBox.draw(window);
        window.display();
    }
}
