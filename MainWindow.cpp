

#include "MainWindow.h"
#include "strsafe.h"
#include "OldGymTracker/Meal.h"
//#include <dwmapi.h>
#include <iomanip>



MainWindow::MainWindow(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
	// Initialize the WNDCLASSW structure and set lpfnWndProc to the static member function
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowsProcedureStatic; // Assign the static member function here

	if (!RegisterClassW(&wc))
	{
		// Handle registration error
		// You should add error handling code here
	}


	pInst = hInst;

	CreateWindowW(L"myWindowClass", MainWindowName.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 600, NULL, NULL, hInst, this);

	// Date creation for future endevours, this is called in the constructor which is bad because even if the user doesnt add the date it gets added with zero calories
	// TODO: Fix.

	std::wstring filename = App.getFilename();

	std::ifstream ifs(filename);
	try {
		boost::archive::binary_iarchive ar(ifs);
		ar& App;
	}
	catch (const std::exception& e) { std::cout << std::endl; }

	std::chrono::system_clock::time_point timePoint = std::chrono::system_clock::now();

	auto t_ymd = std::chrono::year_month_day{ std::chrono::floor<std::chrono::days>(timePoint) };

	std::chrono::year_month_day ymd{};
	ymd = t_ymd;

	// Extract the year, month, and day components
	int year = static_cast<int>(t_ymd.year());
	int month = static_cast<unsigned>(t_ymd.month());
	int day = static_cast<unsigned>(t_ymd.day());

	TodaysDate = std::to_wstring(day) + L"/" + std::to_wstring(month) + L"/" + std::to_wstring(year);

	if (!App.DoesDateExist(TodaysDate))
	{
		App.addDate(year, month, day);
	}

}

MainWindow::~MainWindow()
{
	std::wstring filename = App.getFilename();
	std::ofstream ofs(filename);
	boost::archive::binary_oarchive ar(ofs);
	ar& App;

}

