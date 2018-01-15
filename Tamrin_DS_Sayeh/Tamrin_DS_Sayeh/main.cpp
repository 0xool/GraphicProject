#include <iostream>
#include <math.h>
#include "Tamrin_2.hpp"

void question (int input[] , int size) {
    
    int minMultiply = 1000000000;
    int counter = 0;
    
    if  (size < (10 ^ 5) && size > 3 ){
        
        for (int i = 0 ; i < size; i++){
            for (int j = i+1 ; j < size; j++){
                for (int k = j + 1 ; k < size; k++){
                    
                    int mul = input[i] * input[j] * input[k];
                    if (mul < minMultiply){
                        
                        counter = 1;
                        minMultiply = mul;
                        
                    }else if (mul == minMultiply){
                        counter++;
                    }
                }
            }
        }
        
    }else{
        
        std::printf("size does not have a correct value.");
        
    }
    
    std::printf("Counter : %d \n" , counter);
    std::printf("mul : %d \n" , minMultiply);

}


int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    int a [6] = {1, 3, 3, 1, 3 , 2};
    question(a, 6);

    return 0;




}


