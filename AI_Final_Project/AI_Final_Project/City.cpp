//
//  City.cpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#include "City.hpp"


City::City()
{
    
    this->x = (int)rand() % 200;
    this->y = (int)rand() % 200;
    
}

//=======================================================================================================
City::City(int x , int y)
{
    
    this->x = x;
    this->y = y;
    
}

//=======================================================================================================
int City::getX()
{
    
    return this->x;
    
}

//=======================================================================================================
int City::getY()
{
    
    return this->y;
    
}

//=======================================================================================================
double City::distanceTo(City city)
{
    
    int xDistance = abs(this->x - city.getX());
    int yDistance = abs(this->y - city.getY());
    
    double distance = sqrt( (xDistance * xDistance) + (yDistance * yDistance));
    return distance;
    
}
