#include <cstdlib>
#include <cstring>
#include <assert.h>
#include "evac.h"
#include "EvacRunner.h"
#include "QueueAr.h"

#define cerr if (cerr_disabled) {} else std::cerr
bool cerr_disabled = true;

using namespace std;

Evac::Evac(City *city, int numCity, int numRoadss)
{
  cities = new City[numCity];

  for (int i = 0; i < numCity; i++)
    cities[i] = city[i];


  for (int i = 0; i < numCity; i++)
  {
    cities[i].roads = new Road[cities[i].roadCount];

    for (int j = 0; j < city[i].roadCount; j++)
      cities[i].roads[j] = city[i].roads[j];
  }

  numCities = numCity;
  numRoads = numRoadss;

} // Evac()


void Evac::evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount)
{
  routeCount = 0;
  int time = 1;

  for (int i = 0; i < numEvacs; i++)
  {
    while(cities[evacIDs[i]].population > 0)
    {

          cerr << "evacuating city: " << evacIDs[i] << " with current population: " <<  cities[evacIDs[i]].population << endl;

      Road pathIDs[numCities];
      int furthestCityID = -1;

      int pathIDsSize = findFurthestCity(evacIDs, evacIDs[i], numEvacs, pathIDs, furthestCityID);
      int maxFlow = calculateMaxFlow(pathIDs, pathIDsSize, evacIDs[i]);


      if (cities[evacIDs[i]].population < maxFlow)
        maxFlow = cities[evacIDs[i]].population;

      cerr << "sending to destination: " << cities[ pathIDs[pathIDsSize - 1].destinationCityID ].ID << " with max flow: " << maxFlow << endl;

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
        // for (int i = 0; i < numCities; i++)
        //   cout << cities[i].evacuees << ", " << cities[i].population << endl;

        // if (routeCount > 15000)
        // {
        //   for (int i = 0; i < routeCount; i++)
        //   {
        //     cout << evacRoutes[i].roadID << ", " << evacRoutes[i].time << ", " << evacRoutes[i].numPeople << endl;
        //   }

        //   cout << cities[3].population << endl;
        //   cout << cities[3].evacuees << endl;


        //   for (int i = 0; i < pathIDsSize; i++)
        //   {
        //     cout << pathIDs[i].destinationCityID << endl;
        //   }

        //   cout << endl << evacIDs[i];
        //   cout << pathIDsSize;
          
        //   goto endloop;
        // }
      }
      
      time++;
    }
    
    // endloop:;
  }

  // for (int i = 0; i < routeCount; i++)
  // {
  //  cout << evacRoutes[i].roadID << ", " << evacRoutes[i].time << ", " << evacRoutes[i].numPeople << endl;
  // }

} // evacuate

int Evac::findFurthestCity(int *evacIDs, int startID, int numEvacs, Road *pathIDs, int& furthest)
{
  bool isVisited[numCities];
  int depth[numCities], maxDepth = 0;
  Queue<int> Q(numCities);
  int city_from[numCities];
  Road road_from[numCities];
  int furthestCityID = 0;
  
  memset(isVisited, 0, sizeof(isVisited));
  memset(depth, 0, sizeof(depth));
  memset(city_from, 0, sizeof(city_from));
  memset(road_from, 0, sizeof(road_from));

  for (int i = 0; i < numEvacs; i++)
    isVisited[evacIDs[i]] = true;

  Q.enqueue(startID);
  int currID = startID;

  bool allEvac = true;

  for (int i = 0; i < cities[currID].roadCount; i++)
    if (!isVisited[ cities[currID].roads[i].destinationCityID ])
    {
      allEvac = false;
      break;
    }


  if (allEvac)
    for (int i = 0; i < cities[currID].roadCount; i++)
      isVisited[ cities[currID].roads[i].destinationCityID ] = false;


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

  furthest = furthestCityID;
 
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

  return min;
}