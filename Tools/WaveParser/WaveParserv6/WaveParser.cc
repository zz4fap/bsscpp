#include "WaveParser.hh"
 
WaveParser::WaveParser(const string& fileName)
{
	// Open the file and check if it really is a RIFF WAVE file?
	if(this->openWaveFile(fileName))
	{
		if(this->findID("RIFF") && this->findID("WAVE"))
		{
			// Go back to the beginning of the file
			fseek(waveFileM, 0, SEEK_SET);

			if(this->retrieveStandardHeader() && this->findID("data") && this->retrieveDataSubChunkInfo())
			{
				sampleRateM = WAV.sampleRate;
				numChannelsM = WAV.numChannels;
				bitsPerSampleM = WAV.bitsPerSample;
				dataSubChunkSizeM = DINFO.dataSubChunkSize;
				offsetFromBeginningM = DINFO.offsetFromBeginning;
				leftChannelCurrentPosM = this->offsetFromBeginningM;
				if(this->numChannelsM == 2)
					rightChannelCurrentPosM = this->offsetFromBeginningM + (this->bitsPerSampleM/8);

				this->findEndOfFile();
			}
			else
				throw string("WaveParser: There was an error when retrieving file's information.");
		}
		else
			throw string("WaveParser: The file you are trying to read is NOT a WAVE file.");
	}
	else
		throw string("WaveParser: There was a problem when reading the file.");
}

WaveParser::~WaveParser()
{
	fclose(waveFileM);
}

bool WaveParser::openWaveFile(const string& fileName)
{
	if((this->waveFileM = fopen(fileName.c_str(), "rb")))
		return true;
	else
		return false;
}

bool WaveParser::retrieveStandardHeader()
{
	memset(&WAV,0,sizeof(WAVEFORMATEX));

	if (fread(&WAV, sizeof(WAVEFORMATEX), 1, waveFileM) == 1)
		return true;
	else
	{
		if(ferror(waveFileM) > 0)
			cout << "retrieveStandardHeader: Error reading the file" << endl;
		if(feof(waveFileM) > 0)
			cout << "retrieveStandardHeader: End of file reached" << endl;

		return false;
	}
}

// Look for tags like WAVE, fmt, data, etc... if it has been found true is returned otherwise false is returned.
bool WaveParser::findID(const string& tag)
{
	char buffer[5];

	// Go back to the beginning of the file
	fseek(waveFileM, 0, SEEK_SET);

	if (fread(buffer, sizeof(char), 4, waveFileM) != 4)
	{
		cout <<  "findID: Reading was NOT OK" << endl;
		return false;
	}

	buffer[4] = '\0';

	while(strcmp(buffer, tag.c_str()) && !ferror(waveFileM) && !feof(waveFileM))
	{
		for(int i = 0; i < 3; i++)
			buffer[i] = buffer[i+1];

		if (fread(&buffer[3], sizeof(char), 1, waveFileM) != 1)
		{
			cout <<  "findID: Reading was NOT OK" << endl;
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

		return true;
	}
	else
	{
		if(ferror(waveFileM) > 0)
			cout << "retrieveDataSubChunkInfo: Error reading the file" << endl;
		if(feof(waveFileM) > 0)
			cout << "retrieveDataSubChunkInfo: End of file reached" << endl;

		return false;
	}
}

void WaveParser::findEndOfFile()
{
	fseek(waveFileM, 0, SEEK_END);
	this->endOfFilePosM = this->getAbsolutePosition();
	fseek(waveFileM, this->offsetFromBeginningM, SEEK_SET);
}

bool WaveParser::goBackToBeginOfDataChunk()
{
	if(this->leftChannelCurrentPosM == this->endOfFilePosM)
		this->leftChannelCurrentPosM = this->offsetFromBeginningM;

	if(this->rightChannelCurrentPosM == this->endOfFilePosM && this->numChannelsM == 2)
		this->rightChannelCurrentPosM = this->offsetFromBeginningM + (this->bitsPerSampleM/8);
}

void WaveParser::getStandardHeader(WAVEFORMATEX *WAV)
{
	*WAV = this->WAV;
}

int WaveParser::getNumberOfChannels()
{
	return this->numChannelsM;
}

int WaveParser::getSampleRate()
{
	return this->sampleRateM;
}

int WaveParser::getBytesPerSample()
{
	return this->bitsPerSampleM;
}

int WaveParser::getDataSubChunkSize()
{
	return this->dataSubChunkSizeM;
}

int WaveParser::getOffsetFromBeginningtoDataChunk()
{
	return this->offsetFromBeginningM;
}

long WaveParser::getAbsolutePosition()
{
	return ftell(this->waveFileM);
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
	cout << "Offset from Beginning to data chunk: " << DINFO.offsetFromBeginning << endl; 

	cout << "Left Channel Current Position: " << leftChannelCurrentPosM << endl;
	if(this->numChannelsM == 2)
		cout << "Right Channel Current Position: " << rightChannelCurrentPosM << endl;
}

