//
//  Tour.hpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#ifndef Tour_hpp
#define Tour_hpp

#include <stdio.h>
#include <vector>
#include "City.hpp"

#include "TourManager.hpp"
#include <algorithm>
#include <cstdlib>

#include <random>

class Tour {
    
    
public:
    
    std::vector<City> tour = *new std::vector<City>();
    Tour(TourManager tourManager);
    Tour(std::vector<City> tour , TourManager tourManager);
    
    std::vector<City> getTour();
    void generateIndividual();
    void setCity(City city , int tourPosotion);
    
    int getDistance();
    int tourSize();
    TourManager tm;
    
    void setTourManager(TourManager tourManager);
    TourManager getTourManager();
    
    City getCity(int tourPosition);
    
    
private:
    
    int distance = 0;
    
    
};


#endif /* Tour_hpp */
