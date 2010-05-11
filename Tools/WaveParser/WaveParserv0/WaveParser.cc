#include "WaveParser.hh"

WaveParser::WaveParser(const string& fileName)
{
	if(this->openWaveFile(fileName))
	{
		if(this->retrieveStandardHeader())
		{
			sampleRateM = WAV.sampleRate;
			numChannelsM = WAV.numChannels;
			bitsPerSampleM = WAV.bitsPerSample;
		}

		if(this->findDataSubChunk())
		{
			if(this->retrieveDataSubChunkInfo())
			{
				dataSubChunkSizeM = DINFO.dataSubChunkSize;
				offsetFromBeginningM = DINFO.offsetFromBeginning;
			}
		}
	}
	else
	{	
		throw string("There was an error when openning the file");
	}
}

WaveParser::~WaveParser()
{
	fclose(waveFileM);
}

bool WaveParser::openWaveFile(const string& fileName)
{
	this->waveFileM = fopen(fileName.c_str(), "rb");
     
     	if(waveFileM != NULL)
	{
		cout << "openWaveFile: File is OK" << endl;
		return true;
	}
	else
	{
		cout << "openWaveFile: File is NOT OK" << endl;
		return false;
	}
}

bool WaveParser::retrieveStandardHeader()
{
	memset(&WAV,0,sizeof(WAVEFORMATEX));

	if (fread(&WAV, sizeof(WAVEFORMATEX), 1, waveFileM) == 1)
	{
		cout <<  "retrieveStandardHeader: Reading was OK" << endl;
		return true;
	}
	else
	{
		if(ferror(waveFileM) > 0)
			cout << "retrieveStandardHeader: Error reading the file" << endl;
		if(feof(waveFileM) > 0)
			cout << "retrieveStandardHeader: End of file reached" << endl;

		return false;
	}
}

bool WaveParser::findDataSubChunk()
{
	char buffer[5];

	if (fread(buffer, sizeof(char), 4, waveFileM) != 4)
	{
		cout <<  "findDataSubChunk: Reading was NOT OK" << endl;
		return false;
	}

	buffer[4] = '\0';

	while(strcmp(buffer, "data") && !ferror(waveFileM) && !feof(waveFileM))
	{
		for(int i = 0; i < 3; i++)
			buffer[i] = buffer[i+1];

		if (fread(&buffer[3], sizeof(char), 1, waveFileM) != 1)
		{
			cout <<  "findDataSubChunk: Reading was NOT OK" << endl;
			return false;
		}	
	}

	if(!ferror(waveFileM) && !feof(waveFileM))
		return true;
	else
		return false;
}

bool WaveParser::retrieveDataSubChunkInfo()
{
	memset(&DINFO,0,sizeof(DATACHUNKINFO));

	if (fread(&DINFO, sizeof(DWORD), 1, waveFileM) == 1)
	{
		DINFO.offsetFromBeginning = ftell(waveFileM);

		cout <<  "retrieveDataSubChunkInfo: Reading was OK" << endl;
		return true;
	}
	else
	{
		cout << "retrieveDataSubChunkInfo: Reading was NOT OK" << endl;
		return false;
	}
}

void WaveParser::getStandardHeader(WAVEFORMATEX *WAV)
{
	*WAV = this->WAV;
}

int WaveParser::getNumberOfChannels()
{
	return numChannelsM;
}

int WaveParser::getSampleRate()
{
	return sampleRateM;
}

int WaveParser::getBytesPerSample()
{
	return bitsPerSampleM;
}

int WaveParser::getDataSubChunkSize()
{
	return dataSubChunkSizeM;
}

int WaveParser::getOffsetFromBeginning()
{
	return offsetFromBeginningM;
}

long WaveParser::getCurrentPosition()
{
	return ftell(waveFileM);
}

int WaveParser::getChannelData16Bits(int channelNo, int nOfSamples2Read, short* samplesBuffer)
{
	if((this->bitsPerSampleM/8) != sizeof(short))
	{
		cout << "Use the proper method!!!" << endl;
		return ERROR;
	}

	if(channelNo == 2)
		fseek(waveFileM, (this->bitsPerSampleM/8), SEEK_CUR);

	for(int i = 0; i < nOfSamples2Read; i++)
	{
		if (fread(&samplesBuffer[i], (this->bitsPerSampleM/8), 1, waveFileM) != 1)
		{
			cout << "getChannelData: Error" << endl;
			return ERROR;
		}
		fseek(waveFileM, (this->bitsPerSampleM/8), SEEK_CUR);
	}

	return nOfSamples2Read;
}

int WaveParser::getChannelData(int channelNo, int nOfSamples2Read, void* samplesBuffer, int offset)
{

}

void WaveParser::printWaveFileInfo()
{
	char chunkID[5];
	char format[5];
	char subChunk1ID[5];

	memcpy(chunkID, WAV.chunckID, 4);
	chunkID[4] = '\0';

	memcpy(format, WAV.format, 4);
	format[4] = '\0';

	memcpy(subChunk1ID, WAV.subChunk1ID, 4);
	subChunk1ID[4] = '\0';

	cout << "Chunck ID = " << chunkID << endl;
	cout << "Chunk Size = " << WAV.chunkSize << endl;
	cout << "Format = " << format << endl;
	cout << "Sub Chunk1 ID = " << subChunk1ID << endl;
	cout << "Sub Chunk1 Size = " << WAV.subChunk1Size << endl;

	if(WAV.audioFormat == 1)
		cout << "Audio Format = " << WAV.audioFormat << " (PCM)" << endl;
	else
		cout << "Audio Format = " << WAV.audioFormat << endl;

	if(WAV.numChannels == 1)
		cout << "Num Channels = " << WAV.numChannels << " (Mono)" << endl;
	else if(WAV.numChannels == 2)
		cout << "Num Channels = " << WAV.numChannels << " (Stereo)" << endl;
	else
		cout << "Num Channels = " << WAV.numChannels << endl;

	cout << "Sample Rate = " << WAV.sampleRate << " Hz" << endl;
	cout << "Byte Rate = " << WAV.byteRate << endl;
	cout << "Block Align = " << WAV.blockAlign << endl;
	cout <<	"Bits Per Sample = " << WAV.bitsPerSample << endl;

	cout << "Data SubChunck Size: " << DINFO.dataSubChunkSize << endl;
	cout << "Offset from Beginning to data: " << DINFO.offsetFromBeginning << endl; 
}

