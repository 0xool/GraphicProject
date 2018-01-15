//
//  City.hpp
//  AI_Final_Project
//
//  Created by cyrus refahi on 6/5/17.
//  Copyright © 2017 cyrus refahi. All rights reserved.
//

#ifndef City_hpp
#define City_hpp

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


class City{
    int originalGraph[40][40];

public:
    
    int x;
    int y;
    
    City();
    City(int x , int y);
    int getX();
    
    int getY();
    double distanceTo(City city);
    
};

#endif /* City_hpp */
