#ifndef evacH
#define evacH

#include "EvacRunner.h"


class Evac
{
    int depth[10000];
  bool isVisited[10000];
  int city_from[10000];
  Road road_from[10000];
City* cities;
int numCities;
public:

  Evac(City *cities, int numCities, int numRoadss);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes,
    int &routeCount); // student sets evacRoutes and routeCount
  int findFurthestCity(int *evacIDs, int startID, int numEvacs, Road *pathIDs);
  int calculateMaxFlow(Road *pathIDs, int pathIDsSize, int evac);

}; // class Evac




#endif
