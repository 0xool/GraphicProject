//
//  Tour.cpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#include "Tour.hpp"




int myrandom (int i) { return std::rand()%i;}

Tour::Tour(TourManager tourManager)
{
    
    setTourManager(tourManager);
    
    for (int i = 0; i < this->tm.numberOfCities(); i++) {
        
        this->tour.push_back(City());
        
    }
    
}
//=====================================================================================================================================
Tour::Tour(std::vector<City> tour , TourManager tourManager)
{

    setTourManager(tourManager);
    
    for (int i = 0; i < this->tm.numberOfCities(); i++) {
        
        this->tour = tour;
        
    }
    
}
//=====================================================================================================================================
City Tour::getCity(int tourPosition)
{

    return this->tour[tourPosition];
    
}
//=====================================================================================================================================
std::vector<City> Tour::getTour()
{
 
    return this->tour;
    
}
//=====================================================================================================================================
void Tour::generateIndividual()
{
 
    for (int cityIndex = 0; cityIndex < this->tm.numberOfCities(); cityIndex++) {
        
        setCity(this->tm.getCity(cityIndex), cityIndex);
        
    }
    
    //collection.shuffle(tour);
    
    std::mt19937 g(static_cast<uint32_t>(time(0)));
   // std::shuffle(std::begin(tour), std::end(tour), g);
    
}
//=====================================================================================================================================
void Tour::setTourManager(TourManager tourManager)
{
    
    this->tm = tourManager;
    
}
//=====================================================================================================================================
TourManager Tour::getTourManager()
{

    return this->tm;

}
//=====================================================================================================================================
void Tour::setCity(City city, int tourPosotion)
{

    this->tour[tourPosotion] = city;
    
    distance = 0;
    
}
//=====================================================================================================================================
int Tour::getDistance()
{

    if (distance == 0)
    {
        int tourDistance = 0;
        for (int cityIndex = 0; cityIndex < this->tour.size(); cityIndex++) {
            
            City fromCity = getCity(cityIndex);
            City destirnationCity;
            
            if (cityIndex + 1 < this->tour.size())
            {
                
                destirnationCity = getCity(cityIndex + 1);
                
            }else{
                
                destirnationCity = getCity(0);
                
            }
            
            tourDistance += fromCity.distanceTo(destirnationCity);
            
        }
        
        distance = tourDistance;
        
    }
    
    return distance;
    
}
//=====================================================================================================================================
int Tour::tourSize()
{

    return (int)this->tour.size();
    
}
//=====================================================================================================================================









