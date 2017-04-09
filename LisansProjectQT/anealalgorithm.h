#ifndef ANEALALGORITHM_H
#define ANEALALGORITHM_H

#include <vector>
#include "mesh.h"
#include "node.h"

#include "projectmanager.h"
#include <stdlib.h>


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

};

#endif // ANEALALGORITHM_H
