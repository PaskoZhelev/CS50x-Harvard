/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int lowBound = 0;
    int upBound = n - 1;
    int currentLength = n;
    int middle = 0;
    while(currentLength >= 1) {
        middle = (upBound - lowBound)/2;
        if(middle == 0){
            middle = lowBound;
        }
        
        if(values[middle] == value){
            return true;
        } else if (values[middle] < value) {
            lowBound = middle + 1;
            currentLength = (upBound - lowBound) + 1;
        } else {
            upBound = middle - 1;
            currentLength = (upBound - lowBound) + 1;
        }
    }
    
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int countArr[65537];
    for(int a=0; a < 65537; a++)
        countArr[a] = 0;
    
    for(int i = 0; i < n; i++) {
        int number = values[i];
        countArr[number] += 1;
    }
    int currentIndex = 0;
    for(int j = 0; j < 65537; j++){
        switch(countArr[j]) {
            case 0:
               break;
            case 1:
               values[currentIndex] = j;
                currentIndex++;
                break;
            default:
               for(int k = 0; k < countArr[j]; k++) {
                    values[currentIndex] = j;
                    currentIndex++;
                } 
                break;
        }
    }
    
}
