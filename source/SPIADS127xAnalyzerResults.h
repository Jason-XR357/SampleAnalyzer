#ifndef SPIADS127X_ANALYZER_RESULTS
#define SPIADS127X_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class SPIADS127xAnalyzer;
class SPIADS127xAnalyzerSettings;

class SPIADS127xAnalyzerResults : public AnalyzerResults
{
public:
	SPIADS127xAnalyzerResults( SPIADS127xAnalyzer* analyzer, SPIADS127xAnalyzerSettings* settings );
	virtual ~SPIADS127xAnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	SPIADS127xAnalyzerSettings* mSettings;
	SPIADS127xAnalyzer* mAnalyzer;
};

#endif //SPIADS127X_ANALYZER_RESULTS
