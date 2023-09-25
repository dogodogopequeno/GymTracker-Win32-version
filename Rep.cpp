#include "Rep.h"

Rep::Rep() {}

Rep::Rep(int Weight, int Count) : weight(Weight), count(Count) {}

Rep::~Rep()
{
   // std::cout << "Rep destroyed" << std::endl;
}

Rep::Rep(const Rep& Rep) : weight(Rep.weight), count(Rep.count)
{
  //  std::cout << "Rep copy constructor" << std::endl;
}

Rep::Rep(Rep&& Rep) noexcept : weight(std::move(Rep.weight)), count(std::move(Rep.count))
{
  //  std::cout << "Rep Move constructor" << std::endl;
}

int Rep::getWeight() const
{
    return weight;
}
int Rep::getCount() const
{
    return count;
}

std::ostream& operator<<(std::ostream& os, const Rep obj)
{
    return os;
}

void Rep::Print()
{
    std::cout << "Weight: " << getWeight() << std::endl << "Count: " << getCount() << std::endl;
}