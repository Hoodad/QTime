#include "QTime.h"
#include <windows.h>

// Set at compile time and never changed during runtime
float QTime::MAXIMUM_DELTA_TIME = 1.0f;
float QTime::DEFAULT_EXCEEDED_DELTA_MAX = 0.01666666f;

__int64 QTime::cyclesPerSec = 0;
__int64 QTime::externalTime = 0;
__int64 QTime::internalTime = 0;
__int64 QTime::previousTimeStamp = 0;
float QTime::deltaTime = 0.0f;
float QTime::externalDeltaTime = 0.0f;
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
	deltaTime = 0;

	QueryPerformance(&currentTimeStamp);
	cyclesSinceLastFrame = currentTimeStamp - previousTimeStamp;
	previousTimeStamp = currentTimeStamp;

	HandleExternalTimeUpdate(cyclesSinceLastFrame);

	if(IsInternalTimePaused() == false){
		HandleInternalTimeUpdate(cyclesSinceLastFrame);
	}
}

void QTime::Reset(){
	cyclesPerSec = 0;
	externalTime = 0;
	internalTime = 0;
	previousTimeStamp = 0;
	deltaTime = 0.0f;
	externalDeltaTime = 0.0f;
	timeScale = 1.0f;
	isPaused = false;
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

float QTime::GetExternalDT(){
	return externalDeltaTime;
}

void QTime::SetInternalTimeScale( const float p_value )
{
	timeScale = p_value;
}

bool QTime::IsInternalTimePaused()
{
	return isPaused;
}

void QTime::StepOneFrameInternalTime()
{
	if(IsInternalTimePaused()){
		__int64 cycles = SecondsToCycles(DEFAULT_EXCEEDED_DELTA_MAX);
		HandleInternalTimeUpdate(cycles);
	}
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

void QTime::HandleExternalTimeUpdate(__int64& p_cyclesSinceLastFrame){
	externalTime += p_cyclesSinceLastFrame;
	externalDeltaTime = (static_cast<float>(p_cyclesSinceLastFrame) / 
		static_cast<float>(cyclesPerSec));

	CorrectDeltaTime(externalDeltaTime);
}

void QTime::HandleInternalTimeUpdate(__int64& p_cyclesSinceLastFrame){
	internalTime += 
		static_cast<__int64>(static_cast<float>(p_cyclesSinceLastFrame) * timeScale);

	deltaTime = ( static_cast<float>(p_cyclesSinceLastFrame) / 
		static_cast<float>(cyclesPerSec) 
		) * timeScale;

	CorrectDeltaTime(deltaTime);
}

void QTime::CorrectDeltaTime( float& p_value )
{
	if(p_value > MAXIMUM_DELTA_TIME){
		p_value = DEFAULT_EXCEEDED_DELTA_MAX;
	}
}

float QTime::GetDEFAULT_EXCEEDED_DELTA_MAX()
{
	return DEFAULT_EXCEEDED_DELTA_MAX;
}
