#ifndef SPIADS127X_ANALYZER_SETTINGS
#define SPIADS127X_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class SPIADS127xAnalyzerSettings : public AnalyzerSettings
{
public:
	SPIADS127xAnalyzerSettings();
	virtual ~SPIADS127xAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	Channel Channel_SCLK;	// the SPI serial clock, sample data on rising edge
	Channel Channel_DATA;	// the SPI DATA -> DATA1 pin, MSB -- channel 0 first, 24 bits * 8 channels
	Channel Channel_DRDY;	// data ready signal, falling edge 
	double bits;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mSCLKChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mDATAChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mDRDYChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceNumberList >	mBitsNumberInterface;

private:
	void AddAllChannels(bool is_used_flag);
};

#endif //SPIADS127X_ANALYZER_SETTINGS
