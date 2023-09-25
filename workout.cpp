#include "Workout.h"

Workout::Workout()
{
	//std::cout << "Workout Normal constructor" << std::endl;
}

Workout::Workout(std::vector<Exercise> Exercises, std::wstring Name) : exercises(Exercises), name(Name) {}

Workout::~Workout()
{
	//std::cout << "Workout destroyed" << std::endl;
}
Workout::Workout(std::wstring Name) : name(Name)
{
	//std::cout << "Workout String constructor" << std::endl;
}

Workout::Workout(const Workout& other) : name(other.name)
{
	//std::cout << "Workout Copy Constructor" << std::endl;
}

//Workout::Workout(const Workout&& other) noexcept
//{
//	std::cout << "Workout Move Constructor" << std::endl;
//}

Workout::Workout(Workout&& other) noexcept : name(std::move(other.name)), exercises(std::move(other.exercises))
{
	//std::cout << "Workout Move Constructor" << std::endl;
}

std::ostream& operator<<(std::ostream& os, const Workout obj)
{
	return os;
}

void Workout::Print()
{
	for (Exercise& exercise : exercises)
	{
		exercise.Print();
		std::vector<Set>& sets = exercise.getSets();
		for (Set& set : sets)
		{
			std::vector<Rep>& reps = set.getReps();
			for (Rep& rep : reps)
			{
				set.Print();
				rep.Print();
			}
		}
	}
}

std::vector<Exercise>& Workout::getExercises()
{
	return exercises;
}

void Workout::addExercise(std::wstring& Name)
{
	exercises.push_back(Exercise(Name));
	
}

void Workout::addExercise(Exercise& exercise)
{
	exercises.push_back(exercise);

}

Exercise& Workout::getExercise(std::wstring& ExerciseName)
{
	for (Exercise& exercise : exercises)
	{
		if (ExerciseName == exercise.GetName())
		{
			return exercise;
		}
	}
	std::cout << "Didn't find exercise" << std::endl;
}

std::wstring& Workout::getName()
{
	return name;
}

void Workout::deleteName()
{
  name = {};
}



