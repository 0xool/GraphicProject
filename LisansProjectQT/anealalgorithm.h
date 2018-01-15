#ifndef ANEALALGORITHM_H
#define ANEALALGORITHM_H

#include <vector>
#include "mesh.h"
#include "node.h"

#include "projectmanager.h"
#include <stdlib.h>
#include <math.h>


class AnealAlgorithm
{
public:
    AnealAlgorithm(float xMin , float yMin , float xMax , float yMax , Mesh mesh , OuterShape outerShape);
    AnealAlgorithm();

    void SimulatedAnnealing();
    void SimulatedAnnealingForGraph();
    void setManualTempreture(float tempreture);
    
    void costEvaluation(Mesh mesh);
    void inContactWithInnerShape();
    void setMeshState(std::vector<Mesh> meshes);

    std::vector<Node> edgeDetection();
    float badStepChance();
    std::vector<Node> getGraphAfterAlgorithm();
    
    Mesh generateRandomeSolution();
    std::vector<Mesh> getMeshStates();
    int calculateIntersectionCost(InnerShape outerShape , Node* destinationNode , Node* startNode);

    bool hasIntersectionWithOuterShape(OuterShape outerShape , Node* destinationNode , Node* startNode);
    Mesh  simulatedAnnealingAlgorithm();
    float acceptanceRate(int cost , int newCost , float temp);

    int inOrOut (OuterShape outerShape, Node *destinationNode);
    

private:

    float T;
    float delta = 0;
    float temp_delta;

    float newCost;
    float oldCost;
    float xMin;

    float xMax;
    float yMin;
    float yMax;

    std::vector<Node> graph;
    ProjectManager* pm;
    Mesh mesh;
    
    Mesh solutionMesh;
    OuterShape outerShape;
    std::vector<Mesh> meshStates;

    InnerShape innerShape;
    bool haveAnswer = false;

};

#endif // ANEALALGORITHM_H
