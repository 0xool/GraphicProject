//
//  TourManager.hpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#ifndef TourManager_hpp
#define TourManager_hpp

#include <stdio.h>
#include <vector>
#include "City.hpp"


class TourManager {
    
    
public:
    
    std::vector<City> desitnationCities = *new std::vector<City>();
    void addCity(City city);
    City getCity(int index);
    
    int numberOfCities();
    
    
};




#endif /* TourManager_hpp */
