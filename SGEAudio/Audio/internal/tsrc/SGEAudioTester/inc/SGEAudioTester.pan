
#ifndef SGEAUDIOTESTER_PAN_H
#define SGEAUDIOTESTER_PAN_H

/** SGEAudioTester application panic codes */
enum TSGEAudioTesterPanics
	{
	ESGEAudioTesterUi = 1
	// add further panics here
	};

inline void Panic(TSGEAudioTesterPanics aReason)
	{
	_LIT(applicationName,"SGEAudioTester");
	User::Panic(applicationName, aReason);
	}

#endif // SGEAUDIOTESTER_PAN_H
