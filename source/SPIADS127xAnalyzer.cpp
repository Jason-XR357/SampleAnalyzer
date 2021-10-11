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
	U64 next_SCLK_samples;
	U64 next_drdy_samples;

	mSCLK = GetAnalyzerChannelData( mSettings->Channel_SCLK );
	mDATA = GetAnalyzerChannelData( mSettings->Channel_DATA );
	mDRDY = GetAnalyzerChannelData( mSettings->Channel_DRDY );

	if( mDRDY->GetBitState() == BIT_LOW )
		mDRDY->AdvanceToNextEdge();

	bool chop_8lsb = (mSettings->bits == 16);
	for(;;)
	{
		U32 data;

		mDRDY->AdvanceToNextEdge(); //falling edge
		mSCLK->AdvanceToAbsPosition(mDRDY->GetSampleNumber());
		next_SCLK_samples = mSCLK->GetSampleOfNextEdge();
		data_ready_reaction_samples = (next_SCLK_samples - mDRDY->GetSampleNumber());
		mDRDY->AdvanceToNextEdge(); // RE
		if( next_SCLK_samples > mDRDY->GetSampleNumber())
		{
			mResults->AddMarker( mDRDY->GetSampleNumber(), AnalyzerResults::ErrorX, mSettings->Channel_SCLK );
		}
		else
		{
			Frame frame;
			frame.mData1 = data_ready_reaction_samples;
			frame.mFlags = 1;
			frame.mStartingSampleInclusive = mSCLK->GetSampleNumber();
			frame.mEndingSampleInclusive = mDRDY->GetSampleNumber();
			mResults->AddFrame( frame );
			mResults->CommitResults();

			for(int a = 0; a < 8; a++)
			{
				U64 starting_sample = mSCLK->GetSampleOfNextEdge();
				data = 0;
				for( int i=23; i>=0; i-- )
				{
					mSCLK->AdvanceToNextEdge(); // RE 

					//let's put a dot exactly where we sample this bit:
					mResults->AddMarker( mSCLK->GetSampleNumber(), AnalyzerResults::UpArrow, mSettings->Channel_SCLK );
					mDATA->AdvanceToAbsPosition( mSCLK->GetSampleNumber() );
					if( mDATA->GetBitState() == BIT_HIGH )
					{
						data |= (1 << i);
						if(chop_8lsb && i < 8)
							mResults->AddMarker( mSCLK->GetSampleNumber(), AnalyzerResults::X, mSettings->Channel_DATA );
						else
							mResults->AddMarker( mSCLK->GetSampleNumber(), AnalyzerResults::One, mSettings->Channel_DATA );
					}
					else
					{
						if(chop_8lsb && i < 8)
							mResults->AddMarker( mSCLK->GetSampleNumber(), AnalyzerResults::X, mSettings->Channel_DATA );
						else
							mResults->AddMarker( mSCLK->GetSampleNumber(), AnalyzerResults::Zero, mSettings->Channel_DATA );
					}
					mSCLK->AdvanceToNextEdge(); // FE
				}

				if(mSettings->bits == 16)
					frame.mData1 = data >> 8;
				else
					frame.mData1 = data;
				frame.mFlags = 0;
				frame.mData2 = a;
				frame.mStartingSampleInclusive = starting_sample;
				frame.mEndingSampleInclusive = mSCLK->GetSampleNumber();

				mResults->AddFrame( frame );
				mResults->CommitResults();
				ReportProgress( frame.mEndingSampleInclusive );
			}
		}
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
