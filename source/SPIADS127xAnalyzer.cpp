#include "SPIADS127xAnalyzer.h"
#include "SPIADS127xAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

SPIADS127xAnalyzer::SPIADS127xAnalyzer()
:	Analyzer2(),  
	mSettings( new SPIADS127xAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

SPIADS127xAnalyzer::~SPIADS127xAnalyzer()
{
	KillThread();
}

void SPIADS127xAnalyzer::SetupResults()
{
	mResults.reset( new SPIADS127xAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->Channel_DATA );
}

void SPIADS127xAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();
	U64 data_ready_reaction_samples;

	mSCLK = GetAnalyzerChannelData( mSettings->Channel_SCLK );
	mDATA = GetAnalyzerChannelData( mSettings->Channel_DATA );
	mDRDY = GetAnalyzerChannelData( mSettings->Channel_DRDY );

	if( mDRDY->GetBitState() == BIT_LOW )
		mDRDY->AdvanceToNextEdge();

	for(;;)
	{
		U32 data;

		mDRDY->AdvanceToNextEdge(); //falling edge
		mSCLK->AdvanceToAbsPosition(mDRDY->GetSampleNumber());
		data_ready_reaction_samples = (mDRDY->GetSampleNumber() - mSCLK->GetSampleOfNextEdge());
		mDRDY->AdvanceToNextEdge(); // RE 

		for(int a = 0; a < 8; a++)
		{
			U64 starting_sample = mSCLK->GetSampleOfNextEdge();
			data = 0;
			for( U32 i=23; i>=0; i-- )
			{
				mSCLK->AdvanceToNextEdge(); // RE 

				//let's put a dot exactly where we sample this bit:
				mResults->AddMarker( mSCLK->GetSampleNumber(), AnalyzerResults::DownArrow, mSettings->Channel_SCLK );
				mResults->AddMarker( mSCLK->GetSampleNumber(), AnalyzerResults::X, mSettings->Channel_DATA );
				mDATA->AdvanceToAbsPosition( mSCLK->GetSampleNumber() );
				if( mDATA->GetBitState() == BIT_HIGH )
					data |= (1 << i);

				mSCLK->AdvanceToNextEdge(); // FE
			}

			Frame frame;
			frame.mData1 = 0x55;
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = starting_sample;
			frame.mEndingSampleInclusive = mSCLK->GetSampleNumber();

			mResults->AddFrame( frame );
			mResults->CommitResults();
			ReportProgress( frame.mEndingSampleInclusive );
		}
		mDRDY->AdvanceToNextEdge();  // FE
		mDRDY->AdvanceToNextEdge();  // RE
	}
}

bool SPIADS127xAnalyzer::NeedsRerun()
{
	return false;
}

U32 SPIADS127xAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 SPIADS127xAnalyzer::GetMinimumSampleRateHz()
{
	return 3000000 * 4;
}

const char* SPIADS127xAnalyzer::GetAnalyzerName() const
{
	return "ADS127x";
}

const char* GetAnalyzerName()
{
	return "ADS127x";
}

Analyzer* CreateAnalyzer()
{
	return new SPIADS127xAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
