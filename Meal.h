#include "Header.h"
#pragma once
class Meal
{
private:
	int calories{};
public:
	Meal();
	Meal(int Calories);
	Meal(const Meal& other);
	Meal(Meal&& other) noexcept;
	int getCalories() const;
	void Print() const;
	friend std::ostream& operator<<(std::ostream& os, const Meal obj);

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& calories;
	}
};

