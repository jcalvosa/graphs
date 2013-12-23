

#include "Road.hh"

Road::Road(float distance, float maxSpeed) :
  _distance(distance),
  _maxSpeed(maxSpeed)
{
}
    
Road::Road() :
  _distance(0.f),
  _maxSpeed(0.f)
{
}

Road::Road(const Road & r) :
  _distance(r.Distance()),
  _maxSpeed(r.MaxSpeed())
{
}
    
Road & Road::operator=(const Road & item)
{
  _distance = item.Distance();
  _maxSpeed = item.MaxSpeed();
  return *this;
}

const Road Road::operator+(const Road & r) const
{
  Road result = *this;
      
  result._distance += r.Distance();
  result._maxSpeed += r.MaxSpeed();
  return result;
}

bool Road::operator<(const Road & item) const
{
  return (this->Distance() < item.Distance());
}
