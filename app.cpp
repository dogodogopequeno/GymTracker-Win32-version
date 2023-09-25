#include "app.h"

app::app()
{
}

app::app(std::vector<Workout> Workouts, std::vector<Date> Dates) : workouts(Workouts), dates(Dates) {}

app::~app() {}

std::wstring& app::getFilename()
{
	return filename;
}

std::ostream& operator<<(std::ostream& os, const app obj)
{
	return os;
}

void app::Print()
{
	for (Workout& workout : workouts)
	{
		std::wcout << "Workout: " << workout.getName() << '\n' << std::endl;
		workout.Print();
	}
}

std::vector<Workout>& app::getWorkouts()
{
	return workouts;
}

Workout& app::getWorkout(std::wstring& WorkoutName)
{
	for (Workout& workout : workouts)
	{
		if (WorkoutName == workout.getName())
		{
			return workout;
		}
	}
	std::cout << "Didn't find workout" << std::endl;
}

void app::addWorkout(Workout& workout)
{
	workouts.push_back(workout);
}

void app::addWorkout(std::wstring name)
{
	workouts.push_back(Workout(name));
}

void app::addDate(int Year, int Month, int Day)
{
	dates.push_back(Date(Year, Month, Day));
}

void app::addDate(Date& date)
{
	dates.push_back(date);
}


void app::addDate(std::chrono::system_clock::time_point timePoint)
{
	dates.push_back(Date(timePoint));
}


Date& app::getDateObj(std::wstring SomeDate)
{
	for (Date& Date : dates)
	{
		if (Date.date == SomeDate)
		{
			return Date;
		}
	}
	std::cout << "date not found" << std::endl;

}

Date& app::getDateObj(std::chrono::system_clock::time_point timePoint)
{
	for (Date& date : dates)
	{
		if (date.getDateTP() == timePoint)
		{
			return date;
		}
	}
	throw std::runtime_error("No valid date found");  // Throw an exception
	std::cout << "date not found" << std::endl;
}

std::vector<Date>& app::getDates()
{
	return dates;
}

bool app::DoesDateExist(std::wstring SomeDate)
{
	for (Date& date : dates)
	{
		if (date.date == SomeDate)
		{
			return true;
		}
		else
			return false;
	}
	return false;
}

void app::addDate(std::wstring date)
{
	dates.push_back(Date(date));
}


int app::getVolume()
{
	int volume{};
	for (Workout& workout : workouts)
	{
		for (Exercise& exercise : workout.getExercises())
		{
			for (Set& set : exercise.getSets())
			{
				for (Rep& rep : set.getReps())
				{
					int weight = rep.getWeight() * rep.getCount();
					volume += weight;
				}
			}
		}
	}

	return volume;
}

int app::getReps()
{
	int Reps{};
	for (Workout& workout : workouts)
	{
		for (Exercise& exercise : workout.getExercises())
		{
			for (Set& set : exercise.getSets())
			{
				for (Rep& rep : set.getReps())
				{
					Reps += rep.getCount();
				}
			}
		}
	}

	return Reps;
}

int app::getCals()
{
	int cals{};
	for (Date& date : dates)
	{
		for (Meal& meal : date.getMeals())
		{
			cals += meal.getCalories();
		}
	}
	return cals;
}