int MainWindow::Run()
{
	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

// Define a static member function for the window procedure
LRESULT CALLBACK MainWindow::WindowsProcedureStatic(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Inside this function, you can access class members using MainWindow:: prefix
	MainWindow* pThis;

	if (msg == WM_NCCREATE)
	{
		pThis = static_cast<MainWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else
	{
		pThis = reinterpret_cast<MainWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
	{
		return pThis->WindowsProcedure(hWnd, msg, wParam, lParam);
	}
	else
	{
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
}

LRESULT CALLBACK MainWindow::WindowsProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// Set the background color to dark grey
		HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64)); // RGB(64, 64, 64) corresponds to dark grey
		FillRect(hdc, &ps.rcPaint, hBrush);

		EndPaint(hWnd, &ps);

		//PAINTSTRUCT ps;
		//HDC hdc = BeginPaint(hWnd, &ps);

		//// Set the background color to dark grey
		//HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64)); // RGB(64, 64, 64) corresponds to dark grey
		//FillRect(hdc, &ps.rcPaint, hBrush);

		//// Repaint the window title
		//RECT titleRect;
		//GetClientRect(hWnd, &titleRect);
		//titleRect.bottom = GetSystemMetrics(SM_CYCAPTION); // Adjust the height for the title bar
		//SetBkMode(hdc, TRANSPARENT);
		////DrawText(hdc, MainWindowName.c_str(), -1, &titleRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		//EndPaint(hWnd, &ps);
		if (!controlsCreated)
		{
			AddStartMenuControls(hWnd);
			controlsCreated = true;
		}
		return 0; 
		break;

	}
	case WM_COMMAND:
	{
		switch (wParam)
		{
		case ABOUT_MENU:
		{
			DestroyAllWindows(hWnd);
			AddAboutMenuControls(hWnd);
			break;
		}
		case PROFILE_MENU:
		{
			DestroyAllWindows(hWnd);
			AddProfileMenuControls(hWnd);
			break;
		}
		case WEIGHT_MENU1:
		{
			DestroyAllWindows(hWnd);
			AddWeightInputControls(hWnd);
			return 0;
			break;
		}
		break;
		case IDC_ADD_WEIGHT:
		{
			const int bufferLength = ::GetWindowTextLength(hAddWeight) + 1;
			Weight.resize(bufferLength);
			GetWindowText(hAddWeight, &Weight[0], bufferLength);
			Weight.resize(bufferLength - 1);
			getToday().AddWeight(std::stod(Weight));
			return 0;
			break;
		}
		case  WEIGHT_MENU2:
		{
			DestroyAllWindows(hWnd);
			DisplayWeightControl(hWnd);
			return 0;
			break;
		}
		case CALORIE_MENU1:
		{
			DestroyAllWindows(hWnd);
			AddCalorieInputControls(hWnd);
			return 0;
			break;
		}
		case CALORIE_MENU2:
		{
			DestroyAllWindows(hWnd);
			DisplayCalorieControl(hWnd);
			return 0;
			break;
		}
		case IDC_ADD_CALORIES_TODAY:
		{
			std::wstring calories{};
			const int bufferLength = ::GetWindowTextLength(hCalorieToday) + 1;
			calories.resize(bufferLength);
			GetWindowText(hCalorieToday, &calories[0], bufferLength);
			calories.resize(bufferLength - 1);
			getToday().AddMeal(std::stoi(calories));
			return 0;
			break;
		}
		case IDC_ADD_CALORIES_SOMEOTHERDAY:
		{
			std::wstring calories{};
			std::wstring date{};
			const int bufferLengthCalories = ::GetWindowTextLength(hCalorie) + 1;
			const int bufferLengthDates = ::GetWindowTextLength(hAddCalorieDay) + 1;
			calories.resize(bufferLengthCalories);
			date.resize(bufferLengthDates);
			GetWindowText(hAddCalorieDay, &date[0], bufferLengthDates);
			GetWindowText(hCalorie, &calories[0], bufferLengthCalories);
			calories.resize(bufferLengthCalories - 1);
			date.resize(bufferLengthDates - 1);
			App.addDate(date);
			App.getDateObj(date).AddMeal(std::stoi(calories));
			return 0;
			break;
		}
		case WORKOUT_MENU1:
		{
			DestroyAllWindows(hWnd);
			AddWorkoutChooseControls(hWnd);
			return 0;
			break;
		}
		case IDC_ADD_NEW_WORKOUT:
		{
			DestroyAllWindows(hWnd);
			AddNewWorkoutInputControls(hWnd);
			return 0;
			break;
		}
		case NEW_WORKOUT_VALIDATION:
		{
			const int bufferWorkoutLength = ::GetWindowTextLength(hNewWorkout) + 1;
			workout.resize(bufferWorkoutLength);
			GetWindowText(hNewWorkout, &workout[0], bufferWorkoutLength);
			workout.resize(bufferWorkoutLength - 1);
			bool exists{ false };
			for (Workout& Workout : App.getWorkouts())
			{
				if (App.getWorkout(workout).name == workout)
				{
					exists = true;
					break;
				}
				else
				{
					exists = false;
				}
			}
			if (exists == true)
			{
				int alreadyExists{};
				alreadyExists = MessageBoxW(hWnd, L"This workout already exists. Want to try a different name?", L"Exit", MB_YESNO | MB_ICONERROR);
				if (alreadyExists == IDYES)
				{
					DestroyAllWindows(hWnd);
					AddNewWorkoutInputControls(hWnd);
				}
				else
				{
					DestroyAllWindows(hWnd);
					AddToExistentWorkoutInputControls(hWnd);
				}
			}
			else
			{

				App.addWorkout(workout);
				DestroyAllWindows(hWnd);
				AddExerciseToWorkoutInputControls(hWnd);
			}

			return 0;
			break;
		}
		case ADD_EXERCISE_TO_WORKOUT_VALIDATION:
		{
			const int bufferExerciseLength = ::GetWindowTextLength(hNEWExerciseEdit) + 1;
			exercise.resize(bufferExerciseLength);
			GetWindowText(hNEWExerciseEdit, &exercise[0], bufferExerciseLength);
			exercise.resize(bufferExerciseLength - 1);
			bool exerciseExists{ false };
			for (Exercise& Exercise : App.getWorkout(workout).getExercises())
			{
				if (Exercise.GetName() == exercise)
				{
					exerciseExists = true;
				}
				else
				{
					exerciseExists = false;
					break;
				}
			}
			if (exerciseExists == true)
			{
				int exerciseAlreadyExists{};
				exerciseAlreadyExists = MessageBoxW(hWnd, L"This exercise already exists. Want to add sets to it?", L"Exit", MB_YESNO | MB_ICONERROR);
				if (exerciseAlreadyExists == IDYES)
				{
					DestroyAllWindows(hWnd);
					AddSetsToExerciseInputControls(hWnd, App.getWorkout(workout).getExercise(exercise));
				}
				else
				{
					DestroyAllWindows(hWnd);
					AddStartMenuControls(hWnd);
				}
			}
			if (exerciseExists == false)
			{
				App.getWorkout(workout).addExercise(exercise);
				DestroyAllWindows(hWnd);
				AddSetsToExerciseInputControls(hWnd, App.getWorkout(workout).getExercise(exercise));
			}
			return 0;
			break;
		}
		case IDC_ADD_EXISTENT_WORKOUT:
		{
			DestroyAllWindows(hWnd);
			AddToExistentWorkoutInputControls(hWnd);
			return 0;
			break;
		}
		case EXISTENT_WORKOUT_VALIDATION:
		{
			std::wstring workout{};
			const int bufferWorkoutLength = ::GetWindowTextLength(hNewWorkout) + 1;
			workout.resize(bufferWorkoutLength);
			GetWindowText(hNewWorkout, &workout[0], bufferWorkoutLength);
			workout.resize(bufferWorkoutLength - 1);
			bool exists{ false };
			for (Workout& Workout : App.getWorkouts())
			{
				if (App.getWorkout(workout).name == workout)
				{
					exists = true;
					break;
				}
				else
				{
					exists = false;
				}
			}
			if (exists == false)
			{
				int doesntExist{};
				doesntExist = MessageBoxW(hWnd, L"This workout doesn't exist. Want to try a different name?", L"Exit", MB_YESNO | MB_ICONERROR);
				if (doesntExist == IDYES)
				{
					DestroyAllWindows(hWnd);
					AddToExistentWorkoutInputControls(hWnd);
				}
				else
				{
					DestroyAllWindows(hWnd);
					AddNewWorkoutInputControls(hWnd);
				}
			}
			else
			{
				DestroyAllWindows(hWnd);
				AddExerciseToWorkoutInputControls(hWnd);
			}
			return 0;
			break;
		}
		case ADD_SET_TO_EXERCISE:
		{
			const int bufferRepsLength = ::GetWindowTextLength(hRepsEdit) + 1;
			const int bufferWeightLength = ::GetWindowTextLength(hWeightEdit) + 1;
			reps.resize(bufferRepsLength);
			weight.resize(bufferWeightLength);
			GetWindowText(hRepsEdit, &reps[0], bufferRepsLength);
			GetWindowText(hWeightEdit, &weight[0], bufferWeightLength);
			reps.resize(bufferRepsLength - 1);
			weight.resize(bufferWeightLength - 1);

			const auto& Sets = App.getWorkout(workout).getExercise(exercise).getSets();
			size_t sets_t = App.getWorkout(workout).getExercise(exercise).getSets().size();
			if (sets_t >= 0)
			{
				App.getWorkout(workout).getExercise(exercise).addSet(sets_t + 1);
				App.getWorkout(workout).getExercise(exercise).addRepToSet(sets_t + 1, std::stoi(weight), std::stoi(reps));
			}
			if (sets_t < 0)
			{
				int negative_sets{};
				negative_sets = MessageBoxW(hWnd, L"This exercise encountered an error. (Negative sets)", L"Exit", MB_ABORTRETRYIGNORE | MB_ICONERROR);
				if (negative_sets == IDABORT)
				{
					DestroyWindow(hWnd);
				}
				if (negative_sets == IDRETRY)
				{
					DestroyAllWindows(hWnd);
					AddSetsToExerciseInputControls(hWnd, App.getWorkout(workout).getExercise(exercise));
				}
				if (negative_sets == IDIGNORE)
				{
					App.getWorkout(workout).getExercise(exercise).addSet(sets_t + 1);
					App.getWorkout(workout).getExercise(exercise).addRepToSet(sets_t + 1, std::stoi(weight), std::stoi(reps));
				}
			}
			return 0;
			break;
		}
		case WORKOUT_MENU2:
		{
			DestroyAllWindows(hWnd);
			DisplayWorkouts(hWnd);
			return 0;
			break;
		}
		return 0;
		break;
		}
	return 0;
	break;
	}
	case WM_CREATE:
	{
		//dark mode but it looks bad lol
		//BOOL USE_DARK_MODE = true;
		//BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
		//	hWnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
		//	&USE_DARK_MODE, sizeof(USE_DARK_MODE)));
		HDC hdc = GetDC(hWnd);
		hBrushLabel = NULL;
		AddMenus(hWnd);
		RECT rc = { 0 };
		GetClientRect(hWnd, &rc);
		SCROLLINFO si = { 0 };
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_ALL;
		si.nMin = 0;
		si.nMax = 30 * 99 + 21;
		si.nPage = (rc.bottom - rc.top);
		si.nPos = 0;
		si.nTrackPos = 0;
		SetScrollInfo(hWnd, SB_VERT, &si, true);
		controlsCreated = false;
		return 0;
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
		break;
	}
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = reinterpret_cast<HDC>(wParam);
		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkColor(hdcStatic, RGB(64, 64, 64));
		if (!hBrushLabel) hBrushLabel = CreateSolidBrush(RGB(64, 64, 64));
		return reinterpret_cast<LRESULT>(hBrushLabel);
		break;
	}
	case WM_CTLCOLOREDIT:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(255, 255, 255));
		SetBkColor(hdcStatic, RGB(64, 64, 64));
		return (INT_PTR)CreateSolidBrush(RGB(64, 64, 64));
		break;
	}
	case WM_VSCROLL:
	{
		auto action = LOWORD(wParam);
		HWND hScroll = (HWND)lParam;
		int pos = -1;
		if (action == SB_THUMBPOSITION || action == SB_THUMBTRACK) {
			pos = HIWORD(wParam);
		}
		else if (action == SB_LINEDOWN) {
			pos = g_scrollY + 30;
		}
		else if (action == SB_LINEUP) {
			pos = g_scrollY - 30;
		}
		if (pos == -1)
			break;
		SCROLLINFO si = { 0 };
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		si.nPos = pos;
		si.nTrackPos = 0;
		SetScrollInfo(hWnd, SB_VERT, &si, true);
		GetScrollInfo(hWnd, SB_VERT, &si);
		pos = si.nPos;
		POINT pt;
		pt.x = 0;
		pt.y = pos - g_scrollY;
		auto hdc = GetDC(hWnd);
		LPtoDP(hdc, &pt, 1);
		ReleaseDC(hWnd, hdc);
		ScrollWindow(hWnd, 0, -pt.y, NULL, NULL);
		g_scrollY = pos;
		return 0;
	}
	//case WM_SIZE:
	//{
	//	int newWidth = LOWORD(lParam);
	//	int newHeight = HIWORD(lParam);

	//	// Example:
	//	for (HWND& ctrlhandler : controlHandles)
	//	{
	//		SetWindowPos(ctrlhandler, NULL, newWidth - 150, newHeight - 50, 100, 30, SWP_NOZORDER);
	//	}
	//	// Adjust other controls here...
	//	break;
	//}
	default:
	{
		return DefWindowProcW(hWnd, msg, wParam, lParam);
		break;
	}
	}
}

