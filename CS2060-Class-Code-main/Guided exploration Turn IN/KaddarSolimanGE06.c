//workiung with structures

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 80


typedef struct organization {

    char organizationName[80];
    char bossName[80];
    char purpose[80];
    double goal;

}Organization;

void setUpOrganization(Organization*);
void displayOrganization(Organization);

int main(void)
{
    Organization org1;
    setUpOrganization(&org1);
    displayOrganization(org1);

    return 0;
}

void setUpOrganization(Organization* organization)
{
    printf("%s\n", "Enter a name for the organization:");
    fgets(organization->organizationName, SIZE, stdin);

    printf("%s\n", "Enter the purpose of the organization:");
    fgets(organization->purpose, SIZE, stdin);

    printf("%s\n", "Enter First and Last Name:");
    fgets(organization->bossName, SIZE, stdin);

    printf("%s\n", "Enter goal amount:");
    scanf("%lf", &(organization->goal));


}

void displayOrganization(Organization org) 
{
    printf("%s%s\n\n", "Organization Name: ", org.organizationName);

    printf("%s%s\n\n", "Organization Purpose: ", org.purpose);

    printf("%s%s\n\n", "Owner name: ", org.bossName);

    printf("%s%lf\n\n", "Organization goal: ", org.goal);
}