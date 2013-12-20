
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Graph.hh"

class GraphItem
{
  public:
    GraphItem(const std::string & idItem) : _idItem(idItem)
    {
    }

    std::string Id() const { return _idItem; }

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
};


std::ostream & operator<<(std::ostream & os, const GraphItem & item)
{
  os << item.Id();
  return os;
}

void LoadDistanciasKilometricas(const std::string & f, 
                                jcs::Graph<GraphItem> & graph)
{
  std::string city;
  float distance;

  std::ifstream fichero(f);

  for (std::string fileLine; std::getline(fichero, fileLine);)
  {
    std::stringstream aux(fileLine);

    std::getline(aux, city, ';');

    auto addingData1 = graph.AddVertex(GraphItem(city));
    if (addingData1.second)
    {
      std::cout << "Adding vertex: " << addingData1.first->first.Id() << "\n";
    }

    std::getline(aux, city, ';');

    auto addingData2 = graph.AddVertex(GraphItem(city));
    if (addingData2.second)
    {
      std::cout << "Adding vertex: " << addingData2.first->first.Id() << "\n";
    }

    aux >> distance;

    graph.AddEdge(addingData1.first->first, addingData2.first->first, distance);

    std::cout << "Adding edge: " << addingData1.first->first << "-" 
              << addingData2.first->first << "(" << distance << ")" << "\n";
  }

  fichero.close();
}

int main(int, char **)
{
  jcs::Graph<GraphItem> graph(jcs::Graph<GraphItem>::_NotDirected_);

//   LoadDistanciasKilometricas("C:/Users/27777/Downloads/tmp/distanciasKilometricas3.csv", graph);
  LoadDistanciasKilometricas("../data/distanciasKilometricas3.csv", graph);

  //std::ifstream fichero("C:/Users/27777/Downloads/tmp/GeoLiteCity-latest/GeoLiteCity_20131203/ciudadesEspanolas.csv");

  // search distance between LBR and SLO
  GraphItem c1("WSM");
  GraphItem c2("FSM");

  auto datos = graph.Dijkstra(c1, c2);

  std::cout << c1.Id() << "\n";
  for (auto iter = datos.cbegin(); iter != datos.cend(); ++iter)
  {
    std::cout << " - " << iter->_item->Id() << " : " << iter->_weigth << "\n";
  }

  std::cout.flush();

  return 0;
}
