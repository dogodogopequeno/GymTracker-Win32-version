#include "Header.h"
#include "Set.h"
#pragma once
class Exercise
{
public:
    Exercise();
    Exercise(std::wstring Name, std::vector<Set> Sets);
    Exercise(const std::wstring& name);
    Exercise(const Exercise& exercise);
    Exercise(Exercise&& exercise) noexcept;
    ~Exercise();
    void addSet(int setId);
    void addRepToSet(int setId, int weight, int count);
    void Print();
    std::wstring GetName() const;
    friend std::ostream& operator<<(std::ostream& os, const Exercise obj);

    std::vector<Set>& getSets();


    std::wstring name {};
    std::vector<Set> sets{};

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& name;
        ar& sets;
    }
};

