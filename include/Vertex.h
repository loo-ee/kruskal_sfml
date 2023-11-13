#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <limits>

#define INF std::numeric_limits<int>::max();

struct Vertex {
    std::string vertexName;
    std::vector<std::string> neighbors;
    sf::RectangleShape vertexRect;

    Vertex(std::string vertexName, std::vector<std::string> neighbors);
    void setPosition(sf::Vector2f pos);
    void setDimensions(sf::Vector2f dimensions);
    void setColor(sf::Color color);
};

