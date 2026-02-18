// File: Visualizer.cpp
// Description: Implementation of the tree visualizer.
// Handles layout calculation, drawing, and animation playback.

#include "Visualizer.h"
#include <cmath>
#include <sstream>
#include <algorithm>

// ============================================================================
// CONSTRUCTOR
// ============================================================================

Visualizer::Visualizer(BST* bstPtr, sf::Font* fontPtr)
    : bst(bstPtr), font(fontPtr), 
      stepTimer(0), speedFactor(1.0f), isAnimating(false),
      treeAreaX(Config::TREE_AREA_X), treeAreaY(Config::TREE_AREA_Y),
      treeAreaWidth(Config::TREE_AREA_WIDTH), treeAreaHeight(Config::TREE_AREA_HEIGHT)
{
    // Initialize with empty current step
    currentStep = AnimationStep(AnimationStep::PAUSE, -1, 0);
}

// ============================================================================
// LAYOUT CALCULATION
// ============================================================================
// The layout algorithm works as follows:
// 1. Start at root, place it at center-top
// 2. For each node, recursively calculate the width of left and right subtrees
// 3. Position children based on subtree widths to avoid overlap
// ============================================================================

void Visualizer::calculateLayout() {
    if (bst->isEmpty()) {
        nodeVisuals.clear();
        edges.clear();
        return;
    }
    
    // Calculate tree height for spacing
    int height = bst->getHeight();
    
    // Calculate horizontal space needed
    // More levels = tighter horizontal spacing
    float horizontalSpace = treeAreaWidth / (std::pow(2, std::min(height, 5)));
    horizontalSpace = std::max(horizontalSpace, Config::MIN_HORIZONTAL_SPACING * 2);
    
    // Start layout from root
    // Root is centered at top of tree area
    float startX = treeAreaX + treeAreaWidth / 2;
    float startY = treeAreaY + Config::NODE_RADIUS + 20;
    
    calculateSubtreeLayout(bst->getRoot(), startX, startY, treeAreaWidth / 4);
    
    // Build edge list
    buildEdgeList();
}

float Visualizer::calculateSubtreeLayout(Node* node, float x, float y, float horizontalSpace) {
    if (node == nullptr) return 0;
    
    // Set or update this node's visual state
    NodeVisual& visual = nodeVisuals[node->id];
    visual.nodeId = node->id;
    visual.value = node->value;
    visual.targetX = x;
    visual.targetY = y;
    
    // If this is a new node (just added), start at target position
    if (visual.x == 0 && visual.y == 0) {
        visual.x = x;
        visual.y = y;
    }
    
    // Calculate vertical position for children
    float childY = y + Config::VERTICAL_SPACING;
    
    // Reduce horizontal space for next level
    float childHSpace = horizontalSpace / 2;
    childHSpace = std::max(childHSpace, Config::MIN_HORIZONTAL_SPACING);
    
    // Layout left child
    if (node->left) {
        float leftX = x - horizontalSpace;
        calculateSubtreeLayout(node->left, leftX, childY, childHSpace);
    }
    
    // Layout right child
    if (node->right) {
        float rightX = x + horizontalSpace;
        calculateSubtreeLayout(node->right, rightX, childY, childHSpace);
    }
    
    return horizontalSpace * 2;  // Return width of this subtree
}

void Visualizer::buildEdgeList() {
    edges.clear();
    buildEdgeListHelper(bst->getRoot());
}

void Visualizer::buildEdgeListHelper(Node* node) {
    if (node == nullptr) return;
    
    if (node->left) {
        edges.push_back(EdgeVisual(node->id, node->left->id));
        buildEdgeListHelper(node->left);
    }
    
    if (node->right) {
        edges.push_back(EdgeVisual(node->id, node->right->id));
        buildEdgeListHelper(node->right);
    }
}

void Visualizer::syncVisualState() {
    // Get all nodes from BST
    std::vector<Node*> allNodes = bst->getAllNodes();
    
    // Create a set of current node IDs
    std::unordered_map<int, bool> currentIds;
    for (Node* n : allNodes) {
        currentIds[n->id] = true;
    }
    
    // Remove visuals for nodes that no longer exist
    std::vector<int> toRemove;
    for (auto& pair : nodeVisuals) {
        if (currentIds.find(pair.first) == currentIds.end()) {
            toRemove.push_back(pair.first);
        }
    }
    for (int id : toRemove) {
        nodeVisuals.erase(id);
    }
    
    // Calculate new layout
    calculateLayout();
}

