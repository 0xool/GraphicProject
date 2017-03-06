#include "node.h"

Node::Node()
{

}

Node Node::getChildNodeByIndex(int index)
{

    return this->childNode[index];

}

void Node::addChildNode(Node node)
{

    this->childNode.push_back(node);

}

void Node::addParentNode(Node node)
{

    this->parentNode.push_back(node);

}

void Node::setTreeParentNode(Node* node)
{

    this->treeParentNode = node;

}

void Node::setX(int x)
{

    this->xAxis = x;

}

void Node::setY(int y)
{

    this->yAxis = y;

}

int Node::getX()
{

    return this->xAxis;

}

int Node::getY()
{

    return this->yAxis;

}

Node Node::getParentNodeByIndex(int index)
{

    return this->parentNode[index];

}

std::vector<Node> Node::getChildNodes()
{

    return this->childNode;

}

std::vector<Node> Node::getParentNodes()
{

    return this->parentNode;

}

Node* Node::getTreeParentNode()
{

    return this->treeParentNode;

}
