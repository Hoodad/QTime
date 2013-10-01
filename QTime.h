#pragma once
// =======================================================================================
//                                      QTime
// =======================================================================================

///---------------------------------------------------------------------------------------
/// \brief	Quantum Time is a simple static timer wrapping that can be used to accurately 
/// measure time differences in various Windows applications on the CPU. 
///        
/// # QTime
/// It makes use of the QueryPerformanceCounter to calculate time differences and uses 
/// __int64 to store these values. This results in turnover only happening once every 193 
/// year.
///
/// ExternalTime refers to time outside this application.
/// Where internalTime refers to the time spent running the application, taken into 
/// consideration that pause and timeScale will affect the internalTime but not 
/// externalTime.
///
/// Created on: 01-10-2013
/// Created by Robin Thunstroem
///--------------------------------------------------------------------------------------


class QTime{
private:
	// Used to check whether the system has been paused or not, viewed in seconds.
	static float MAXIMUM_DELTA_TIME;

	// Used only once the maximum delta time has been reached, viewed in seconds.
	static float DEFAULT_EXCEEDED_DELTA_MAX;

	static __int64 cyclesPerSec;

	static __int64 externalTime;
	static __int64 internalTime;

	static __int64 previousTimeStamp;

	static float deltaTime;

	static float timeScale;
	static bool isPaused;
public:
	// Should be called before any calls to update is made.
	static void Init();
	
	// Will have to be called every TICK to update the delta time and other variables.
	static void Update();

	// Pauses only the internal time
	static void PauseInteralTime( const bool p_value);

	// Returns the number of cycles since the program started. Cannot be paused
	static __int64 GetExternalTime();

	// Returns the number of cycles elapsed during the whole system has been running. 
	// Taken in consideration time paused and time scale.
	static __int64 GetInternalTime();

	// Gets the delta time calculated at the last call to the Update function
	static float GetDT();

	// Can be used to scale how fast time is perceived by the internal time
	static void SetInternalTimeScale(const float p_value);

	// Will only return true if the PauseSystemTime has been set to true
	static bool IsInternalTimePaused();

	// Help function that can calculate from CPU cycles how long that would be in seconds
	// CAUTION! Should only be used to calculate smaller time differences due to floats
	// lack of precision as the values increases.
	static float CyclesToSeconds(__int64 p_cycles);

	// Help function that converts seconds to cycles.
	static __int64 SecondsToCycles( float p_seconds);
private:
	static void QueryPerformance(__int64 *p_value);
};