// ============================================================================
// UPDATE (ANIMATION PROCESSING)
// ============================================================================

void Visualizer::update(float deltaTime) {
    // Update node positions (smooth movement)
    updateNodePositions(deltaTime);
    
    // Process current animation
    if (isAnimating) {
        processAnimationStep(deltaTime);
    }
}

void Visualizer::updateNodePositions(float deltaTime) {
    // Smoothly move nodes towards their target positions
    float moveSpeed = 10.0f * speedFactor;
    
    for (auto& pair : nodeVisuals) {
        NodeVisual& visual = pair.second;
        
        // Lerp towards target
        float dx = visual.targetX - visual.x;
        float dy = visual.targetY - visual.y;
        
        if (std::abs(dx) > 0.5f || std::abs(dy) > 0.5f) {
            visual.x += dx * moveSpeed * deltaTime;
            visual.y += dy * moveSpeed * deltaTime;
        } else {
            visual.x = visual.targetX;
            visual.y = visual.targetY;
        }
    }
}

void Visualizer::processAnimationStep(float deltaTime) {
    // Adjust time based on speed
    float adjustedDelta = deltaTime * speedFactor;
    stepTimer += adjustedDelta;
    
    // Calculate progress (0 to 1)
    float progress = currentStep.duration > 0 ? stepTimer / currentStep.duration : 1.0f;
    
    // Process based on step type
    switch (currentStep.type) {
        case AnimationStep::HIGHLIGHT_NODE:
            // Already set when step started
            break;
            
        case AnimationStep::HIGHLIGHT_EDGE:
            // Already set when step started
            break;
            
        case AnimationStep::COLOR_CHANGE:
            // Already applied when step started
            break;
            
        case AnimationStep::FADE_IN:
            if (nodeVisuals.count(currentStep.nodeId)) {
                nodeVisuals[currentStep.nodeId].alpha = progress * 255;
            }
            break;
            
        case AnimationStep::FADE_OUT:
            if (nodeVisuals.count(currentStep.nodeId)) {
                nodeVisuals[currentStep.nodeId].alpha = (1.0f - progress) * 255;
            }
            break;
            
        case AnimationStep::FLASH_NODE:
            // Flash effect: bright -> normal
            if (nodeVisuals.count(currentStep.nodeId)) {
                float flash = std::sin(progress * 3.14159f * 4) * 0.5f + 0.5f;
                sf::Color baseColor = Config::NODE_HIGHLIGHT_FILL;
                nodeVisuals[currentStep.nodeId].fillColor = sf::Color(
                    static_cast<sf::Uint8>(baseColor.r * (0.5f + flash * 0.5f)),
                    static_cast<sf::Uint8>(baseColor.g * (0.5f + flash * 0.5f)),
                    static_cast<sf::Uint8>(baseColor.b * (0.5f + flash * 0.5f))
                );
            }
            break;
            
        case AnimationStep::MOVE_NODES:
        case AnimationStep::PAUSE:
        case AnimationStep::RESET_COLORS:
            // These are handled elsewhere or are just time delays
            break;
    }
    
    // Check if step is complete
    if (stepTimer >= currentStep.duration) {
        // Finalize step
        switch (currentStep.type) {
            case AnimationStep::FADE_OUT:
                // Remove the visual after fade out
                nodeVisuals.erase(currentStep.nodeId);
                break;
                
            case AnimationStep::RESET_COLORS:
                // Reset all nodes to default colors
                for (auto& pair : nodeVisuals) {
                    pair.second.fillColor = Config::NODE_DEFAULT_FILL;
                    pair.second.outlineColor = Config::NODE_DEFAULT_OUTLINE;
                    pair.second.isHighlighted = false;
                }
                // Reset edges
                for (auto& edge : edges) {
                    edge.isHighlighted = false;
                }
                break;
                
            default:
                break;
        }
        
        // Move to next step
        startNextStep();
    }
}

