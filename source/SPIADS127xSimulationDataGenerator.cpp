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
}

U32 SPIADS127xSimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	return 0;
}

void SPIADS127xSimulationDataGenerator::Create24ADCData(int32_t val)
{

}
