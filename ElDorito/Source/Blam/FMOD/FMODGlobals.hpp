#pragma once

namespace FMOD {

	// Max number of channels (real and virtual). Each channel represents a sound.
	const int FMOD_MAX_CHANNELS = 1024;

	// TODO: Add the commented enums to FMODEnum.cpp

	struct FMOD_CREATESOUNDEXINFO{
		int cbsize;
		unsigned int length;
		unsigned int fileoffset;
		int numchannels;
		int defaultfrequency;
		FMOD_SOUND_FORMAT format;
		unsigned int decodebuffersize;
		int initialsubsound;
		int numsubsounds;
		int *inclusionlist;
		int inclusionlistnum;
		void* pcmreadcallback;			//FMOD_SOUND_PCMREAD_CALLBACK
		void* pcmsetposcallback;		//FMOD_SOUND_PCMSETPOS_CALLBACK
		void* nonblockcallback;			//FMOD_SOUND_NONBLOCK_CALLBACK
		const char *dlsname;
		const char *encryptionkey;
		int maxpolyphony;
		void *userdata;
		FMOD_SOUND_TYPE suggestedsoundtype;
		void* fileuseropen;				//FMOD_FILE_OPEN_CALLBACK
		void* fileuserclose;			//FMOD_FILE_CLOSE_CALLBACK
		void* fileuserread;				//FMOD_FILE_READ_CALLBACK
		void* fileuserseek;				//FMOD_FILE_SEEK_CALLBACK
		void* fileuserasyncread;		//FMOD_FILE_ASYNCREAD_CALLBACK
		void* fileuserasynccancel;		//FMOD_FILE_ASYNCCANCEL_CALLBACK
		void *fileuserdata;
		int filebuffersize;
		FMOD_CHANNELORDER channelorder;
		FMOD_CHANNELMASK channelmask;
		void* *initialsoundgroup;		//FMOD_SOUNDGROUP
		unsigned int initialseekposition;
		FMOD_TIMEUNIT initialseekpostype;
		int ignoresetfilesystem;
		unsigned int audioqueuepolicy;
		unsigned int minmidigranularity;
		int nonblockthreadid;
		void *fsbguid;					//FMOD_GUID
	};

}