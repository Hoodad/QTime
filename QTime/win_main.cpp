#include "QTime.h"
#include <igloo\igloo_alt.h>

using namespace igloo;

Describe(A_test_of_time){

	It(Starts_with_checking_initiation){
		QTime::Init();
		Assert::That( QTime::GetDT(), Equals(0.0f) );
		Assert::That( QTime::GetExternalDT(), Equals(0.0f) );
		Assert::That( QTime::GetInternalTime(), Equals(0) );
		Assert::That( QTime::GetExternalTime(), Equals(0) );
	}

	Describe(More_Details){

		void SetUp(){
			QTime::Init();
			QTime::Update();
		}

		void TearDown(){
			QTime::Reset();
		}

		It(Updates_the_time){
			QTime::Update();
			Assert::That( QTime::GetDT(), IsGreaterThan(0.0f) );
			Assert::That( QTime::GetInternalTime(), IsGreaterThan(0) );
			Assert::That( QTime::GetExternalTime(), IsGreaterThan(0) );
		}

		It(Slows_down_time){
			QTime::SetInternalTimeScale(0.5f);
			QTime::Update();
			Assert::That( QTime::GetInternalTime(), IsLessThan(QTime::GetExternalTime()));
			Assert::That( QTime::GetDT(), IsLessThan(QTime::GetExternalDT()));
		}

		It(Speeds_up_time){
			QTime::SetInternalTimeScale(1.5f);
			QTime::Update();
			Assert::That( QTime::GetInternalTime(), IsGreaterThan(QTime::GetExternalTime()));
			Assert::That( QTime::GetDT(), IsGreaterThan(QTime::GetExternalDT()));
		}

		It(Pauses_time){
			__int64 internalTime = QTime::GetInternalTime();
			QTime::PauseInteralTime(true);
			QTime::Update();
			Assert::That( QTime::GetInternalTime(), Equals(internalTime) );
			Assert::That( QTime::GetExternalTime(), !Equals(QTime::GetInternalTime()) );
		}
		
		It(Single_step_time){
			QTime::PauseInteralTime(true);
			QTime::StepOneFrameInternalTime();
			float deltaTime = QTime::GetDT();
			float difference = deltaTime - QTime::GetDEFAULT_EXCEEDED_DELTA_MAX();
			Assert::That( difference, IsLessThan(0.000001f));
		}
	};
};

int main(int argCount, const char ** args){

	TestRunner::RunAllTests(argCount, args);
	std::cin.get();
	return 1;
}