void Visualizer::startNextStep() {
    if (animationQueue.empty()) {
        isAnimating = false;
        return;
    }
    
    currentStep = animationQueue.front();
    animationQueue.pop();
    stepTimer = 0;
    isAnimating = true;
    
    // Apply initial state for this step
    switch (currentStep.type) {
        case AnimationStep::HIGHLIGHT_NODE:
            if (nodeVisuals.count(currentStep.nodeId)) {
                nodeVisuals[currentStep.nodeId].fillColor = Config::NODE_HIGHLIGHT_FILL;
                nodeVisuals[currentStep.nodeId].outlineColor = Config::NODE_HIGHLIGHT_OUTLINE;
                nodeVisuals[currentStep.nodeId].isHighlighted = true;
            }
            break;
            
        case AnimationStep::HIGHLIGHT_EDGE:
            // Highlight edge from nodeId2 to nodeId
            for (auto& edge : edges) {
                if (edge.fromNodeId == currentStep.nodeId2 && 
                    edge.toNodeId == currentStep.nodeId) {
                    edge.isHighlighted = true;
                    break;
                }
            }
            break;
            
        case AnimationStep::COLOR_CHANGE:
            if (nodeVisuals.count(currentStep.nodeId)) {
                nodeVisuals[currentStep.nodeId].fillColor = currentStep.color;
            }
            break;
            
        case AnimationStep::FADE_IN:
            if (nodeVisuals.count(currentStep.nodeId)) {
                nodeVisuals[currentStep.nodeId].alpha = 0;
                nodeVisuals[currentStep.nodeId].fillColor = Config::NODE_NEW_FILL;
                nodeVisuals[currentStep.nodeId].outlineColor = Config::NODE_NEW_OUTLINE;
            }
            break;
            
        case AnimationStep::MOVE_NODES:
            // Recalculate layout - positions will be updated in updateNodePositions
            syncVisualState();
            break;
            
        default:
            break;
    }
}

// ============================================================================
// DRAWING
// ============================================================================

void Visualizer::draw(sf::RenderWindow& window) {
    // Draw tree area background
    sf::RectangleShape treeBackground;
    treeBackground.setPosition(treeAreaX - 10, treeAreaY - 10);
    treeBackground.setSize(sf::Vector2f(treeAreaWidth + 20, treeAreaHeight + 20));
    treeBackground.setFillColor(Config::TREE_AREA_COLOR);
    treeBackground.setOutlineThickness(1);
    treeBackground.setOutlineColor(sf::Color(60, 60, 70));
    window.draw(treeBackground);
    
    // Draw "Tree View" label
    sf::Text treeLabel;
    treeLabel.setFont(*font);
    treeLabel.setString("Binary Search Tree");
    treeLabel.setCharacterSize(Config::TITLE_FONT_SIZE);
    treeLabel.setFillColor(Config::TEXT_SECONDARY);
    treeLabel.setPosition(treeAreaX, treeAreaY - 35);
    window.draw(treeLabel);
    
    // Draw edges first (so they appear behind nodes)
    for (const auto& edge : edges) {
        if (nodeVisuals.count(edge.fromNodeId) && nodeVisuals.count(edge.toNodeId)) {
            const NodeVisual& from = nodeVisuals.at(edge.fromNodeId);
            const NodeVisual& to = nodeVisuals.at(edge.toNodeId);
            
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(from.x, from.y + Config::NODE_RADIUS), 
                          edge.isHighlighted ? Config::EDGE_HIGHLIGHT_COLOR : Config::EDGE_COLOR),
                sf::Vertex(sf::Vector2f(to.x, to.y - Config::NODE_RADIUS), 
                          edge.isHighlighted ? Config::EDGE_HIGHLIGHT_COLOR : Config::EDGE_COLOR)
            };
            
            // Draw thicker line if highlighted
            if (edge.isHighlighted) {
                // Draw multiple lines for thickness effect
                for (int i = -1; i <= 1; i++) {
                    sf::Vertex thickLine[] = {
                        sf::Vertex(sf::Vector2f(from.x + i, from.y + Config::NODE_RADIUS), 
                                  Config::EDGE_HIGHLIGHT_COLOR),
                        sf::Vertex(sf::Vector2f(to.x + i, to.y - Config::NODE_RADIUS), 
                                  Config::EDGE_HIGHLIGHT_COLOR)
                    };
                    window.draw(thickLine, 2, sf::Lines);
                }
            } else {
                window.draw(line, 2, sf::Lines);
            }
        }
    }
    
    // Draw nodes
    for (const auto& pair : nodeVisuals) {
        const NodeVisual& visual = pair.second;
        
        // Skip if fully transparent
        if (visual.alpha <= 0) continue;
        
        // Create circle shape for node
        sf::CircleShape circle(Config::NODE_RADIUS);
        circle.setOrigin(Config::NODE_RADIUS, Config::NODE_RADIUS);
        circle.setPosition(visual.x, visual.y);
        
        // Apply colors with alpha
        sf::Color fillColor = visual.fillColor;
        fillColor.a = static_cast<sf::Uint8>(visual.alpha);
        circle.setFillColor(fillColor);
        
        sf::Color outlineColor = visual.outlineColor;
        outlineColor.a = static_cast<sf::Uint8>(visual.alpha);
        circle.setOutlineColor(outlineColor);
        circle.setOutlineThickness(Config::NODE_OUTLINE_THICKNESS);
        
        window.draw(circle);
        
        // Draw value text
        sf::Text valueText;
        valueText.setFont(*font);
        valueText.setString(std::to_string(visual.value));
        valueText.setCharacterSize(Config::NODE_FONT_SIZE);
        
        sf::Color textColor = Config::TEXT_COLOR;
        textColor.a = static_cast<sf::Uint8>(visual.alpha);
        valueText.setFillColor(textColor);
        
        // Center text on node
        sf::FloatRect textBounds = valueText.getLocalBounds();
        valueText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                           textBounds.top + textBounds.height / 2.0f);
        valueText.setPosition(visual.x, visual.y);
        
        window.draw(valueText);
    }
    
    // Draw empty tree message if needed
    if (bst->isEmpty() && !isAnimating) {
        sf::Text emptyText;
        emptyText.setFont(*font);
        emptyText.setString("Tree is empty\nInsert values to visualize!");
        emptyText.setCharacterSize(16);
        emptyText.setFillColor(sf::Color(120, 120, 130));
        
        sf::FloatRect bounds = emptyText.getLocalBounds();
        emptyText.setOrigin(bounds.width / 2, bounds.height / 2);
        emptyText.setPosition(treeAreaX + treeAreaWidth / 2, 
                             treeAreaY + treeAreaHeight / 2);
        window.draw(emptyText);
    }
}

