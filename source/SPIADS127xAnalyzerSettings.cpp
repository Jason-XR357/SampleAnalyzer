#include "SPIADS127xAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


SPIADS127xAnalyzerSettings::SPIADS127xAnalyzerSettings()
:Channel_DATA( UNDEFINED_CHANNEL ),
Channel_SCLK( UNDEFINED_CHANNEL ),
Channel_DRDY( UNDEFINED_CHANNEL )
{
	bits = 16.0;

	mSCLKChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mSCLKChannelInterface->SetTitleAndTooltip( "SCLK", "SPI data clock" );
	mSCLKChannelInterface->SetChannel( Channel_SCLK );

	mDATAChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mDATAChannelInterface->SetTitleAndTooltip( "DATA", "DATA1 pin" );
	mDATAChannelInterface->SetChannel( Channel_DATA );

	mDRDYChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mDRDYChannelInterface->SetTitleAndTooltip( "DRDY", "Data Ready" );
	mDRDYChannelInterface->SetChannel( Channel_DRDY );

	mBitsNumberInterface.reset( new AnalyzerSettingInterfaceNumberList() );
	mBitsNumberInterface->SetTitleAndTooltip( "Data Width",  "Number of bits to decode" );
	mBitsNumberInterface->AddNumber( 16.0, "16 MSB" , "16 bit data");
	mBitsNumberInterface->AddNumber( 24.0, "24" , "All 24 bits");

	AddInterface( mSCLKChannelInterface.get() );
	AddInterface( mDATAChannelInterface.get() );
	AddInterface( mDRDYChannelInterface.get() );
	AddInterface( mBitsNumberInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	AddAllChannels(false);
}

SPIADS127xAnalyzerSettings::~SPIADS127xAnalyzerSettings()
{
}

void SPIADS127xAnalyzerSettings::AddAllChannels(bool is_used_flag)
{
	ClearChannels();
	AddChannel( Channel_SCLK, "SCLK", is_used_flag );
	AddChannel( Channel_DATA, "DATA", is_used_flag );
	AddChannel( Channel_DRDY, "DRDY", is_used_flag );
}

bool SPIADS127xAnalyzerSettings::SetSettingsFromInterfaces()
{
	Channel_SCLK = mSCLKChannelInterface->GetChannel();
	Channel_DATA = mDATAChannelInterface->GetChannel();
	Channel_DRDY = mDRDYChannelInterface->GetChannel();
	bits = mBitsNumberInterface->GetNumber();
//	mBitRate = mBitRateInterface->GetInteger();

	AddAllChannels(true);

	return true;
}

void SPIADS127xAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mSCLKChannelInterface->SetChannel( Channel_SCLK );
	mDATAChannelInterface->SetChannel( Channel_DATA );
	mDRDYChannelInterface->SetChannel( Channel_DRDY );
	mBitsNumberInterface->SetNumber( bits );
}

void SPIADS127xAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> Channel_SCLK;
	text_archive >> Channel_DATA;
	text_archive >> Channel_DRDY;
	text_archive >> bits;

	AddAllChannels(true);

	UpdateInterfacesFromSettings();
}

const char* SPIADS127xAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << Channel_SCLK;
	text_archive << Channel_DATA;
	text_archive << Channel_DRDY;
	text_archive << bits;

	return SetReturnString( text_archive.GetString() );
}
