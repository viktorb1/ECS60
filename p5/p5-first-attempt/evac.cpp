#include <cstring>
#include <cstdlib>
#include "evac.h"
#include "EvacRunner.h"
#include "QueueAr.h"
#include <assert.h>

using namespace std;

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
} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  routeCount = 0;
  int time = 1;

  for (int i = 0; i < numEvacs; i++)
  {
    Road pathIDs[numCities];

    int pathIDsSize = findFurthestCity(evacIDs, evacIDs[i], numEvacs, pathIDs);

    while (cities[evacIDs[i]].population > 0)
    {
      int maxFlow = calculateMaxFlow(pathIDs, pathIDsSize, evacIDs[i]);

      if (maxFlow == 0) // find another path since maxFlow is zero
      {
        pathIDsSize = findFurthestCity(evacIDs, evacIDs[i], numEvacs, pathIDs);
        maxFlow = calculateMaxFlow(pathIDs, pathIDsSize, evacIDs[i]);
      }

      int current_city = evacIDs[i];

      cities[current_city].population -= maxFlow;
      cities[current_city].evacuees += maxFlow;

      for(int j = 0; j < pathIDsSize; j++)
      {
        cities[ pathIDs[j].destinationCityID ].evacuees += maxFlow;
        cities[ current_city ].evacuees -= maxFlow;
        evacRoutes[routeCount].roadID = pathIDs[j].ID;
        evacRoutes[routeCount].time = time;
        evacRoutes[routeCount].numPeople = maxFlow;
        current_city = pathIDs[j].destinationCityID;
        routeCount++;
      }

      time++;
    }
  }
} // evacuate


int Evac::findFurthestCity(int *evacIDs, int startID, int numEvacs, Road *pathIDs)
{
  
  int maxDepth = 0, furthestCityID = 0;
  Queue<int> Q(numCities);
  
  memset(isVisited, 0, sizeof(isVisited));
  memset(depth, 0, sizeof(depth));
  memset(city_from, 0, sizeof(city_from));
  memset(road_from, 0, sizeof(road_from));

  Q.enqueue(startID);
  int currID = startID;

  while (!Q.isEmpty())
  {
    currID = Q.dequeue();

    for (int i = 0; i < cities[currID].roadCount; i++)
    {
      int destination = cities[currID].roads[i].destinationCityID;

      if (!isVisited[destination] && (cities[destination].evacuees < cities[destination].population))
      {
        depth[destination] = depth[currID] + 1;

        if (depth[destination] > maxDepth)
        {
          maxDepth = depth[destination];
          furthestCityID = destination;
        }

        city_from[destination] = currID;
        road_from[destination] = cities[currID].roads[i];
        Q.enqueue(destination);
        isVisited[destination] = true;
      }
    }
  }

  int i = maxDepth - 1;

  pathIDs[i] = road_from[furthestCityID];
  i--;

  while (furthestCityID != startID && i >= 0)
  {
    furthestCityID = city_from[furthestCityID];
    pathIDs[i] = road_from[furthestCityID];
    i--;
  }

  return maxDepth;
}


int Evac::calculateMaxFlow(Road *pathIDs, int pathIDsSize, int evac)
{
  int min = pathIDs[0].peoplePerHour;

  for (int i = 0; i < pathIDsSize; i++)
  {
    if (pathIDs[i].peoplePerHour < min)
      min = pathIDs[i].peoplePerHour;

    int city_id = pathIDs[i].destinationCityID;
    int population = cities[city_id].population;
    int evacuees = cities[city_id].evacuees;
    int available_spots = population - evacuees;

    if (available_spots < min)
      min = available_spots;
  }

  if (cities[evac].population < min)
    min = cities[evac].population;

  return min;
}