#ifndef SPIADS127X_ANALYZER_H
#define SPIADS127X_ANALYZER_H

#include <Analyzer.h>
#include "SPIADS127xAnalyzerResults.h"
#include "SPIADS127xSimulationDataGenerator.h"

class SPIADS127xAnalyzerSettings;
class ANALYZER_EXPORT SPIADS127xAnalyzer : public Analyzer2
{
public:
	SPIADS127xAnalyzer();
	virtual ~SPIADS127xAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< SPIADS127xAnalyzerSettings > mSettings;
	std::auto_ptr< SPIADS127xAnalyzerResults > mResults;
	AnalyzerChannelData* mSCLK;
	AnalyzerChannelData* mDATA;
	AnalyzerChannelData* mDRDY;

	SPIADS127xSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //SPIADS127X_ANALYZER_H
