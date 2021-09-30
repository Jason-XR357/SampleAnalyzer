#ifndef SPIADS127X_SIMULATION_DATA_GENERATOR
#define SPIADS127X_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class SPIADS127xAnalyzerSettings;

class SPIADS127xSimulationDataGenerator
{
public:
	SPIADS127xSimulationDataGenerator();
	~SPIADS127xSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, SPIADS127xAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	SPIADS127xAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void Create24ADCData(int32_t adc_raw_data);

	SimulationChannelDescriptor mSCLKData;
	SimulationChannelDescriptor mDataData;
	SimulationChannelDescriptor mDRDYData;
	int adc_output_channel;

};
#endif //SPIADS127X_SIMULATION_DATA_GENERATOR
