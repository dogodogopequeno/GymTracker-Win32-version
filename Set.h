#include "Header.h"
#include "Rep.h"
#pragma once
class Set
{
public:

    Set();
    Set(int setID, std::vector<Rep> Reps);
    Set(int setID);
    Set(const Set& set);
    Set(Set&& set) noexcept;
    ~Set();
    void addRep(int weight, int count);
    int getId() const;
    friend std::ostream& operator<<(std::ostream& os, const Set obj);
    void Print();
    std::vector<Rep>& getReps();


    int setId{};
    std::vector<Rep> reps {};

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& setId;
        ar& reps;
    }
};

