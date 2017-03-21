#ifndef ANEALALGORITHM_H
#define ANEALALGORITHM_H

#include <vector>
#include "mesh.h"
#include "node.h"
#include "projectmanager.h"

class AnealAlgorithm
{
public:
    AnealAlgorithm(float xMin , float yMin , float xMax , float yMax , Mesh mesh);
    AnealAlgorithm();

    void SimulatedAnnealing();
    void SimulatedAnnealingForGraph(Node node);
    void setManualTempreture(float tempreture);

    std::vector<Node> edgeDetection();
    float badStepChance();

    std::vector<Node> getGraphAfterAlgorithm();

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

};

#endif // ANEALALGORITHM_H