void MainWindow::AddMenus(HWND hWnd)
{
	hMainMenu = CreateMenu();
	hWorkoutMenu = CreateMenu();
	hCalorieMenu = CreateMenu();
	hWeightMenu = CreateMenu();

	// WorkoutMenu
	AppendMenu(hWorkoutMenu, MF_STRING, WORKOUT_MENU1, L"Create a Workout");
	AppendMenu(hWorkoutMenu, MF_STRING, WORKOUT_MENU2, L"Display Workouts");
	AppendMenu(hMainMenu, MF_POPUP, (UINT_PTR)hWorkoutMenu, L"Workout Manager");

	// CalorieMenu
	AppendMenu(hCalorieMenu, MF_STRING, CALORIE_MENU1, L"Add a meal");
	AppendMenu(hCalorieMenu, MF_STRING, CALORIE_MENU2, L"Display calories");
	AppendMenu(hMainMenu, MF_POPUP, (UINT_PTR)hCalorieMenu, L"Calorie Tracker");

	// WeightMenu
	AppendMenu(hWeightMenu, MF_STRING, WEIGHT_MENU1, L"Set your weight today");
	AppendMenu(hWeightMenu, MF_STRING, WEIGHT_MENU2, L"See your progress");
	AppendMenu(hMainMenu, MF_POPUP, (UINT_PTR)hWeightMenu, L"Weight Tracker");

	//About and profile menu
	AppendMenu(hMainMenu, MF_STRING, PROFILE_MENU, L"Profile");
	AppendMenu(hMainMenu, MF_STRING, ABOUT_MENU, L"About");

	SetMenu(hWnd, hMainMenu);

	//hBrushLabel = CreateSolidBrush(RGB(0, 0, 0));
	//MENUINFO mi = { 0 };
	//mi.cbSize = sizeof(mi);
	//mi.fMask = MIM_BACKGROUND | MIM_APPLYTOSUBMENUS;
	//mi.hbrBack = hBrushLabel;

	//SetMenuInfo(hMainMenu, &mi);
	//SetMenuInfo(hWorkoutMenu, &mi);
	//SetMenuInfo(hCalorieMenu, &mi);
	//SetMenuInfo(hWeightMenu, &mi);

	//SetTextColor((HDC)hMainMenu, RGB(255, 255, 255));
	//SetBkColor((HDC)hMainMenu, RGB(0, 0, 0));

	//SetTextColor((HDC)hWorkoutMenu, RGB(255, 255, 255));
	//SetBkColor((HDC)hWorkoutMenu, RGB(0, 0, 0));

	//SetTextColor((HDC)hCalorieMenu, RGB(255, 255, 255));
	//SetBkColor((HDC)hCalorieMenu, RGB(0, 0, 0));

	//SetTextColor((HDC)hWeightMenu, RGB(255, 255, 255));
	//SetBkColor((HDC)hWeightMenu, RGB(0, 0, 0));
}

