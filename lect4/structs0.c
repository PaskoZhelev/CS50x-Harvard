#include <cs50.h>
#include <stdio.h>
#include <string.h>

#include "structs.h"

#define STUDENTS 3

int main(void)
{
    student students[STUDENTS];
    
    for(int i = 0; i < STUDENTS; i++)
    {
        printf("Name: ");
        students[i].name = get_string();
        
        printf("Dorm: ");
        students[i].dorm = get_string();
        
    }
    
    for(int j = 0; j < STUDENTS; j++)
    {
        printf("%s is in %s\n", students[j].name, students[j].dorm);
    }
}