// ============================================================================
// ANIMATION CONTROL
// ============================================================================

void Visualizer::setSpeed(float speed) {
    speedFactor = std::max(Config::MIN_ANIMATION_SPEED, 
                          std::min(speed, Config::MAX_ANIMATION_SPEED));
}

bool Visualizer::isCurrentlyAnimating() const {
    return isAnimating;
}

void Visualizer::clearAnimations() {
    while (!animationQueue.empty()) {
        animationQueue.pop();
    }
    isAnimating = false;
    
    // Reset all visual states
    for (auto& pair : nodeVisuals) {
        pair.second.fillColor = Config::NODE_DEFAULT_FILL;
        pair.second.outlineColor = Config::NODE_DEFAULT_OUTLINE;
        pair.second.isHighlighted = false;
        pair.second.alpha = 255;
    }
    for (auto& edge : edges) {
        edge.isHighlighted = false;
    }
}

// ============================================================================
// ANIMATION SEQUENCES
// ============================================================================

void Visualizer::animateInsert(const std::vector<Node*>& path, Node* newNode) {
    clearAnimations();
    
    float stepDuration = Config::DEFAULT_ANIMATION_DURATION;
    
    // First sync the visual state to include the new node
    syncVisualState();
    
    // Highlight path from root to insertion point
    for (size_t i = 0; i < path.size(); i++) {
        // Highlight current node
        animationQueue.push(AnimationStep(
            AnimationStep::HIGHLIGHT_NODE, 
            path[i]->id, 
            stepDuration * 0.7f
        ));
        
        // Highlight edge to next node
        if (i > 0) {
            AnimationStep edgeStep(AnimationStep::HIGHLIGHT_EDGE, path[i]->id, 
                                  stepDuration * 0.3f);
            edgeStep.nodeId2 = path[i-1]->id;
            animationQueue.push(edgeStep);
        }
    }
    
    // Reset colors briefly
    animationQueue.push(AnimationStep(AnimationStep::RESET_COLORS, -1, stepDuration * 0.2f));
    
    // Fade in the new node with special color
    if (newNode) {
        animationQueue.push(AnimationStep(
            AnimationStep::FADE_IN, 
            newNode->id, 
            stepDuration
        ));
    }
    
    // Final reset
    animationQueue.push(AnimationStep(AnimationStep::PAUSE, -1, stepDuration * 0.5f));
    animationQueue.push(AnimationStep(AnimationStep::RESET_COLORS, -1, 0.1f));
    
    startNextStep();
}