void MainWindow::AddWeightInputControls(HWND hWnd)
{
	hAddWeightHelper = CreateWindowW(L"STATIC", L"Enter weight here: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 120, 100, 120, 80, hWnd, NULL, NULL, NULL);
	hAddWeight = CreateWindowW(L"EDIT", L"E.G 75", WS_VISIBLE | WS_CHILD, 270, 100, 100, 50, hWnd, NULL, NULL, NULL);
	hWeightButton = CreateWindowW(L"Button", L"Add Weight!", WS_VISIBLE | WS_CHILD, 200, 204, 100, 50, hWnd, (HMENU)(IDC_ADD_WEIGHT), NULL, NULL);

	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(12);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	SendMessage(hAddWeightHelper, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hAddWeight, WM_SETFONT, (WPARAM)hFont, TRUE);

	controlHandles.push_back(hAddWeight);
	controlHandles.push_back(hAddWeightHelper);
	controlHandles.push_back(hWeightButton);
}

void MainWindow::DisplayWeightControl(HWND hWnd)
{
	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(12);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);
	int X1 = 10;
	int X2 = 200;
	int YIncrement = 10;
	hWeightProgress = CreateWindowW(L"STATIC", L"Weight Diary:", WS_VISIBLE | WS_CHILD | SS_CENTER, X1+100, YIncrement, 120, 80, hWnd, NULL, NULL, NULL);

	SendMessage(hWeightProgress, WM_SETFONT, (WPARAM)hFont, TRUE);
	controlHandles.push_back(hWeightProgress);


	for (size_t i = 0; i < App.getDates().size(); ++i)
	{
		double weight = App.getDates().at(i).GetWeight();
		std::wstringstream weightStream;
		weightStream << std::fixed << std::setprecision(2) << weight;
		std::wstring weightString = weightStream.str();

		// Increase Y increment for workout title
		YIncrement += 60;

		auto hPWeightHelper = CreateWindowW(L"STATIC", App.getDates().at(i).date.c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, X1, YIncrement, 100, 50, hWnd, NULL, pInst, NULL);
		auto hPWeight = CreateWindowW(L"STATIC", weightString.c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, X2, YIncrement, 100, 50, hWnd, NULL, pInst, NULL);

		SendMessage(hPWeightHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hPWeight, WM_SETFONT, (WPARAM)hFont, TRUE);
		controlHandles.push_back(hPWeightHelper);
		controlHandles.push_back(hPWeight);
	}
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}

void MainWindow::CreateSpecificFont(HFONT& hFont, std::wstring& Font, HWND hWnd, long& fontHeight)
{
	if (Font == L"Arial")
	{
		hFont = CreateFont
		(
			fontHeight,         // Font height
			0,                // Default font width
			0,                // No escapement
			0,                // No orientation
			FW_BOLD,          // Bold weight
			FALSE,             // Italic
			FALSE,            // No underline
			FALSE,            // No strikeout
			DEFAULT_CHARSET,  // Default character set
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE,
			L"Arial"
		);
	}
}

