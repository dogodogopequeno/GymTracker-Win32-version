#include "Header.h"
#include "Meal.h"
#pragma once

class Date
{
	public:
	std::vector<Meal> meals{};
	double weight{};
	std::chrono::system_clock::time_point now;
	std::chrono::system_clock::time_point timePoint_{};
	std::chrono::year_month_day ymd{};
	int year {};
	int month{};
	int day{};
	std::wstring date{};

public:
	Date();
	Date(std::wstring Date);
	Date(int Year, int Month, int Day);
	Date(Date&& other) noexcept;
	Date(const Date& other);
	Date(std::chrono::system_clock::time_point timePoint);
	void Print();
	void PrintDay();
	void AddMeal(int calories);
	void AddWeight(double Weight);
	double GetWeight();
	auto getID();
	std::chrono::system_clock::time_point getDateTP();
	friend std::ostream& operator<<(std::ostream& os, const Meal obj);

	std::vector<Meal>& getMeals();
	int GoThroughMealsReturnCalories();

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& meals;
		ar& year;
		ar& month;
		ar& day;
		ar& weight;
		ar& date;
	}
};

