
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>

#include "Graph.hh"

class GraphItem
{
  public:
    GraphItem(const std::string & idItem,
              float latitude, 
              float longitude) : 
      _idItem(idItem),
      _latitude(latitude),
      _longitude(longitude)
    {
    }

    GraphItem(const std::string & idItem) :
      _idItem(idItem),
      _latitude(0.f),
      _longitude(0.f)
    {
    }

    GraphItem() :
      _idItem(""),
      _latitude(0.f),
      _longitude(0.f)
    {
    }

    // getters
    std::string Id() const 		{ return _idItem; }
    float Latitude() const		{ return _latitude; }
    float Longitude() const		{ return _longitude; }

    // operators
    bool operator==(const GraphItem & item) const
    {
      return (this->Id() == item.Id());
    }

    GraphItem & operator=(const GraphItem & item)
    {
      _idItem = item.Id();
      return *this;
    }

    bool operator!=(const GraphItem & item) const
    {
      return (this->Id() != item.Id());
    }

    bool operator<(const GraphItem & item) const
    {
      return (this->Id() < item.Id());
    }

    friend std::ostream & operator<<(std::ostream & os, const GraphItem & item);

  protected:
    std::string _idItem;
    float _latitude;
    float _longitude;
};


std::ostream & operator<<(std::ostream & os, const GraphItem & item)
{
  os << item.Id() << ":" << item.Latitude() << " - " << item.Longitude();
  return os;
}

class Road
{
  public:
    Road(float distance, float maxSpeed) :
      _distance(distance),
      _maxSpeed(maxSpeed)
    {
    }
    
    Road() :
      _distance(0.f),
      _maxSpeed(0.f)
    {
    }

    Road(const Road & r) :
      _distance(r._distance),
      _maxSpeed(r._maxSpeed)
    {
    }
    
    Road & operator=(const Road & item)
    {
      _distance = item._distance;
      _maxSpeed = item._maxSpeed;
      return *this;
    }

    const Road operator+(const Road & r) const
    {
      Road result = *this;
      
      result._distance += r._distance;
      result._maxSpeed += r._maxSpeed;
      return result;
    }

    bool operator<(const Road & item) const
    {
      return (this->Distance() < item.Distance());
    }

    // getters
    float Distance() const 	{ return _distance; }
    float MaxSpeed() const	{ return _maxSpeed; }
    
  protected:
    float _distance;
    float _maxSpeed;
};

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
      
      std::cout << "Adding vertex: " << item << "\n";
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
  MyGraph graph(MyGraph::_NotDirected_);

  LoadCities("../data/spanishCities_test.csv", graph);
  LoadRoads("../data/spanishRoads_test.csv", graph);

  GraphItem c1("1");
  GraphItem c2("8");

  // search distance between 1 and 8
  DistanceValidator dValidator;
  auto distances = graph.Dijkstra(c1, c2, dValidator);

  std::cout << "Distances from " << c1.Id() << "\n";
  for (auto iter = distances.cbegin(); iter != distances.cend(); ++iter)
  {
    std::cout << " - " << iter->_item.Id() << " : " << dValidator.CalcDistance(iter->_weigth) << "\n";
  }

  std::cout.flush();

  // search time between 1 and 8
  TimeValidator tValidator;
  auto times = graph.Dijkstra(c1, c2, tValidator);

  std::cout << "Time from " << c1.Id() << "\n";
  for (auto iter = times.cbegin(); iter != times.cend(); ++iter)
  {
    std::cout << " - " << iter->_item.Id() << " : " << tValidator.CalcDistance(iter->_weigth) << "\n";
  }

  std::cout.flush();


  return 0;
}