Date& MainWindow::getToday()
{
	return App.getDateObj(TodaysDate);
}

void MainWindow::AddCalorieInputControls(HWND hWnd)
{
	hCalorieHelper = CreateWindowW(L"Static", L"Add calories today? Go right! Some other day? Go left!", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 10, 400, 80, hWnd, NULL, NULL, NULL);

	hAddCalorieTodayHelper = CreateWindowW(L"STATIC", L"Enter calories below!", WS_VISIBLE | WS_CHILD | SS_CENTER, 250, 100, 140, 20, hWnd, NULL, NULL, NULL);
	hCalorieToday = CreateWindowW(L"EDIT", L"E.G 75", WS_VISIBLE | WS_CHILD, 250, 152, 100, 20, hWnd, NULL, NULL, NULL);
	hCalorieButtonToday = CreateWindowW(L"Button", L"Add Calories Today!", WS_VISIBLE | WS_CHILD, 250, 204, 150, 20, hWnd, (HMENU)(IDC_ADD_CALORIES_TODAY), NULL, NULL);

	hAddCalorieDay = CreateWindowW(L"EDIT", L"E.G 24/08/2000", WS_VISIBLE | WS_CHILD, 10, 152, 120, 20, hWnd, NULL, NULL, NULL);
	hAddCalorieDayHelper = CreateWindowW(L"STATIC", L"Enter the date below!", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 100, 140, 20, hWnd, NULL, NULL, NULL);

	hAddCalorieHelper = CreateWindowW(L"STATIC", L"Enter calories below!", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 204, 140, 20, hWnd, NULL, NULL, NULL);
	hCalorie = CreateWindowW(L"EDIT", L"E.G 300", WS_VISIBLE | WS_CHILD, 10, 256, 120, 20, hWnd, NULL, NULL, NULL);
	hCalorieButton = CreateWindowW(L"Button", L"Add Calories!", WS_VISIBLE | WS_CHILD, 10, 308, 120, 20, hWnd, (HMENU)(IDC_ADD_CALORIES_SOMEOTHERDAY), NULL, NULL);

	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	SendMessage(hCalorieHelper, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hAddCalorieTodayHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hCalorieToday, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hCalorieButtonToday, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hAddCalorieDay, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hAddCalorieDayHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hAddCalorieHelper, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hCalorie, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hCalorieButton, WM_SETFONT, (WPARAM)hFont, TRUE);


	controlHandles.push_back(hCalorieHelper);
	controlHandles.push_back(hAddCalorieTodayHelper);
	controlHandles.push_back(hCalorieToday);
	controlHandles.push_back(hCalorieButtonToday);
	controlHandles.push_back(hAddCalorieDay);
	controlHandles.push_back(hAddCalorieDayHelper);
	controlHandles.push_back(hAddCalorieHelper);
	controlHandles.push_back(hCalorie);
	controlHandles.push_back(hCalorieButton);
}

void MainWindow::DisplayCalorieControl(HWND hWnd)
{
	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	for (size_t i = 0; i < App.getDates().size(); ++i) {
		auto hStatic1 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 10, 30 * i, 250, 21, hWnd, NULL, pInst, NULL);
		wchar_t buf1[15];
		StringCchPrintf(buf1, 15, L"%s", App.getDates().at(i).date.c_str());
		SetWindowText(hStatic1, buf1);

		auto hStatic2 = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 150, 30 * i, 250, 21, hWnd, NULL, pInst, NULL);
		wchar_t buf2[15];
		StringCchPrintf(buf2, 15, L"%s", std::to_wstring(App.getDates().at(i).GoThroughMealsReturnCalories()).c_str());
		SetWindowText(hStatic2, buf2);

		SendMessage(hStatic1, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
		SendMessage(hStatic2, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
		controlHandles.push_back(hStatic1);
		controlHandles.push_back(hStatic2);

	}
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}

