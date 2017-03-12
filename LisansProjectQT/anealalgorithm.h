#ifndef ANEALALGORITHM_H
#define ANEALALGORITHM_H

#include <vector>
#include "node.h"
#include "projectmanager.h"

class AnealAlgorithm
{
public:
    AnealAlgorithm();

    void SimulatedAnnealing();
    void SimulatedAnnealingForGraph(Node node);

    std::vector<Node> edgeDetection();
    float badStepChance();


    std::vector<Node> getGraphAfterAlgorithm();

private:

    float T;
    float newCost;
    float oldCost;

    std::vector<Node> graph;
    ProjectManager* pm;

};

#endif // ANEALALGORITHM_H
