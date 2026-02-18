// File: Visualizer.h
// Description: Handles all visual aspects of the BST.
// Responsibilities:
// - Calculate node positions (tree layout algorithm)
// - Draw nodes, edges, and labels
// - Manage animation queue and playback
// - Export tree to PNG

#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <unordered_map>
#include <functional>
#include "BST.h"
#include "Config.h"

// ============================================================================
// ANIMATION STEP STRUCTURE
// ============================================================================
// Represents a single step in an animation sequence.
// The visualizer processes these steps one by one.
// ============================================================================
struct AnimationStep {
    // Types of animation steps
    enum Type {
        HIGHLIGHT_NODE,     // Highlight a node during traversal
        HIGHLIGHT_EDGE,     // Highlight an edge during traversal
        COLOR_CHANGE,       // Change node color (found, delete, new)
        FADE_IN,            // New node appears
        FADE_OUT,           // Node disappears (deleted)
        MOVE_NODES,         // All nodes move to new positions
        PAUSE,              // Just wait
        RESET_COLORS,       // Reset all nodes to default colors
        FLASH_NODE          // Quick flash effect (for errors like duplicate)
    };
    
    Type type;
    int nodeId;             // Which node this step affects (-1 for all/none)
    int nodeId2;            // Second node for edges (parent node for edge highlight)
    sf::Color color;        // Color for color change steps
    float duration;         // How long this step takes
    
    // Default constructor
    AnimationStep() 
        : type(PAUSE), nodeId(-1), nodeId2(-1), color(sf::Color::White), duration(0) {}
    
    // Constructor for convenience
    AnimationStep(Type t, int id = -1, float dur = 0.3f, 
                  sf::Color c = sf::Color::White, int id2 = -1)
        : type(t), nodeId(id), nodeId2(id2), color(c), duration(dur) {}
};

// ============================================================================
// NODE VISUAL STATE
// ============================================================================
// Stores the visual state of each node for rendering.
// This is separate from the BST Node to keep visualization concerns separate.
// ============================================================================
struct NodeVisual {
    int nodeId;
    int value;
    float x, y;             // Current position
    float targetX, targetY; // Target position for animation
    sf::Color fillColor;
    sf::Color outlineColor;
    float alpha;            // For fade in/out (0-255)
    bool isHighlighted;
    
    NodeVisual() : nodeId(-1), value(0), x(0), y(0), targetX(0), targetY(0),
                   fillColor(Config::NODE_DEFAULT_FILL),
                   outlineColor(Config::NODE_DEFAULT_OUTLINE),
                   alpha(255), isHighlighted(false) {}
};

// ============================================================================
// EDGE VISUAL STATE
// ============================================================================
struct EdgeVisual {
    int fromNodeId;
    int toNodeId;
    bool isHighlighted;
    
    EdgeVisual(int from, int to) 
        : fromNodeId(from), toNodeId(to), isHighlighted(false) {}
};

// ============================================================================
// VISUALIZER CLASS
// ============================================================================
class Visualizer {
private:
    BST* bst;                                   // Pointer to the BST
    sf::Font* font;                             // Font for node labels
    
    // Visual state tracking
    std::unordered_map<int, NodeVisual> nodeVisuals;  // Node ID -> visual state
    std::vector<EdgeVisual> edges;                     // All edges
    
    // Animation system
    std::queue<AnimationStep> animationQueue;
    AnimationStep currentStep;
    float stepTimer;                            // Time elapsed in current step
    float speedFactor;                          // Multiplier for animation speed
    bool isAnimating;                           // Is an animation in progress?
    
    // Layout parameters
    float treeAreaX, treeAreaY;                 // Top-left of tree drawing area
    float treeAreaWidth, treeAreaHeight;        // Size of drawing area
    
    // ========================================================================
    // PRIVATE HELPER METHODS
    // ========================================================================
    
    // Calculate positions for all nodes in the tree
    // Uses a recursive algorithm to assign x,y based on tree structure
    void calculateLayout();
    
    // Recursive helper for layout calculation
    // Returns the width of the subtree rooted at 'node'
    float calculateSubtreeLayout(Node* node, float x, float y, float horizontalSpace);
    
    // Build the edge list from current tree structure
    void buildEdgeList();
    void buildEdgeListHelper(Node* node);
    
    // Sync visual state with BST state
    void syncVisualState();
    
    // Process a single animation step
    void processAnimationStep(float deltaTime);
    
    // Start the next animation step from the queue
    void startNextStep();
    
    // Smoothly interpolate node positions
    void updateNodePositions(float deltaTime);

public:
    // ========================================================================
    // CONSTRUCTOR
    // ========================================================================
    Visualizer(BST* bstPtr, sf::Font* fontPtr);
    
    // ========================================================================
    // MAIN UPDATE AND DRAW
    // ========================================================================
    
    // Update animations each frame
    // deltaTime: seconds since last frame
    void update(float deltaTime);
    
    // Draw the tree to the window
    void draw(sf::RenderWindow& window);
    
    // ========================================================================
    // ANIMATION CONTROL
    // ========================================================================
    
    // Set animation speed (1.0 = normal)
    void setSpeed(float speed);
    
    // Check if currently animating
    bool isCurrentlyAnimating() const;
    
    // Clear all pending animations
    void clearAnimations();
    
    // ========================================================================
    // ANIMATION SEQUENCES
    // ========================================================================
    // These methods create animation sequences for BST operations
    
    // Animate insertion: show path taken, then new node appearing
    void animateInsert(const std::vector<Node*>& path, Node* newNode);
    
    // Animate failed insert (duplicate): flash the existing node
    void animateDuplicateInsert(const std::vector<Node*>& path);
    
    // Animate deletion: show path, highlight node, show removal
    void animateDelete(const std::vector<Node*>& path, Node* deletedNode, Node* successor);
    
    // Animate failed delete (not found): show search path
    void animateNotFound(const std::vector<Node*>& path);
    
    // Animate search: highlight each node in path, then result
    void animateSearch(const std::vector<Node*>& path, bool found);
    
    // Animate clearing the tree
    void animateClear();
    
    // ========================================================================
    // LAYOUT AND REFRESH
    // ========================================================================
    
    // Force recalculation of layout
    void refresh();
    
    // Set the drawing area bounds
    void setTreeArea(float x, float y, float width, float height);
    
    // ========================================================================
    // EXPORT
    // ========================================================================
    
    // Export current tree view to PNG file
    bool exportToPNG(const std::string& filename);
    
    // Get the in-order traversal as a string (for display)
    std::string getInorderString();
};

#endif // VISUALIZER_H

