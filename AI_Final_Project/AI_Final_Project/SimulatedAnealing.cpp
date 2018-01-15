//
//  SimulatedAnealing.cpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#include "SimulatedAnealing.hpp"




SimulatedAnealing::SimulatedAnealing()
{
    
    
}
//=======================================================================================================
SimulatedAnealing::SimulatedAnealing(int solutionSize)
{
    
    TourManager TM = *new TourManager();
    
    City city = *new City(60, 200);
    TM.addCity(city);
    City city2 = *new City(180, 200);
    TM.addCity(city2);
    City city3 = *new City(80, 180);
    TM.addCity(city3);
    City city4 = *new City(140, 180);
    TM.addCity(city4);
    City city5 = *new City(20, 160);
    TM.addCity(city5);
    City city6 = *new City(100, 160);
    TM.addCity(city6);
    City city7 = *new City(200, 160);
    TM.addCity(city7);
    City city8 = *new City(140, 140);
    TM.addCity(city8);
    City city9 = *new City(40, 120);
    TM.addCity(city9);
    City city10 = *new City(100, 120);
    TM.addCity(city10);
    City city11 = *new City(180, 100);
    TM.addCity(city11);
    City city12 = *new City(60, 80);
    TM.addCity(city12);
    City city13 = *new City(120, 80);
    TM.addCity(city13);
    City city14 = *new City(180, 60);
    TM.addCity(city14);
    City city15 = *new City(20, 40);
    TM.addCity(city15);
    City city16 = *new City(100, 40);
    TM.addCity(city16);
    City city17 = *new City(200, 40);
    TM.addCity(city17);
    City city18 = *new City(20, 20);
    TM.addCity(city18);
    City city19 = *new City(60, 20);
    TM.addCity(city19);
    City city20 = *new City(160, 20);
    TM.addCity(city20);
    
    
    double temp = TEMPRETURE;
    double coolingRate = COOLINGRATE;
    
    Tour currentSolution = *new Tour(TM);
    currentSolution.generateIndividual();
    
    std::cout << "initial solution distance: " << currentSolution.getDistance();
    
    Tour best = *new Tour(currentSolution.getTour() , TM);
    
    //here is the core of the algorithm after each instance a new solution is generated and chosen based on the probability and temp in reconfigured.
    while (temp > 1) {
        //first we need a new solution
        //create a new neighbor
        Tour newSolution = *new Tour(currentSolution.getTour(), TM);
        
        
        int tourPos1 = (int)(newSolution.tourSize() * (((double) rand() / (RAND_MAX))));
        int tourPos2 = (int)(newSolution.tourSize() * (((double) rand() / (RAND_MAX))));
        
        City citySwap1 = newSolution.getCity(tourPos1);
        City citySwap2 = newSolution.getCity(tourPos2);
        
        newSolution.setCity(citySwap1, tourPos2);
        newSolution.setCity(citySwap2, tourPos1);
        
        int currentEnergy = currentSolution.getDistance();
        int neighborEnergy = newSolution.getDistance();
        
        //we check the current solution with the newly generated solution.
        double r = (( (double)rand() / (RAND_MAX)));
        if (acceptanceRate(currentEnergy, neighborEnergy, temp) > r)
        {
            
            currentSolution = *new Tour(newSolution.getTour(), TM);
            
        }
        
        //we chose whether to chose the bad solution or not!!
        if (currentSolution.getDistance() < best.getDistance()) {
            
            best = *new Tour(currentSolution.getTour(), TM);
        
        }
        
        //reconfigure temp for the next instance the less this get the less we chose the bad solution.
        //cool system
        temp *= 1 - coolingRate;
        
    }
    
    std::cout << " \n Final solution distance :" << best.getDistance() << "\n";
    
    for(int i=0; i<best.getTour().size(); ++i)
    {
        std::cout << "X:" << best.getTour()[i].getX() << ' ';
        std::cout << " \n Y:" << best.getTour()[i].getY() << "\n";
    }
    
}
//=======================================================================================================
float SimulatedAnealing::acceptanceRate(int cost , int currentCost , float temp)
{
    
    
    if (currentCost < cost){
        
        return 1;
    }else {
        
        return exp((cost - currentCost) / temp);
    }
    
    return exp((cost - currentCost) / temp);
    
}
//=======================================================================================================
float SimulatedAnealing::badStepChance()
{
    
    
    return 0;
    
}
//=======================================================================================================
int SimulatedAnealing::generateRandomSolution()
{
    
    
    
    
    return 0;
    
}
//=======================================================================================================



