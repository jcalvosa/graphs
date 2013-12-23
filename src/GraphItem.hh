
#ifndef __GraphItem_hh__
#define __GraphItem_hh__

#include <string>
#include <iostream>

class GraphItem
{
  public:
    GraphItem(const std::string & idItem,
              float latitude, 
              float longitude);

    GraphItem(const std::string & idItem);

    GraphItem();

    // getters
    std::string Id() const 		{ return _idItem; }
    float Latitude() const		{ return _latitude; }
    float Longitude() const		{ return _longitude; }

    // operators
    bool operator==(const GraphItem & item) const;
    GraphItem & operator=(const GraphItem & item);
    bool operator!=(const GraphItem & item) const;
    bool operator<(const GraphItem & item) const;

    friend std::ostream & operator<<(std::ostream & os, const GraphItem & item);

  protected:
    std::string _idItem;
    float _latitude;
    float _longitude;
};


#endif
