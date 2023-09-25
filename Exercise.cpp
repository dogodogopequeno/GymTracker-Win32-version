#include "Exercise.h"

Exercise::Exercise() {}

Exercise::Exercise(std::wstring Name, std::vector<Set> Sets) : name(Name), sets(Sets) {}

Exercise::Exercise(const std::wstring& name) : name(name) 
{
  //  std::cout << "Exercise String constructor" << std::endl;
}

Exercise::Exercise(const Exercise& exercise) : name(exercise.name)
{
    //std::cout << "Exercise copy constructor" << std::endl;
}

Exercise::Exercise(Exercise&& exercise) noexcept : name(std::move(exercise.name)), sets(std::move(exercise.sets))
{
    //std::cout << "Exercise Move constructor" << std::endl;
}

Exercise::~Exercise()
{
   // std::cout << "Exercise destroyed" << std::endl;
}

void Exercise::addSet(int setId) 
{
    sets.push_back(Set(setId));
}
void Exercise::addRepToSet(int setId, int weight, int count) 
{
    for (Set& set : sets) {
        if (set.getId() == setId) {
            set.addRep(weight, count);
            break;
        }
    }
}

 std::vector<Set>& Exercise::getSets() 
{
    return sets;
}

std::ostream& operator<<(std::ostream& os, const Exercise obj)
{
    return os;
}

std::wstring Exercise::GetName() const
{
    return name;
}

void Exercise::Print()
{
    std::cout << "Exercise: " << GetName() << std::endl;
}
