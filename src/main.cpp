
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "Graph.hh"
#include "GraphItem.hh"
#include "Road.hh"


std::ostream & operator<<(std::ostream & os, const GraphItem & item)
{
  os << item.Id() << ":" << item.Latitude() << " - " << item.Longitude();
  return os;
}

typedef jcs::Graph<GraphItem, Road> MyGraph;

void LoadCities(const std::string & f, MyGraph & graph)
{
  std::string city;
  std::string latitude;
  float lat;
  float lon;
  
  std::ifstream file(f);

  for (std::string fileLine; !std::getline(file, fileLine).eof();)
  {
    std::stringstream aux(fileLine);

    std::getline(aux, city, ';');
    std::getline(aux, latitude, ';');
    aux >> lon;
    
    std::stringstream latitudeString(latitude);
    latitudeString >> lat;
    
    auto addingData2 = graph.AddVertex(GraphItem(city, lat, lon));
    if (addingData2.second)
    {
      auto & item = addingData2.first->first;
    }
  }
  
  file.close();
}

float CalcDistance2D(const GraphItem & origin, const GraphItem & destination)
{
  float latDiff = origin.Latitude() - destination.Latitude();
  float lonDiff = origin.Longitude() - destination.Longitude();
  
  return sqrt((latDiff * latDiff) + (lonDiff * lonDiff));	
}

void LoadRoads(const std::string & f, MyGraph & graph)
{
  std::ifstream file(f);

  std::string city1;
  std::string city2;
  float maxSpeed;
  
  for (std::string fileLine; !std::getline(file, fileLine).eof();)
  {
    std::stringstream aux(fileLine);

    std::getline(aux, city1, ';');
    std::getline(aux, city2, ';');
    aux >> maxSpeed;
    
    auto originItemData = graph.AddVertex(GraphItem(city1));
    auto destinationItemData = graph.AddVertex(GraphItem(city2));
  
    // both vertex belong to graph
    if (!originItemData.second && !destinationItemData.second)
    {
      auto distance = CalcDistance2D(originItemData.first->first, destinationItemData.first->first);
      
      graph.AddEdge(originItemData.first->first, destinationItemData.first->first, Road(distance, maxSpeed));
    }
  }
  
  file.close();
}

class DistanceValidator : public MyGraph::IWeightCalculator
{
  public:
    virtual float CalcDistance(const Road & weight) const
    {
      return weight.Distance();
    }
};

class TimeValidator : public MyGraph::IWeightCalculator
{
  public:
    virtual float CalcDistance(const Road & weight) const
    {
      return (weight.Distance() / weight.MaxSpeed());
    }
};

int main(int, char **)
{
  static const std::string cities = "../data/spanishCities_test.csv";
  static const std::string roads = "../data/spanishRoads_test.csv";

  MyGraph graph(MyGraph::_NotDirected_);

  std::cout << "Adding cities from " << cities << "\n";
  LoadCities(cities, graph);
  std::cout << "Adding roads from " << roads << "\n";
  LoadRoads(roads, graph);

  GraphItem c1("1");
  GraphItem c2("8");

  // search distance between 1 and 8
  DistanceValidator dValidator;
  auto distances = graph.Dijkstra(c1, c2, dValidator);

  std::cout << "Distances from " << c1.Id() << "\n";
  std::for_each(distances.cbegin(), distances.cend(), 
                [&](const MyGraph::DijkstraEdge & item)
  {
    std::cout << " - " << item._item.Id() << " : " << dValidator.CalcDistance(item._weigth) << "\n";
  });

  std::cout.flush();

  // search time between 1 and 8
  TimeValidator tValidator;
  auto times = graph.Dijkstra(c1, c2, tValidator);

  std::cout << "Time from " << c1.Id() << "\n";
  std::for_each(times.cbegin(), times.cend(),
                [&](const MyGraph::DijkstraEdge & item)
  {
    std::cout << " - " << item._item.Id() << " : " << tValidator.CalcDistance(item._weigth) << "\n";
  });

  std::cout.flush();

  return 0;
}
