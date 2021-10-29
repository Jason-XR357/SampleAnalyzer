#include "SPIADS127xAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "SPIADS127xAnalyzer.h"
#include "SPIADS127xAnalyzerSettings.h"
#include <iostream>
#include <fstream>

SPIADS127xAnalyzerResults::SPIADS127xAnalyzerResults( SPIADS127xAnalyzer* analyzer, SPIADS127xAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

SPIADS127xAnalyzerResults::~SPIADS127xAnalyzerResults()
{
}

void SPIADS127xAnalyzerResults::format_number(DisplayBase display_base, U64 value, char *buf, int buf_len)
{
	if(mSettings->bits == 16.0)
	{
		if(display_base == Decimal)
			sprintf(buf, "%d", (signed short)value);
		else
			AnalyzerHelpers::GetNumberString( value, display_base, 16, buf, buf_len);
	}
	else
	{
		if(display_base == Decimal)
		{
			int tval;
			if(value & 0x800000)
				tval = 0xff000000 | value;
			else
				tval = value;
			sprintf(buf, "%d", (int)tval);
		}
		else
			AnalyzerHelpers::GetNumberString( value, display_base, 24, buf, buf_len);
	}
}

void SPIADS127xAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	if(channel == mSettings->Channel_DATA)
	{
		ClearResultStrings();
		Frame frame = GetFrame( frame_index );

		char number_str[128];
		if(frame.mFlags == 1)
		{
//			AnalyzerHelpers::GetTimeString( frame.mData1, 0, mAnalyzer->GetSampleRate(), number_str, 128 );
			sprintf(number_str, "%.8f", (double)frame.mData1 / mAnalyzer->GetSampleRate());
		}
		else
		{
			format_number(display_base, frame.mData1, number_str, sizeof(number_str));
		}
		AddResultString( number_str );
	}
}

void SPIADS127xAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s]";
	if(mSettings->export_timing)
		file_stream << ",delay";
	if(mSettings->export_data)
		file_stream << ",1,2,3,4,5,6,7,8";

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char number_str[128];

		if(frame.mFlags == 1)
		{
			char time_str[128];
			AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );
			file_stream << std::endl << time_str;
			if(mSettings->export_timing)
			{
//				AnalyzerHelpers::GetTimeString( frame.mData1, 0, sample_rate, number_str, 128 );
				sprintf(number_str, "%.8f", (double)frame.mData1 / mAnalyzer->GetSampleRate());
				file_stream << "," << number_str;
			}
		}
		else if(mSettings->export_data)
		{
			format_number(display_base, frame.mData1, number_str, sizeof(number_str));
			file_stream << "," << number_str;
		}
		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}
	file_stream << std::endl;
	file_stream.close();
}

void SPIADS127xAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
// 	Frame frame = GetFrame( frame_index );
// 	ClearTabularText();
// 
// 	char number_str[128];
// 	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 32, number_str, 128 );
// 	AddTabularText( number_str );
#endif
}

void SPIADS127xAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void SPIADS127xAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}
