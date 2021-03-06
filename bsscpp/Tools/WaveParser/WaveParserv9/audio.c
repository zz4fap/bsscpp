/* OggEnc
 **
 ** This program is distributed under the GNU General Public License, version 2.
 ** A copy of this license is included with this source.
 **
 ** Copyright 2000, Michael Smith <msmith@xiph.org>
 **
 ** Portions from Vorbize, (c) Kenneth Arnold <kcarnold@yahoo.com>
 ** and libvorbis examples, (c) Monty <monty@xiph.org>
 **/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <malloc.h>
#include <windows.h>
#include <stdarg.h>
#include "audio.h"

/* Macros to read header data */
#define READ_U32(buf) \
	(((buf)[3]<<24)|((buf)[2]<<16)|((buf)[1]<<8)|((buf)[0]&0xff));

#define READ_U16(buf) \
	(((buf)[1]<<8)|((buf)[0]&0xff));

static char *_filename;
void (*error_handler)(const char *fmt, ...) = error_dialog;

static int find_chunk(FILE *in, char *type, unsigned int *len)
{
	unsigned char buf[8];

	while(1)
	{
		if(fread(buf,1,8,in) < 8) /* Suck down a chunk specifier */
		{
			error_handler("Unexpected EOF in reader WAV header");
			fprintf(stderr, "Warning: Unexpected EOF in reading WAV header\n");
			return 0; /* EOF before reaching the appropriate chunk */
		}

		if(memcmp(buf, type, 4))
		{
			*len = READ_U32(buf+4);
			if(fseek(in, *len, SEEK_CUR)) /* Skip the rest of the chunk */
			{
				/* seek failed. Do it the hard way */
				unsigned char buf2[1024];
				int seek_needed = *len, seeked;
				while(seek_needed>0)
				{
					seeked = fread(buf2,1,seek_needed>1024?1024:seek_needed,in);
					if(!seeked) {
						error_handler("Unexpected EOF in reading WAV header");
						return 0; /* Couldn't read more, can't read file */
					} else
						seek_needed -= seeked;
				}
			}
			buf[4] = 0;
			fprintf(stderr, "Skipping chunk of type \"%s\", length %d\n", buf, *len);
		}
		else
		{
			*len = READ_U32(buf+4);
			return 1;
		}
	}
}

int wav_open(FILE *in, oe_enc_opt *opt)
{
	unsigned char hdrbuf[12];
	unsigned char *buf;
	unsigned int len;
	wav_fmt format;
	wavfile *wav = malloc(sizeof(wavfile));

	int ret = fread(hdrbuf, 1, 12, in);

	if(ret < 12)
		return 0;

	if(memcmp(hdrbuf, "RIFF", 4))
		return 0;

	len = READ_U32(hdrbuf+4); /* We don't actually use this */
	if(memcmp(hdrbuf+8, "WAVE",4))
		return 0; /* Not wave file */

	/* Ok. At this point, we know we have a WAV file. Now we have to detect
	 * whether we support the subtype, and we have to find the actual data
	 */

	if(!find_chunk(in, "fmt ", &len))
		return 0; /* EOF */

	/* dynamic allocation, since "fmt" chunk can be any length */
	buf = malloc(len);

	if(len<16) 
	{
		free(buf);
		error_handler("Truncated format chunk of length %d in WAV header", len);
		fprintf(stderr, "Error: Truncated format chunk in WAV header\n");
		return 0; /* Weird format chunk */
	}

	if(fread(buf,1,len,in) < 16)
	{
		free(buf);
		error_handler("Unexpected EOF in reading WAV header");
		fprintf(stderr, "Error: Unexpected EOF in reading WAV header\n");
		return 0;
	}

	format.format =      READ_U16(buf); 

/* temporary hack, until we decide where to put this defined constant */
#ifndef WAVE_FORMAT_PCM
# define WAVE_FORMAT_PCM 0x0001
#endif

	if (format.format != WAVE_FORMAT_PCM)
	{
		free(buf);
		error_handler("WAVE format: %#02.02x not supported.", format.format);
		fprintf(stderr, "Error: WAVE format: %#02.02x not supported.\n", format.format);
		return 0;
	} 
	else if (len > 18)
	{
		free(buf);
		error_handler("WAVE PCM with bad format size %d\n", len);
		fprintf(stderr, "Error: WAVE PCM with bad format size: %d\n", len);
		return 0;
	}

	format.channels =    READ_U16(buf+2); 
	format.samplerate =  READ_U32(buf+4);
	format.bytespersec = READ_U32(buf+8);
	format.align =       READ_U16(buf+12);
	format.samplesize =  READ_U16(buf+14);

	free(buf);

	if(!find_chunk(in, "data", &len))
		return 0; /* EOF */

	if( format.format == 1 &&
		(format.channels == 2 || format.channels == 1) &&
		format.align == format.channels*2 && /* We could deal with this one pretty easily */
		format.samplesize == 16)
	{
		/* OK, good - we have the one supported format,
		   now we want to find the size of the file */
		opt->rate = format.samplerate;
		opt->channels = format.channels;
		if(opt->channels ==2)
			opt->read_samples = wav_read_stereo;
		else
			opt->read_samples = wav_read_mono;

		wav->f = in;
		wav->samplesread = 0;

		if(len)
		{
			opt->total_samples_per_channel = len/(format.channels*2);
			wav->totalsamples = len/(format.channels*2);
		}
		else
		{
			long pos;
			pos = ftell(in);
			if(fseek(in, 0, SEEK_END) == -1)
			{
				opt->total_samples_per_channel = 0; /* Give up */
				wav->totalsamples = 0;
			}
			else
			{
				opt->total_samples_per_channel = (ftell(in) - pos)/(format.channels*2);
				wav->totalsamples = len/(format.channels*2);
				fseek(in,pos, SEEK_SET);
			}
		}
		opt->readdata = (void *)wav;
		return 1;
	}
	else
	{
		error_handler("WAV file is unsupported subformat (must be 44.1kHz/16bit, this is %f/%dbit, %s)\n",
			(double)format.samplerate / 1000,
			format.samplesize,
			(format.channels == 1) ? "mono" : "stereo");
		fprintf(stderr, "ERROR: WAV file is unsupported subformat (must be 44.1kHz/16bit, this is %f/%dbit, %s)\n",
			(double)format.samplerate / 1000,
			format.samplesize,
			(format.channels == 1) ? "mono" : "stereo");
		return 0;
	}
}

