#include "Meal.h"

Meal::Meal () {}
Meal::Meal(int Calories) : calories(Calories){ }

Meal::Meal(const Meal& other) : calories(other.calories) { }

Meal::Meal(Meal&& other) noexcept : calories(std::move(other.calories)) { }


std::ostream& operator<<(std::ostream& os, const Meal obj)
{
	return os;
}
int Meal::getCalories() const
{
	return calories;
}

void Meal::Print() const
{
	std::cout << "Calories: " << getCalories() << std::endl;
}



/*void Meal::addCalories(int i_cal, int i_day)
{
	calories = i_cal;
	day = i_day;
}*/