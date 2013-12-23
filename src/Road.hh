

#ifndef __Road_hh__
#define __Road_hh__

class Road
{
  public:
    Road();
    Road(float distance, float maxSpeed);
    Road(const Road & r);
    
    // operators
    Road & operator=(const Road & item);
    const Road operator+(const Road & r) const;
    bool operator<(const Road & item) const;

    // getters
    float Distance() const 	{ return _distance; }
    float MaxSpeed() const	{ return _maxSpeed; }
  protected:
    float _distance;
    float _maxSpeed;
};


#endif
