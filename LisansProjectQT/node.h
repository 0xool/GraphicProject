#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>



class Node
{
public:
    Node();

    //models
    std::vector<Node> parentNode;
    Node* treeParentNode;
    std::vector<Node*> childNode;

    int xAxis;
    int yAxis;


    //setter
    void addParentNode(Node node);
    void addChildNode(Node* node);
    void setTreeParentNode(Node* node);

    void setX(int x);
    void setY(int y);
    void setName(std::string name);

    //-getter
    std::vector<Node*> getChildNodes();
    std::vector<Node> getParentNodes();
    Node* getTreeParentNode();

    Node* getChildNodeByIndex(int index);
    Node getParentNodeByIndex(int index);
    int getX();

    int getY();
    std::string getName();


private :

    std::string name;


};

#endif // NODE_H
