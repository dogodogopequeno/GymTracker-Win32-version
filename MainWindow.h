#include "OldGymTracker/Header.h"
#include <Windows.h>
#include "OldGymTracker/app.h"


#pragma once
class MainWindow
{
public:
	MainWindow(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow);
	~MainWindow();

	int Run();

	static LRESULT CALLBACK WindowsProcedureStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Non-static member function for handling the window procedure
	LRESULT CALLBACK WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	void DestroyAllWindows(HWND hWnd);
	void AddMenus(HWND hWnd);
	void AddWeightInputControls(HWND hWnd);
	void DisplayWeightControl(HWND hWnd);
	void DisplayCalorieControl(HWND hWnd);
	void AddCalorieInputControls(HWND hWnd);
	void AddWorkoutChooseControls(HWND hWnd);
	void AddNewWorkoutInputControls(HWND hWnd);
	void AddToExistentWorkoutInputControls(HWND hWnd);
	void AddExerciseToWorkoutInputControls(HWND hWnd);
	void AddSetsToExerciseInputControls(HWND hWnd, Exercise& exercise);
	int DisplayWorkouts(HWND hWnd);
	void AddAboutMenuControls(HWND hWnd);
	void AddProfileMenuControls(HWND hWnd);

	void CreateSpecificFont(HFONT& hFont, std::wstring& Font, HWND hWnd, long& fontHeight);
	long FontSize(int fontSize);

	Date& getToday();


	//cases
	enum MenuCase
	{
		WORKOUT_MENU1 = 0, WORKOUT_MENU2, CALORIE_MENU1, CALORIE_MENU2, WEIGHT_MENU1, WEIGHT_MENU2, IDC_ADD_WEIGHT,
		IDC_DISPLAY_WEIGHT, IDC_ADD_CALORIES_TODAY, IDC_ADD_CALORIES_SOMEOTHERDAY, IDC_ADD_NEW_WORKOUT,
		IDC_ADD_EXISTENT_WORKOUT, NEW_WORKOUT_VALIDATION, EXISTENT_WORKOUT_VALIDATION, ADD_EXERCISE_TO_WORKOUT_VALIDATION, ADD_SET_TO_EXERCISE = 16, 
		PROFILE_MENU, ABOUT_MENU, 

	};

	//Variables for windows
	WNDCLASSW wc = { 0 };

	MSG msg = { 0 };
	// menus
	HMENU hMainMenu{};
	HMENU hWorkoutMenu{};
	HMENU hCalorieMenu{};
	HMENU hWeightMenu{};

	// weight handlers
	HWND hAddWeight{};
	HWND hAddWeightHelper{};
	HWND hWeightButton{};
	HWND hDisplayWeight{};
	HWND hDisplayWeightHelper{};

	// calorie handlers
	HWND hCalorieHelper{};

	HWND hAddCalorieTodayHelper{};
	HWND hCalorieToday{};
	HWND hCalorieButtonToday{};

	HWND hAddCalorieDay{};
	HWND hAddCalorieDayHelper{};

	HWND hAddCalorieHelper{};
	HWND hCalorie{};
	HWND hCalorieButton{};

	// workout handlers

	HWND hAddWorkoutHelper{};
	HWND hAddNewWorkoutButton{};
	HWND hAddExistentWorkoutButton{};
	HWND hAddNewWorkoutHelper{};
	HWND hNewWorkout{};
	HWND hNewWorkoutButton{};
	HWND hAddExistentWorkoutHelper{};
	HWND hExistentWorkout{};
	HWND hExistentWorkoutButton{};

	// exercise handlers

	HWND hNEWorkout{};
	HWND hNEWorkoutDisplay{};
	HWND hNEWExerciseHelper{};
	HWND hNEWExerciseEdit{};
	HWND hNEWExerciseButton{};

	// SETS HANDLERS

	HWND hSetsHelper{};
	HWND hRepsToSetHelper{};
	HWND hRepsEdit{};
	HWND hWeightToSetHelper{};
	HWND hWeightEdit{};
	HWND hSetsButton{};

	//profile handlers
	HWND hNumberWorkoutsHelper{};
	HWND hNumberWorkouts{};
	HWND hVolumeHelper{};
	HWND hVolumeH{};
	HWND hRepsHelper{};
	HWND hReps{};
	HWND hCalsHelper{};
	HWND hCals{};
	HWND hPWeightHelper{};
	HWND hPWeight{};
	HWND hWeightProgress{};


	// start menu
	void AddStartMenuControls(HWND hWnd);
	HWND hNotice{};
	HWND hStart{};
	HWND hMade{};

	HBRUSH hBrushLabel{};


	 //variables for other functionality

	std::wstring MainWindowName = L"GymTracker";
	std::wstring Weight = {};
	std::wstring TodaysDate{};
	std::vector<HWND> controlHandles{};

	std::wstring workout{};
	std::wstring exercise{};
	std::wstring reps{};
	std::wstring weight{};

	int clientHeight{};
	int totalContentHeight{};

	bool controlsCreated{};

	app App;

	HINSTANCE pInst;

	int g_scrollY = 0;
};

