
#ifndef __Graph_h__
#define __Graph_h__

#include <utility>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <limits>



namespace jcs
{

template <typename T>
class Graph
{
  // graph containers
  typedef std::pair<const T, float> WeightToNode;
  typedef std::set<WeightToNode> Weights;
  typedef std::map<const T, Weights> WeigthMatrix;

  public:
    enum Type
    {
      _Directed_,
      _NotDirected_
    };
    
    typedef typename WeigthMatrix::iterator WeigthsIterator;

    struct DijkstraEdge
    {
      const T * _item;
      float _weigth;
    };

    typedef std::deque<DijkstraEdge> DijkstraPath;

  public:
    Graph(Type t) :
      _type(t)
    {
    }

    std::pair<WeigthsIterator, bool> AddVertex(const T & item)
    {
      Weights weights;

      return _weigthMatrix.insert(std::make_pair(item, weights));
    }

    void AddEdge(const T & source, const T & dest, float _weigth)
    {
      SetEdge(source, dest, _weigth);

      if (_type == _NotDirected_)
      {
        SetEdge(dest, source, _weigth);
      }
    }

    float GetWeigth(const T & source, const T & dest) const
    {
      auto item = _weigthMatrix.find(source);

      for (auto iter = item->second.begin(); iter != item->second.end(); ++iter)
      {
        if (iter->first == dest)
        {
          return iter->second;
        }
      }

      return std::numeric_limits<float>::max();
    }

    DijkstraPath Dijkstra(const T & source, const T & dest)
    {
      DijkstraWeights weights;
      std::set<const T *> openCandidates;

      // fill dijkstra information
      FillInternalStructures(weights);

      // source is main candidate item
      auto & candidate = GetDijkstraCandidate(source, weights);
      candidate._weigth = 0.0f;

      openCandidates.insert(candidate._graphItem);

      while (!openCandidates.empty())
      {
        // get lowest weight item not visited
        auto & lowestItem = GetDijkstraCandidate(weights);

        // delete item from candidates queue
        openCandidates.erase(lowestItem._graphItem);

        // set item as visited
        lowestItem._visited = true;

        // calculate weight to actual item
        float weightToActualItem = lowestItem._weigth;

        // for all actual item neighbours
        try
        {
          auto & weightsToNeighbours = GetNeighbours(*lowestItem._graphItem);

          for (auto iter = weightsToNeighbours.begin(); iter != weightsToNeighbours.end(); ++iter)
          {
            // get actual to neigh weigth
            float weightFromActualToNeighbour = iter->second;

            // get distance for neighbour
            auto & neighbourItem = GetDijkstraCandidate(iter->first, weights);

            // possible new weight
            float alt = weightToActualItem + weightFromActualToNeighbour;

            // is less than actual distance?
            if (alt < neighbourItem._weigth)
            {
              // update actual distande
              neighbourItem._weigth = alt;
              neighbourItem._previous = lowestItem._graphItem;

              // if this neighbour is not visited, save on cadidates queue
              if (!neighbourItem._visited)
              {
                openCandidates.insert(neighbourItem._graphItem);
              }
            }
          }
        }
        catch (const DijkstraError &)
        {

        }
      }

      // now, all is calculated, backwards from destiny and get the complete path
      return GetDijkstraPath(source, dest, weights);
    }

  private:
    // graph type
    Type _type;

    // main weights matrix: adjacency_list with weights
    WeigthMatrix _weigthMatrix;

    void SetEdge(const T & source, const T & dest, float w)
    {
      auto item = _weigthMatrix.find(source);

      if (item == _weigthMatrix.end())
      {
        Weights weight;

        weight.insert(std::make_pair(dest, w));
        _weigthMatrix.insert(std::make_pair(source, weight));
      }
      else
      {
        auto & nodeWeights = item->second;    
        nodeWeights.insert(std::make_pair(dest, w));
      }
    }

    // for internal Dijkstra algorithm: structures and methods
    struct DijkstraItem
    {
      float _weigth;
      bool _visited;
      const T * _graphItem;
      const T * _previous;
    };

    typedef std::vector<DijkstraItem> DijkstraWeights;

    void FillInternalStructures(DijkstraWeights & weights)
    {
      // fill difkstra configuration
      for (auto iter = _weigthMatrix.begin(); iter != _weigthMatrix.end(); ++iter)
      {
        DijkstraItem dItem;

        dItem._visited = false;
        dItem._previous = nullptr;
        dItem._graphItem = &iter->first;
        dItem._weigth = std::numeric_limits<float>::max();

        weights.push_back(dItem);
      }
    }


    DijkstraItem & GetDijkstraCandidate(const DijkstraWeights & weights) const
    {
      unsigned position = std::numeric_limits<unsigned>::max();
      unsigned cont = 0;

      float dist = std::numeric_limits<float>::max();

      for (auto iter = weights.begin(); iter != weights.end(); ++iter)
      {
        if (!iter->_visited && iter->_weigth < dist)
        {
          position = cont;
          dist = iter->_weigth;
        }
        cont ++;
      }

      return const_cast<DijkstraItem &>(weights[position]);
    }

    DijkstraItem & GetDijkstraCandidate(const T & source, 
                                        const DijkstraWeights & weights) const
    {
      unsigned position = 0;

      for (auto iter = weights.begin(); iter != weights.end(); ++iter)
      {
        if (*(iter->_graphItem) == source)
        {
          break;
        }

        position++;
      }

      return const_cast<DijkstraItem &>(weights[position]);
    }

    enum DijkstraError
    {
      ItemNotFound
    };

    Weights & GetNeighbours(const T & item) const
    {
      auto neighboursIter = _weigthMatrix.find(item);

      if (neighboursIter != _weigthMatrix.end())
      {
        return const_cast<Weights &>(neighboursIter->second);
      }

      throw ItemNotFound;
    }

    DijkstraPath GetDijkstraPath(const T & source, 
                                 const T & dest, 
                                 const DijkstraWeights & weights)
    {
      DijkstraPath aux;
  
      T item(dest);

      bool cont = true;
      while (cont)
      {
        if (item != source)
        {
          for (auto iter = weights.cbegin(); iter != weights.cend(); ++iter)
          {
            if (*(iter->_graphItem) == item)
            {
              if (iter->_previous)
              {
                DijkstraEdge x;

                x._weigth = GetWeigth(*iter->_previous, *iter->_graphItem);
                x._item = iter->_graphItem;

                aux.push_front(x);

                item = *(iter->_previous);
              }
              else
              {
                cont = false;
              }
              break;
            }
          }
        }
        else
        {
          cont = false;
        }
      }

      return aux;
    }



};


} // end namespace

#endif