void Visualizer::animateDuplicateInsert(const std::vector<Node*>& path) {
    clearAnimations();
    
    float stepDuration = Config::DEFAULT_ANIMATION_DURATION;
    
    // Highlight path to existing node
    for (size_t i = 0; i < path.size(); i++) {
        animationQueue.push(AnimationStep(
            AnimationStep::HIGHLIGHT_NODE, 
            path[i]->id, 
            stepDuration * 0.5f
        ));
    }
    
    // Flash the last node (the duplicate)
    if (!path.empty()) {
        animationQueue.push(AnimationStep(
            AnimationStep::FLASH_NODE, 
            path.back()->id, 
            stepDuration * 1.5f
        ));
    }
    
    // Reset
    animationQueue.push(AnimationStep(AnimationStep::RESET_COLORS, -1, 0.1f));
    
    startNextStep();
}

void Visualizer::animateDelete(const std::vector<Node*>& path, Node* deletedNode, Node* successor) {
    clearAnimations();
    
    float stepDuration = Config::DEFAULT_ANIMATION_DURATION;
    
    // Highlight search path
    for (size_t i = 0; i < path.size(); i++) {
        animationQueue.push(AnimationStep(
            AnimationStep::HIGHLIGHT_NODE, 
            path[i]->id, 
            stepDuration * 0.5f
        ));
    }
    
    // Highlight the node to delete in red
    if (deletedNode) {
        animationQueue.push(AnimationStep(
            AnimationStep::COLOR_CHANGE, 
            deletedNode->id, 
            stepDuration,
            Config::NODE_DELETE_FILL
        ));
        
        // If there's a successor, highlight it
        if (successor && successor != deletedNode) {
            animationQueue.push(AnimationStep(
                AnimationStep::COLOR_CHANGE, 
                successor->id, 
                stepDuration,
                Config::NODE_FOUND_FILL
            ));
        }
        
        // Fade out the deleted node (or successor if it was moved)
        animationQueue.push(AnimationStep(AnimationStep::PAUSE, -1, stepDuration * 0.3f));
    }
    
    // Move nodes to new positions
    animationQueue.push(AnimationStep(AnimationStep::MOVE_NODES, -1, stepDuration));
    
    // Reset colors
    animationQueue.push(AnimationStep(AnimationStep::RESET_COLORS, -1, 0.1f));
    
    startNextStep();
}

void Visualizer::animateNotFound(const std::vector<Node*>& path) {
    clearAnimations();
    
    float stepDuration = Config::DEFAULT_ANIMATION_DURATION;
    
    // Highlight the search path
    for (size_t i = 0; i < path.size(); i++) {
        animationQueue.push(AnimationStep(
            AnimationStep::HIGHLIGHT_NODE, 
            path[i]->id, 
            stepDuration * 0.5f
        ));
    }
    
    // Pause to show "not found"
    animationQueue.push(AnimationStep(AnimationStep::PAUSE, -1, stepDuration));
    
    // Reset
    animationQueue.push(AnimationStep(AnimationStep::RESET_COLORS, -1, 0.1f));
    
    startNextStep();
}

void Visualizer::animateSearch(const std::vector<Node*>& path, bool found) {
    clearAnimations();
    
    float stepDuration = Config::DEFAULT_ANIMATION_DURATION;
    
    // Highlight each node in the search path
    for (size_t i = 0; i < path.size(); i++) {
        animationQueue.push(AnimationStep(
            AnimationStep::HIGHLIGHT_NODE, 
            path[i]->id, 
            stepDuration * 0.6f
        ));
        
        // Highlight edge to next node
        if (i > 0) {
            AnimationStep edgeStep(AnimationStep::HIGHLIGHT_EDGE, path[i]->id, 
                                  stepDuration * 0.3f);
            edgeStep.nodeId2 = path[i-1]->id;
            animationQueue.push(edgeStep);
        }
    }
    
    // If found, show green; otherwise stay yellow briefly
    if (found && !path.empty()) {
        animationQueue.push(AnimationStep(
            AnimationStep::COLOR_CHANGE, 
            path.back()->id, 
            stepDuration * 1.5f,
            Config::NODE_FOUND_FILL
        ));
    } else {
        animationQueue.push(AnimationStep(AnimationStep::PAUSE, -1, stepDuration * 0.5f));
    }
    
    // Reset
    animationQueue.push(AnimationStep(AnimationStep::RESET_COLORS, -1, 0.1f));
    
    startNextStep();
}

