#ifndef evacH
#define evacH

#include "EvacRunner.h"


class Evac
{
City* cities;
int numCities;

public:
  Evac(City *cities, int numCities, int numRoadss);
  void evacuate(int *evacIDs, int numEvacs, EvacRoute *evacRoutes, int &routeCount);
  void moveNeighbors(int startID, bool* visited, EvacRoute *evacRoutes, int& routeCount, int time);
  bool moreToEvacuate(int numEvacs, int* evacIDs);
}; // class Evac




#endif
