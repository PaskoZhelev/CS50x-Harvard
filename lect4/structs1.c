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
    
    FILE *file = fopen("students.csv", "w");
    
    if(file != NULL) {
        for(int j = 0; j < STUDENTS; j++)
        {
            fprintf(file, "%s,%s\n", students[j].name, students[j].dorm);
        }
        fclose(file);
    }
}