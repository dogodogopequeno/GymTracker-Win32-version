#include "Header.h"
#include "workout.h"
#include "Exercise.h"
#include "Set.h"
#include "Rep.h"
#include "Date.h"
#include "Meal.h"
#pragma once
class app
{
public:
    app();
    app(std::vector<Workout> Workouts, std::vector<Date> Dates);
    ~app();

    //friend 
    friend std::ostream& operator<<(std::ostream& os, const app obj);

    // workout functionality
    void Print();
    std::vector<Workout>& getWorkouts();
    Workout& getWorkout(std::wstring& WorkoutName);
    void addWorkout(Workout& workout);
    void addWorkout(std::wstring name);

    // helper functions
    int sets(Exercise& obj);
    int reps(int sets, Set& obj);
    std::wstring& getFilename();

    // Date functions
    void addDate(Date& date);
    void addDate(std::chrono::system_clock::time_point timePoint);
    void addDate(int Year, int Month, int Day);
    void addDate(std::wstring date);

    Date& getDateObj(std::wstring SomeDate);
    Date& getDateObj(std::chrono::system_clock::time_point timePoint);
    std::vector<Date>& getDates();

    bool DoesDateExist(std::wstring SomeDate);


    int getVolume();
    int getReps();
    int getCals();



    std::wstring id{};
    std::vector<Workout> workouts;
    int value{};
    std::wstring input{};
    bool running = true;

    std::wstring filename {L"GymTracker.txt"};

    std::wstring weight{};
    std::wstring calories{};
    double valueWeight {};
    int valueCalories {};

    int day{};
    std::vector<Date> dates{};
    std::chrono::system_clock::time_point p_tp;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar& workouts;
        ar& dates;
    }
};

