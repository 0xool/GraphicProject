#ifndef ANEALALGORITHM_H
#define ANEALALGORITHM_H

#include <vector>
#include "node.h"

class AnealAlgorithm
{
public:
    AnealAlgorithm();

    void SimulatedAnnealing();
    void SimulatedAnnealingForGraph(Node node);

    float badStepChance();

    std::vector<Node> getGraphAfterAlgorithm();

private:

    float T;
    float newCost;
    float oldCost;

    std::vector<Node> graph;






};

#endif // ANEALALGORITHM_H
