#include "include/GraphManager.h"

GraphManager::~GraphManager() {
    this->clearVertices();
    this->clearEdges();
}

void GraphManager::addVertex(std::string vertexName, 
    std::vector<std::string> neighbors, float posX, float posY) {

    const int vertexWidth = 20;
    const int vertexHeight = 20;
    Vertex* newVertex = new Vertex(vertexName, neighbors);

    newVertex->setPosition(sf::Vector2f(posX, posY));
    newVertex->setDimensions(sf::Vector2f(vertexWidth, vertexHeight));

    this->vertices[vertexName] = newVertex;
}

void GraphManager::addEdge(std::string src, std::string dst, float weight) {
    Edge* newEdge = new Edge(src, dst, weight);

    std::unordered_map<std::string, Vertex* >::iterator vertex1 = 
        this->vertices.find(src);

    std::unordered_map<std::string, Vertex* >::iterator vertex2 = 
        this->vertices.find(dst);

    if (vertex1 != this->vertices.end()) 
        this->vertices[src]->neighbors.push_back(dst);
    else 
        this->addVertex(src, {dst}, 100, 500);

    if (vertex2 != this->vertices.end()) 
        this->vertices[dst]->neighbors.push_back(src);
    else
        this->addVertex(dst, {src}, 150, 500);

    this->edges.push_back(newEdge);
}

void GraphManager::clearVertices() {
    for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
        delete (*it).second;
    }

    this->vertices.clear();
}

void GraphManager::clearEdges() {
    for (auto it = this->edges.begin(); it != this->edges.end(); it++) {
        delete (*it);
    }

    this->edges.clear();
}

void GraphManager::renderText(std::string label, sf::Vector2f pos, 
    sf::Font& font, int size, sf::Color color, sf::RenderWindow& window) {

    sf::Text rectText;
    rectText.setPosition(pos);
    rectText.setFont(font);
    rectText.setString(label);
    rectText.setCharacterSize(size);
    rectText.setFillColor(color);

    window.draw(rectText);
}

void GraphManager::sortEdges() {
    std::sort(this->edges.begin(), this->edges.end(), [](const Edge* a, const Edge* b) {
        return a->weight < b->weight;
        });
}

void GraphManager::makeSets() {
    for (std::unordered_map<std::string, Vertex* >::iterator it = 
        this->vertices.begin(); it != this->vertices.end(); it++) {

        Vertex* currentVertex = (*it).second;

        this->parentSubGraphs[currentVertex] = currentVertex;
        this->subGraphsRanks[currentVertex] = 0;
    }
}

void GraphManager::unionSet(Vertex* vertex0, Vertex* vertex1) {
    Vertex* root0 = this->findParent(vertex0);
    Vertex* root1 = this->findParent(vertex1);

    if (root0 == root1) 
        return;

    if (this->subGraphsRanks[root0] < this->subGraphsRanks[root1])
        this->parentSubGraphs[root0] = root1;
    else if (this->subGraphsRanks[root0] > this->subGraphsRanks[root1])
        this->parentSubGraphs[root1] = root0;
    else {
        this->parentSubGraphs[root1] = root0;
        this->subGraphsRanks[root0] += 1;
    }
}

std::unordered_map<std::string, Vertex* >* GraphManager::getVerticesMap() {
    return &this->vertices;
}

std::unordered_map<Vertex*, Vertex* >* GraphManager::getParentSubGraphs() {
    return &this->parentSubGraphs;
}

std::unordered_map<Vertex*, int>* GraphManager::getSubGraphsRanks() {
    return &this->subGraphsRanks;
}

std::vector<Edge* >* GraphManager::getEdges() {
    return &this->edges;
}

Edge* GraphManager::getOneEdge(std::string src, std::string dst) {
    std::vector<Edge*>::iterator foundEdge = std::find_if(this->edges.begin(), 
        this->edges.end(), [&src, &dst](Edge* currentEdge) {

        return (currentEdge->src == src && currentEdge->dst == dst) || 
                (currentEdge->src == dst && currentEdge->dst == src);
        });

    if (foundEdge != this->edges.end())
        return (*foundEdge);

    return nullptr;
}

Vertex* GraphManager::findParent(Vertex* targetVertex) {
    if (this->parentSubGraphs[targetVertex] != targetVertex)
        this->parentSubGraphs[targetVertex] = this->findParent(this->parentSubGraphs[targetVertex]);

    return this->parentSubGraphs[targetVertex];
}


