

#include "GraphItem.hh"


GraphItem::GraphItem(const std::string & idItem,
                    float latitude, 
                    float longitude) : 
  _idItem(idItem),
  _latitude(latitude),
  _longitude(longitude)
{
}

GraphItem::GraphItem(const std::string & idItem) :
                    _idItem(idItem),
                    _latitude(0.f),
                    _longitude(0.f)
{
}

GraphItem::GraphItem() :
  _idItem(""),
  _latitude(0.f),
  _longitude(0.f)
{
}

// operators
bool GraphItem::operator==(const GraphItem & item) const
{
  return (this->Id() == item.Id());
}

GraphItem & GraphItem::operator=(const GraphItem & item)
{
  _idItem = item.Id();
  return *this;
}

bool GraphItem::operator!=(const GraphItem & item) const
{
  return (this->Id() != item.Id());
}

bool GraphItem::operator<(const GraphItem & item) const
{
  return (this->Id() < item.Id());
}