long MainWindow::FontSize(int fontSize)
{
	HDC hdc = GetDC(NULL);
	long fsize = -MulDiv(fontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
	ReleaseDC(NULL, hdc);

	return 	fsize;
}

void MainWindow::AddWorkoutChooseControls(HWND hWnd)
{
	hAddWorkoutHelper = CreateWindowW(L"STATIC", L"What would you like to do?", WS_VISIBLE | WS_CHILD | SS_CENTER, 170, 10, 200, 20, hWnd, NULL, NULL, NULL);

	hAddNewWorkoutButton = CreateWindowW(L"Button", L"Add a new workout.", WS_VISIBLE | WS_CHILD, 100, 50, 150, 20, hWnd, (HMENU)(IDC_ADD_NEW_WORKOUT), NULL, NULL);
	hAddExistentWorkoutButton = CreateWindowW(L"Button", L"Add to an existent workout.", WS_VISIBLE | WS_CHILD, 300, 50, 200, 20, hWnd, (HMENU)(IDC_ADD_EXISTENT_WORKOUT), NULL, NULL);

	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	SendMessage(hAddWorkoutHelper, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hAddNewWorkoutButton, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hAddExistentWorkoutButton, WM_SETFONT, (WPARAM)hFont, TRUE);

	controlHandles.push_back(hAddWorkoutHelper);
	controlHandles.push_back(hAddNewWorkoutButton);
	controlHandles.push_back(hAddExistentWorkoutButton);
}

void MainWindow::AddNewWorkoutInputControls(HWND hWnd)
{
	hAddNewWorkoutHelper = CreateWindowW(L"STATIC", L"Enter the workout name!", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 10, 200, 20, hWnd, NULL, NULL, NULL);
	hNewWorkout = CreateWindowW(L"EDIT", L"E.G Upper Body", WS_VISIBLE | WS_CHILD, 250, 10, 200, 30, hWnd, NULL, NULL, NULL);

	hNewWorkoutButton = CreateWindowW(L"Button", L"Add the workout!", WS_VISIBLE | WS_CHILD, 150, 50, 150, 20, hWnd, (HMENU)NEW_WORKOUT_VALIDATION , NULL, NULL);

	HFONT hFont{};
	std::wstring font{L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	SendMessage(hAddNewWorkoutHelper, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hNewWorkout, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hNewWorkoutButton, WM_SETFONT, (WPARAM)hFont, TRUE);

	controlHandles.push_back(hAddNewWorkoutHelper);
	controlHandles.push_back(hNewWorkout);
	controlHandles.push_back(hNewWorkoutButton);
}

void MainWindow::AddToExistentWorkoutInputControls(HWND hWnd)
{
	hAddExistentWorkoutHelper = CreateWindowW(L"STATIC", L"Enter the workout name that you want to add exercises to!", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 10, 200, 50, hWnd, NULL, NULL, NULL);
	hExistentWorkout = CreateWindowW(L"EDIT", L"E.G Upper Body", WS_VISIBLE | WS_CHILD, 400, 10, 200, 20, hWnd, NULL, NULL, NULL);

	hExistentWorkoutButton = CreateWindowW(L"Button", L"Add to workout!", WS_VISIBLE | WS_CHILD, 150, 100, 150, 20, hWnd, (HMENU)EXISTENT_WORKOUT_VALIDATION, NULL, NULL);

	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	SendMessage(hAddExistentWorkoutHelper, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hExistentWorkout, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hExistentWorkoutButton, WM_SETFONT, (WPARAM)hFont, TRUE);

	controlHandles.push_back(hAddExistentWorkoutHelper);
	controlHandles.push_back(hExistentWorkout);
	controlHandles.push_back(hExistentWorkoutButton);
}


void MainWindow::AddExerciseToWorkoutInputControls(HWND hWnd)
{
	hNEWorkout = CreateWindowW(L"STATIC", L"Workout: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 10, 200, 20, hWnd, NULL, NULL, NULL);
	hNEWorkoutDisplay = CreateWindowW(L"STATIC", App.getWorkout(workout).getName().c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 250, 10, 200, 20, hWnd, NULL, NULL, NULL);

	hNEWExerciseHelper = CreateWindowW(L"STATIC", L"Enter an exercise name to add!", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 50, 200, 50, hWnd, NULL, NULL, NULL);
	hNEWExerciseEdit = CreateWindowW(L"EDIT", L"E.G Barbell Chest Press", WS_VISIBLE | WS_CHILD, 250, 50, 200, 50, hWnd, NULL, NULL, NULL);

	hNEWExerciseButton = CreateWindowW(L"Button", L"Add the exercise", WS_VISIBLE | WS_CHILD, 150, 150, 150, 20, hWnd, (HMENU)ADD_EXERCISE_TO_WORKOUT_VALIDATION, NULL, NULL);

	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	SendMessage(hNEWorkout, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hNEWorkoutDisplay, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hNEWExerciseHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hNEWExerciseEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hNEWExerciseButton, WM_SETFONT, (WPARAM)hFont, TRUE);

	controlHandles.push_back(hNEWorkout);
	controlHandles.push_back(hNEWorkoutDisplay);
	controlHandles.push_back(hNEWExerciseHelper);
	controlHandles.push_back(hNEWExerciseEdit);
	controlHandles.push_back(hNEWExerciseButton);
}

void MainWindow::AddSetsToExerciseInputControls(HWND hWnd, Exercise& exercise)
{
	hSetsHelper = CreateWindowW(L"STATIC", L"Add a new set!", WS_VISIBLE | WS_CHILD | SS_CENTER, 250, 10, 100, 50, hWnd, NULL, NULL, NULL);

	hRepsToSetHelper = CreateWindowW(L"STATIC", L"Add the amount of reps for the set!", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 60, 250, 50, hWnd, NULL, NULL, NULL);
	hRepsEdit = CreateWindowW(L"EDIT", L"E.G 12", WS_VISIBLE | WS_CHILD, 300, 60, 50, 50, hWnd, NULL, NULL, NULL);

	hWeightToSetHelper = CreateWindowW(L"STATIC", L"Add the weight to lift!", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 110, 250, 50, hWnd, NULL, NULL, NULL);
	hWeightEdit = CreateWindowW(L"EDIT", L"E.G 50", WS_VISIBLE | WS_CHILD, 300, 110, 50, 50, hWnd, NULL, NULL, NULL);

	hSetsButton = CreateWindowW(L"Button", L"Add the set!", WS_VISIBLE | WS_CHILD, 250, 250, 150, 30, hWnd, (HMENU)ADD_SET_TO_EXERCISE, NULL, NULL);

	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	SendMessage(hSetsHelper, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
	SendMessage(hRepsToSetHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hRepsEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hWeightToSetHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hWeightEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hSetsButton, WM_SETFONT, (WPARAM)hFont, TRUE);

	controlHandles.push_back(hSetsHelper);
	controlHandles.push_back(hRepsToSetHelper);
	controlHandles.push_back(hRepsEdit);
	controlHandles.push_back(hWeightToSetHelper);
	controlHandles.push_back(hWeightEdit);
	controlHandles.push_back(hSetsButton);
}


int MainWindow::DisplayWorkouts(HWND hWnd)
{
	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);

	int X1 = 0;
	int X2 = 120;
	int Y = 30;
	int Width = 250;
	int Height = 50;
	int YIncrement = 10; // Separate Y increment
	int totalHeight = 0;

	for (size_t i = 0; i < App.getWorkouts().size(); ++i)
	{
		// Reset X1 and X2 for each workout
		X1 = 0;
		X2 = 120;

		// Increase Y increment for workout title
		YIncrement += 20;

		auto hDisplayWorkoutHelper = CreateWindowW(L"STATIC", L"Workout: ", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X1, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);
		auto hDisplayWorkout = CreateWindowW(L"STATIC", App.getWorkouts().at(i).name.c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X2, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);

		SendMessage(hDisplayWorkoutHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(hDisplayWorkout, WM_SETFONT, (WPARAM)hFont, TRUE);
		controlHandles.push_back(hDisplayWorkoutHelper);
		controlHandles.push_back(hDisplayWorkout);

		totalHeight += 20;

		for (size_t j = 0; j < App.getWorkouts().at(i).getExercises().size(); j++)
		{
			YIncrement += 60; // Increase Y increment

			auto hDisplayExerciseHelper = CreateWindowW(L"STATIC", L"Exercise: ", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X1, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);
			auto hDisplayExercise = CreateWindowW(L"STATIC", App.getWorkouts().at(i).getExercises().at(j).GetName().c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X2, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);

			SendMessage(hDisplayExerciseHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
			SendMessage(hDisplayExercise, WM_SETFONT, (WPARAM)hFont, TRUE);
			controlHandles.push_back(hDisplayExerciseHelper);
			controlHandles.push_back(hDisplayExercise);

			totalHeight += 20;

			for (size_t k = 0; k < App.getWorkouts().at(i).getExercises().at(j).getSets().size(); k++)
			{
				YIncrement += 40; // Increase Y increment

				auto hDisplaySetHelper = CreateWindowW(L"STATIC", L"Set: ", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X1, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);
				auto hDisplaySet = CreateWindowW(L"STATIC", std::to_wstring(App.getWorkouts().at(i).getExercises().at(j).getSets().at(k).getId()).c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X2, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);

				SendMessage(hDisplaySetHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
				SendMessage(hDisplaySet, WM_SETFONT, (WPARAM)hFont, TRUE);
				controlHandles.push_back(hDisplaySetHelper);
				controlHandles.push_back(hDisplaySet);

				totalHeight += 20;

				for (size_t q = 0; q < App.getWorkouts().at(i).getExercises().at(j).getSets().at(k).getReps().size(); q++)
				{
					YIncrement += 20; // Increase Y increment

					auto hDisplayRepHelper = CreateWindowW(L"STATIC", L"Reps", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X1, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);
					auto hDisplayRep = CreateWindowW(L"STATIC", std::to_wstring(App.getWorkouts().at(i).getExercises().at(j).getSets().at(k).getReps().at(q).getCount()).c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X2, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);
					YIncrement += 20;
					auto hDisplayWeightHelper = CreateWindowW(L"STATIC", L"Weight (KG): ", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X1, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);
					auto hDisplayWeight = CreateWindowW(L"STATIC", std::to_wstring(App.getWorkouts().at(i).getExercises().at(j).getSets().at(k).getReps().at(q).getWeight()).c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, X2, YIncrement, Width, Height, hWnd, NULL, pInst, NULL);

					SendMessage(hDisplayRepHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
					SendMessage(hDisplayRep, WM_SETFONT, (WPARAM)hFont, TRUE);
					SendMessage(hDisplayWeightHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
					SendMessage(hDisplayWeight, WM_SETFONT, (WPARAM)hFont, TRUE);
					controlHandles.push_back(hDisplayRepHelper);
					controlHandles.push_back(hDisplayRep);
					controlHandles.push_back(hDisplayWeightHelper);
					controlHandles.push_back(hDisplayWeight);

					totalHeight += 20;
				}
			}
		}
		YIncrement += 80;
	}
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
	return totalHeight;
}



void MainWindow::AddStartMenuControls(HWND hWnd)
{
	hStart = CreateWindowW(L"STATIC", L"GymTracker", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 10, 500, 500, hWnd, NULL, NULL, NULL);
	hMade = CreateWindowW(L"STATIC", L"Made by Diogo Manteu", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 200, 500, 500, hWnd, NULL, NULL, NULL);

	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(50);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);
	SendMessage(hStart, WM_SETFONT, (WPARAM)hFont, TRUE);

	HFONT hFont1{};
	long FontHeight1 = FontSize(10);
	CreateSpecificFont(hFont1, font, hWnd, FontHeight1);
	SendMessage(hMade, WM_SETFONT, (WPARAM)hFont1, TRUE);

	controlHandles.push_back(hStart);
	controlHandles.push_back(hMade);



	
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}

void MainWindow::DestroyAllWindows(HWND hWnd)
{
	for (HWND handle : controlHandles)
	{
		DestroyWindow(handle);
	}
}

void MainWindow::AddAboutMenuControls(HWND hWnd)
{
		hNotice = CreateWindowW(L"STATIC", L"Copyright (C) 2023 Diogo Manteu - All Rights Reserved* You may use, distribute and modify this code or application under the terms of the Boost Software License - Version 1.0 - August 17th, 2003 license. You should have received a copy of the Boost Software License with this file. If not, please visit: https://www.boost.org/users/license.html", WS_VISIBLE | WS_CHILD | SS_CENTER, 10, 10, 500, 500, hWnd, NULL, NULL, NULL);

		HFONT hFont{};
		std::wstring font{ L"Arial" };
		long FontHeight = FontSize(10);
		CreateSpecificFont(hFont, font, hWnd, FontHeight);
		SendMessage(hNotice, WM_SETFONT, (WPARAM)hFont, TRUE);

		controlHandles.push_back(hNotice);
}

void MainWindow::AddProfileMenuControls(HWND hWnd)
{
	HFONT hFont{};
	std::wstring font{ L"Arial" };
	long FontHeight = FontSize(10);
	CreateSpecificFont(hFont, font, hWnd, FontHeight);
	int X1 = 10;
	int X2 = 200;

	int Y1 = 10;
	int Y2 = 70;
	int Y3 = 130;
	int Y4 = 190;
	int YIncrement = 250;


	hNumberWorkoutsHelper = CreateWindowW(L"STATIC", L"Workouts:", WS_VISIBLE | WS_CHILD | SS_CENTER, X1, Y1, 100, 50, hWnd, NULL, NULL, NULL);
	hNumberWorkouts = CreateWindowW(L"STATIC", std::to_wstring(App.getWorkouts().size()).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, X2, Y1, 100, 50, hWnd, NULL, NULL, NULL);

	hVolumeHelper = CreateWindowW(L"STATIC", L"Total Volume(KG)", WS_VISIBLE | WS_CHILD | SS_CENTER, X1, Y2, 100, 50, hWnd, NULL, NULL, NULL);
	hVolumeH = CreateWindowW(L"STATIC", std::to_wstring(App.getVolume()).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, X2, Y2, 100, 50, hWnd, NULL, NULL, NULL);

	hRepsHelper = CreateWindowW(L"STATIC", L"Total reps:", WS_VISIBLE | WS_CHILD | SS_CENTER, X1, Y3, 100, 50, hWnd, NULL, NULL, NULL);
	hReps = CreateWindowW(L"STATIC", std::to_wstring(App.getReps()).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, X2, Y3, 100, 50, hWnd, NULL, NULL, NULL);

	hCalsHelper = CreateWindowW(L"STATIC", L"Total calories:", WS_VISIBLE | WS_CHILD | SS_CENTER, X1, Y4, 100, 50, hWnd, NULL, NULL, NULL);
	hCals = CreateWindowW(L"STATIC", std::to_wstring(App.getCals()).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, X2, Y4, 100, 50, hWnd, NULL, NULL, NULL);


	std::optional<double> weight = getToday().GetWeight();
	if (weight.has_value())
	{
		double actualWeight = weight.value();
		std::wstringstream weightStream;
		weightStream << std::fixed << std::setprecision(2) << actualWeight;
		std::wstring weightString = weightStream.str();

		hDisplayWeight = CreateWindowW(L"Static", weightString.c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, X2, YIncrement, 100, 50, hWnd, NULL, NULL, NULL);
		hDisplayWeightHelper = CreateWindowW(L"Static", L"Your current weight: ", WS_VISIBLE | WS_CHILD | SS_CENTER, X1, YIncrement, 120, 80, hWnd, NULL, NULL, NULL);

		SendMessage(hDisplayWeight, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
		SendMessage(hDisplayWeightHelper, WM_SETFONT, (WPARAM)hFont, TRUE);


		controlHandles.push_back(hDisplayWeight);
		controlHandles.push_back(hDisplayWeightHelper);
	}
	else
	{
		hDisplayWeight = CreateWindowW(L"Static", L"No weight added.", WS_VISIBLE | WS_CHILD | SS_CENTER, X2, YIncrement, 100, 50, hWnd, NULL, NULL, NULL);
		hDisplayWeightHelper = CreateWindowW(L"Static", L"Your current weight: ", WS_VISIBLE | WS_CHILD | SS_CENTER, X1, YIncrement, 120, 80, hWnd, NULL, NULL, NULL);

		SendMessage(hDisplayWeight, WM_SETFONT, (WPARAM)hFont, TRUE); // Apply the new font
		SendMessage(hDisplayWeightHelper, WM_SETFONT, (WPARAM)hFont, TRUE);


		controlHandles.push_back(hDisplayWeight);
		controlHandles.push_back(hDisplayWeightHelper);
	}



	SendMessage(hNumberWorkoutsHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hNumberWorkouts, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hVolumeHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hVolumeH, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hRepsHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hReps, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hCalsHelper, WM_SETFONT, (WPARAM)hFont, TRUE);
	SendMessage(hCals, WM_SETFONT, (WPARAM)hFont, TRUE);
	controlHandles.push_back(hNumberWorkoutsHelper);
	controlHandles.push_back(hNumberWorkouts);
	controlHandles.push_back(hVolumeHelper);
	controlHandles.push_back(hVolumeH);
	controlHandles.push_back(hRepsHelper);
	controlHandles.push_back(hReps);
	controlHandles.push_back(hCalsHelper);
	controlHandles.push_back(hCals);

	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}
