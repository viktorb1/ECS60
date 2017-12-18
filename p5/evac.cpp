#include <cstring>
#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"


Evac::Evac(City *city, int numCity, int numRoadss) : numCities(numCity)
{
  cities = new City[numCity];

  for (int i = 0; i < numCity; i++)
  {
    cities[i] = city[i];
    cities[i].roads = new Road[cities[i].roadCount];

    for (int j = 0; j < city[i].roadCount; j++)
      cities[i].roads[j] = city[i].roads[j];
  }
}


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  routeCount = 0;
  int time = 1;
  bool visited[numCities];

  memset(visited, 0, sizeof(visited));

  for (int i = 0; i < numEvacs; i++)
    cities[evacIDs[i]].evacuees = cities[evacIDs[i]].population;

  while (moreToEvacuate(numEvacs, evacIDs))
  {
    memset(visited, 0, sizeof(visited));

    for(int i = 0; i < numEvacs; i++)
      moveNeighbors(evacIDs[i], visited, evacRoutes, routeCount, time);

    time++;
  }
}


bool Evac::moreToEvacuate(int numEvacs, int* evacIDs)
{
  for (int i = 0; i < numEvacs; i++)
    if(cities[evacIDs[i]].evacuees > 0)
      return true;
  
  return false;
}


void Evac::moveNeighbors(int startID, bool* visited, EvacRoute *evacRoutes, int& routeCount, int time)
{
  visited[startID] = true;
 
  for(int j = 0; j < cities[startID].roadCount; j++)
  {
    int nextCity = cities[startID].roads[j].destinationCityID;

    if (!visited[nextCity] && cities[startID].evacuees > 0)
    {
      int minimum = min(cities[startID].roads[j].peoplePerHour, cities[nextCity].population - cities[nextCity].evacuees);

      if (cities[startID].evacuees < minimum)
        minimum = cities[startID].evacuees;

      if (minimum > 0)
      {
        evacRoutes[routeCount].roadID = cities[startID].roads[j].ID;
        evacRoutes[routeCount].time = time;
        evacRoutes[routeCount].numPeople = minimum;
        routeCount++;

        cities[nextCity].evacuees += minimum;
        cities[startID].evacuees -= minimum;
        visited[nextCity] = true;
      }

      if(cities[nextCity].evacuees == cities[nextCity].population)
        moveNeighbors(cities[startID].roads[j].destinationCityID, visited, evacRoutes, routeCount, time);
    }
  }
}