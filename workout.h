#include "Header.h"
#include "Exercise.h"
#include "Set.h"
#include "Rep.h"
#pragma once
class Workout
{
public:
    Workout();
    Workout(std::vector<Exercise> Exercises, std::wstring Name);
    Workout(std::wstring Name);
    Workout(const Workout& other);
    Workout(Workout&& other) noexcept;
    ~Workout();

    friend std::ostream& operator<<(std::ostream& os, const Workout obj);


    void Print();
    std::vector<Exercise>& getExercises();
    Exercise& getExercise(std::wstring& ExerciseName);
    std::wstring& getName();
    void deleteName();
    void addExercise(std::wstring& Name);
    void addExercise(Exercise& exercise);


    std::wstring id{};
    std::vector<Exercise> exercises{};
    std::wstring name {};
    size_t size = exercises.size();

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& exercises;
        ar& name;
    }
};