void Visualizer::animateClear() {
    clearAnimations();
    
    // Get all nodes and fade them out one by one
    std::vector<Node*> nodes = bst->getAllNodes();
    
    float stepDuration = 0.15f;
    
    for (Node* node : nodes) {
        animationQueue.push(AnimationStep(
            AnimationStep::FADE_OUT, 
            node->id, 
            stepDuration
        ));
    }
    
    startNextStep();
}

// ============================================================================
// LAYOUT AND REFRESH
// ============================================================================

void Visualizer::refresh() {
    syncVisualState();
}

void Visualizer::setTreeArea(float x, float y, float width, float height) {
    treeAreaX = x;
    treeAreaY = y;
    treeAreaWidth = width;
    treeAreaHeight = height;
    calculateLayout();
}

// ============================================================================
// EXPORT TO PNG
// ============================================================================

bool Visualizer::exportToPNG(const std::string& filename) {
    // Create a render texture the size of the tree area
    sf::RenderTexture renderTexture;
    
    // Add some padding
    float padding = 50.0f;
    unsigned int width = static_cast<unsigned int>(treeAreaWidth + padding * 2);
    unsigned int height = static_cast<unsigned int>(treeAreaHeight + padding * 2);
    
    if (!renderTexture.create(width, height)) {
        return false;
    }
    
    // Clear with background color
    renderTexture.clear(Config::TREE_AREA_COLOR);
    
    // Temporarily adjust node positions for export
    float offsetX = padding - treeAreaX + Config::NODE_RADIUS;
    float offsetY = padding - treeAreaY + Config::NODE_RADIUS;
    
    // Draw edges
    for (const auto& edge : edges) {
        if (nodeVisuals.count(edge.fromNodeId) && nodeVisuals.count(edge.toNodeId)) {
            const NodeVisual& from = nodeVisuals.at(edge.fromNodeId);
            const NodeVisual& to = nodeVisuals.at(edge.toNodeId);
            
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(from.x + offsetX, from.y + offsetY + Config::NODE_RADIUS), 
                          Config::EDGE_COLOR),
                sf::Vertex(sf::Vector2f(to.x + offsetX, to.y + offsetY - Config::NODE_RADIUS), 
                          Config::EDGE_COLOR)
            };
            renderTexture.draw(line, 2, sf::Lines);
        }
    }
    
    // Draw nodes
    for (const auto& pair : nodeVisuals) {
        const NodeVisual& visual = pair.second;
        
        sf::CircleShape circle(Config::NODE_RADIUS);
        circle.setOrigin(Config::NODE_RADIUS, Config::NODE_RADIUS);
        circle.setPosition(visual.x + offsetX, visual.y + offsetY);
        circle.setFillColor(Config::NODE_DEFAULT_FILL);
        circle.setOutlineColor(Config::NODE_DEFAULT_OUTLINE);
        circle.setOutlineThickness(Config::NODE_OUTLINE_THICKNESS);
        renderTexture.draw(circle);
        
        sf::Text valueText;
        valueText.setFont(*font);
        valueText.setString(std::to_string(visual.value));
        valueText.setCharacterSize(Config::NODE_FONT_SIZE);
        valueText.setFillColor(Config::TEXT_COLOR);
        
        sf::FloatRect textBounds = valueText.getLocalBounds();
        valueText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                           textBounds.top + textBounds.height / 2.0f);
        valueText.setPosition(visual.x + offsetX, visual.y + offsetY);
        renderTexture.draw(valueText);
    }
    
    // Finalize and save
    renderTexture.display();
    return renderTexture.getTexture().copyToImage().saveToFile(filename);
}

// ============================================================================
// UTILITY
// ============================================================================

std::string Visualizer::getInorderString() {
    std::vector<int> values = bst->inorderTraversal();
    
    if (values.empty()) {
        return "[ Empty ]";
    }
    
    std::ostringstream ss;
    ss << "[ ";
    for (size_t i = 0; i < values.size(); i++) {
        ss << values[i];
        if (i < values.size() - 1) {
            ss << ", ";
        }
    }
    ss << " ]";
    
    return ss.str();
}

