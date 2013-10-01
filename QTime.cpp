#include "QTime.h"
#include "Helper.h"
#include <windows.h>

// Set at compile time and never changed during runtime
float QTime::MAXIMUM_DELTA_TIME = 1.0f;
float QTime::DEFAULT_EXCEEDED_DELTA_MAX = 1.0f/60.0f;

__int64 QTime::cyclesPerSec = 0;
__int64 QTime::externalTime = 0;
__int64 QTime::internalTime = 0;
__int64 QTime::previousTimeStamp = 0;
float QTime::deltaTime = 0.0f;
float QTime::timeScale = 1.0f;
bool QTime::isPaused = false;

void QTime::Init()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)& cyclesPerSec);
	QueryPerformance(&previousTimeStamp);
}

void QTime::Update( )
{
	__int64 cyclesSinceLastFrame;
	__int64 currentTimeStamp;

	QueryPerformance(&currentTimeStamp);
	cyclesSinceLastFrame = currentTimeStamp - previousTimeStamp;
	previousTimeStamp = currentTimeStamp;

	externalTime += cyclesSinceLastFrame;

	if(IsInternalTimePaused()){
		deltaTime = 0;
	}
	else{
		internalTime += cyclesSinceLastFrame;
		deltaTime = (
			static_cast<float>(cyclesSinceLastFrame) / static_cast<float>(cyclesPerSec)
			) * timeScale;

		if(deltaTime > MAXIMUM_DELTA_TIME){
			deltaTime = DEFAULT_EXCEEDED_DELTA_MAX;
		}
	}
}

void QTime::PauseInteralTime(const bool p_value){
	isPaused = p_value;
}

__int64 QTime::GetExternalTime() {
	return externalTime;
}

__int64 QTime::GetInternalTime()
{
	return internalTime;
}

float QTime::GetDT() {
	return deltaTime;
}

void QTime::SetInternalTimeScale( const float p_value )
{
	timeScale = p_value;
}

bool QTime::IsInternalTimePaused()
{
	return isPaused;
}

float QTime::CyclesToSeconds( __int64 p_cycles )
{
	return static_cast<float>(p_cycles) / static_cast<float>(cyclesPerSec);
}

__int64 QTime::SecondsToCycles( float p_seconds )
{
	return static_cast<__int64>( p_seconds * cyclesPerSec);
}

void QTime::QueryPerformance( __int64 *p_value ){
	QueryPerformanceCounter((LARGE_INTEGER*) p_value);
}