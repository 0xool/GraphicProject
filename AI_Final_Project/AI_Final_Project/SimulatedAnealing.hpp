//
//  SimulatedAnealing.hpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#ifndef SimulatedAnealing_hpp
#define SimulatedAnealing_hpp

#include <stdio.h>
#include <iostream>
#include <math.h>

#include "Tour.hpp"
#include "City.hpp"

#define TEMPRETURE  10000
#define COOLINGRATE  0.003

class SimulatedAnealing {
    
    
public:
    
    //myImplementation
    int T = 1000;
    SimulatedAnealing();
    SimulatedAnealing(int solutionSize);
    
    float badStepChance();
    int generateRandomSolution();
    float acceptanceRate(int cost , int currentCost , float temp);
    
    //suggestedImplementation
    void simulatedAnealing();
    
    
};







#endif /* SimulatedAnealing_hpp */
