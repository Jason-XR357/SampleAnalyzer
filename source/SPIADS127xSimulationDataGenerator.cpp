#include "SPIADS127xSimulationDataGenerator.h"
#include "SPIADS127xAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

SPIADS127xSimulationDataGenerator::SPIADS127xSimulationDataGenerator()
{
	adc_output_channel = 0;
}

SPIADS127xSimulationDataGenerator::~SPIADS127xSimulationDataGenerator()
{
}

void SPIADS127xSimulationDataGenerator::Initialize( U32 simulation_sample_rate, SPIADS127xAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mSCLKData.SetChannel( mSettings->Channel_SCLK );
	mSCLKData.SetSampleRate( simulation_sample_rate );
	mSCLKData.SetInitialBitState( BIT_LOW );

	mDataData.SetChannel( mSettings->Channel_DATA );
	mDataData.SetSampleRate( simulation_sample_rate );
	mDataData.SetInitialBitState( BIT_LOW );

	mDRDYData.SetChannel( mSettings->Channel_DRDY );
	mDRDYData.SetSampleRate( simulation_sample_rate );
	mDRDYData.SetInitialBitState( BIT_HIGH );
}

U32 SPIADS127xSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	if(mSettings->bits > 0)
	{
		while( mDataData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
		{
			Create24ADCData(12345 << 8);
		}
	}

	*simulation_channel = &mDataData;
	return 1;
}

void SPIADS127xSimulationDataGenerator::Create24ADCData(int32_t val)
{
	const U32 samples_per_sclk = mSimulationSampleRateHz / 3000000;

	if(adc_output_channel == 0)
	{
		mDRDYData.Advance( samples_per_sclk * 100 );
		mDataData.Advance( mDRDYData.GetCurrentSampleNumber() - mDataData.GetCurrentSampleNumber() );
		mSCLKData.Advance( mDRDYData.GetCurrentSampleNumber() - mSCLKData.GetCurrentSampleNumber() );
		mDRDYData.Transition();	// low
		mDRDYData.Advance( samples_per_sclk * 2.3 );
		mDRDYData.Transition();
		mDRDYData.Advance( samples_per_sclk * (24 * 8 * 2 - 2.3) );
		mSCLKData.Advance( samples_per_sclk / 2 );
	}

	int b = 24;
	while(b > 0)
	{
		b--;
		if((val >> b) & 1)
			mDataData.TransitionIfNeeded(BIT_HIGH);
		else
			mDataData.TransitionIfNeeded(BIT_LOW);
		mSCLKData.Transition();
		mSCLKData.Advance( samples_per_sclk );
		mSCLKData.Transition();
		mSCLKData.Advance( samples_per_sclk );
		mDataData.Advance( samples_per_sclk * 2 );
	}
}