long wav_read_stereo(void *in, float **buffer, int samples)
{
	signed char *buf = alloca(samples*4);
	wavfile *f = (wavfile *)in;
	long bytes_read = fread(buf, 1, samples*4, f->f);
	int i;

	if(f->totalsamples && f->samplesread + bytes_read/4 > f->totalsamples)
		bytes_read = 4*(f->totalsamples - f->samplesread);
	f->samplesread += bytes_read/4;
		

	for(i = 0; i < bytes_read/4; i++)
	{
		buffer[0][i] = (float) (((buf[i*4+1]<<8) | (((int)buf[i*4]) & 0xff))/32768.0);
		buffer[1][i] = (float) (((buf[i*4+3]<<8) | (((int)buf[i*4+2]) & 0xff))/32768.0);
	}

	return bytes_read/4;
}

long raw_read_stereo(void *in, float **buffer, int samples)
{
	signed char *buf = alloca(samples*4);
	long bytes_read = fread(buf,1,samples*4, (FILE *)in);
	int i;

	for(i=0;i<bytes_read/4; i++)
	{
		buffer[0][i] = (float) (((buf[i*4+1]<<8) | (((int)buf[i*4]) & 0xff))/32768.0);
		buffer[1][i] = (float) (((buf[i*4+3]<<8) | (((int)buf[i*4+2]) & 0xff))/32768.0);
	}

	return bytes_read/4;
}

long wav_read_mono(void *in, float **buffer, int samples)
{
	signed char *buf = alloca(samples*2);
	wavfile *f = (wavfile *)in;
	long bytes_read = fread(buf, 1, samples*2, f->f);
	int i;

	if(f->totalsamples && f->samplesread + bytes_read/2 > f->totalsamples)
		bytes_read = 2*(f->totalsamples - f->samplesread);
	f->samplesread += bytes_read/2;


	for(i=0;i<bytes_read/2; i++)
		buffer[0][i] = (float) (((buf[i*2+1]<<8) | (((int)buf[i*2]) & 0xff))/32768.0);

	return bytes_read/2;
}

void wav_close(void *info)
{
	wavfile *f = (wavfile *)info;

	free(f);
}

int raw_open(FILE *in, oe_enc_opt *opt)
{
	opt->rate = 44100; /* we assume this */
	opt->channels = 2;
	opt->readdata = (void *)in;
	opt->read_samples = raw_read_stereo; /* it's the same, currently */
	opt->total_samples_per_channel = 0; /* raw mode, don't bother */
	return 1;
}

/**
 * Set teh current input file name.
 */
void set_filename(const char *filename)
{
	_filename = (char *) filename;
}

/**
 * Display an error dialog, possibly adding system error information.
 */
void error_dialog(const char *fmt, ...)
{
	va_list ap;
	char msgbuf[1024];
	char *bufp = msgbuf;

	/* A really rough sanity check to protect against blatant buffer overrun */
	if (strlen(fmt) > 750)
	{
		sprintf(msgbuf, "%s %s", "<buffer overflow> ", fmt);
	} 
	else 
	{
		if (_filename != NULL && strlen(_filename) < 255)
		{
			sprintf(msgbuf, "%s: ", _filename);
			bufp += strlen(msgbuf);
		}

		va_start(ap, fmt);
		
		vsprintf(bufp, fmt, ap);

		va_end(ap);

		if (errno != 0)
		{
			bufp = msgbuf + strlen(msgbuf);
			sprintf(bufp, " error is %s (%d)", strerror(errno), errno);
			errno = 0;
		}
	}

	MessageBox(NULL, msgbuf, "Error", 0);
}

void log_error(const char *fmt, ...)
{
	va_list ap;
	FILE *fp;
	char msgbuf[1024];
	char *bufp = msgbuf;

	/* A really rough sanity check to protect against blatant buffer overrun */
	if (strlen(fmt) > 750)
	{
		sprintf(msgbuf, "%s %s", "<buffer overflow> ", fmt);
	}
	else
	{
		if (_filename != NULL && strlen(_filename) < 255)
		{
			sprintf(msgbuf, "%s : ", _filename);
			bufp += strlen(msgbuf);
		}

		va_start(ap, fmt);

		vsprintf(bufp, fmt, ap);

		va_end(ap);

		if (errno != 0)
		{
			bufp = msgbuf + strlen(msgbuf);
			sprintf(bufp, " error is: %s (%d)", strerror(errno), errno);
			errno = 0;
		}
	}

	va_start(ap, fmt);

	if ((fp = fopen("oggdrop.log", "a")) == (FILE *)NULL)
		return;

	fprintf(fp, "%s\n", msgbuf);
	fflush(fp);
	fclose(fp);

	va_end(ap);
}

void set_use_dialogs(int use_dialogs)
{
	if (!use_dialogs)
		error_handler = log_error;
	else
		error_handler = error_dialog;
}
