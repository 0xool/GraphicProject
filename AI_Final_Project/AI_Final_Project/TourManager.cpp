//
//  TourManager.cpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#include "TourManager.hpp"



void TourManager::addCity(City city)
{
    
    this->desitnationCities.push_back(city);
    
}
//=======================================================================================================
City TourManager::getCity(int index)
{
    
    return this->desitnationCities[index];
    
}
//=======================================================================================================
int TourManager::numberOfCities()
{
    return (int)this->desitnationCities.size();
}
//=======================================================================================================
