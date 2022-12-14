// mmfswcodecwrapper.h
// Copyright (c) 2003 Symbian Ltd
//

#ifndef __MMFSWCODECWRAPPER_H__
#define __MMFSWCODECWRAPPER_H__

#include <mmfhwdevice.h>

class CMMFSwCodecDataPath; //forward reference


/** 
@publishedAll
@released

Class for a software codec used by the CMMFSwCodecWrapper class to make the CMMFSwCodec a 
CMMFHwDevice plugin. The CMMFSwCodec processes source data in a certain fourCC coding type and
converts it to a destination buffer of another fourCC coding type.

A CMMFSwCodec object would usually not be instantiated directly
but instead would be instantiated via the CMMFSwCodecWrapper class's Codec()
method.

The processing of the data is handled by the codecs ProcessL() member.
The intention is that the source buffer for conversion is converted to the appropriate coding type
in the destination buffer.  The size of the buffers passed in are determined by SourceBufferSize()
and SinkBufferSize() methods.  The buffer sizes should be chosen such that
the ProcessL() method can be guaranteed to have enough destination buffer to
completely process one source buffer.

The ProcessL should return a TCodecProcessResult returning the number of source bytes processed
and the number of destination bytes processed along with a process result code defined thus:
- EProcessComplete: the codec processed all the source data into the sink buffer
- EProcessIncomplete: the codec filled sink buffer before all the source buffer was processed
- EDstNotFilled: the codec processed the source buffer but the sink buffer was not filled
- EEndOfData: the codec detected the end data - all source data in processed but sink may not be full
- EProcessError: the codec process error condition

Unlike the 7.0s CMMFCodec::ProcessL method, the CMMFSwCodec::ProcessL method
should not return EProcessIncomplete as this case is not handled by the
CMMFSwCodecWrapper.
*/
class CMMFSwCodec : public CBase
	{
public:
	/**
	@publishedAll
	@released

	Indicates the result of processing data from the source buffer to a destination buffer
	and provides functions to compare the result code.
	The CMMFSwCodec buffer sizes should be set to return EProcessComplete
	The other return codes are to keep the ProcessL method compatible with
	the 7.0s CMMFCodec API.
	*/
	class TCodecProcessResult
		{
	public:
		/**
		Flag to track the codec's processing status.
		*/
		enum TCodecProcessResultStatus
			{
			/** The codec has successfully completed its processing. */
			EProcessComplete,
			/** Could not empty the source buffer because the destination buffer became full. */
			EProcessIncomplete,
			/** Codec came across an end of data. */
			EEndOfData,
			/** Could not fill the destination buffer because the source buffer has been emptied. */
			EDstNotFilled,
			/** An error occured. */
			EProcessError
			};

		/** Overloaded operator to test equality. */
		TBool operator==(const TCodecProcessResultStatus aStatus) const {return (iCodecProcessStatus == aStatus);}
		/** Overloaded operator to test inequality. */
		TBool operator!=(const TCodecProcessResultStatus aStatus) const {return (iCodecProcessStatus != aStatus);}

		/**
		Default constructor.
		*/
		TCodecProcessResult()
			:iCodecProcessStatus(EProcessError), iSrcBytesProcessed(0), iDstBytesAdded(0) {};

		public:
		/**
		The codec's processing status

		@see enum TCodecProcessResultStatus
		*/
		TCodecProcessResultStatus iCodecProcessStatus;

		/** The number of source bytes processed */
		TUint iSrcBytesProcessed;

		/** The number of bytes added to the destination buffer */
		TUint iDstBytesAdded;
		};
public:

	/**
	Processes the data in the specified source buffer and writes the processed data to
	the specified destination buffer.

	This function is synchronous, when the function returns the data has been processed.
	This is a virtual function that each derived class must implement.

	@param	aSource
			The source buffer containing data to encode or decode.
	@param	aDest
	 		The destination buffer to hold the data after encoding or decoding.

	@return	The result of the processing.

	@see    TCodecProcessResult
	*/
	virtual TCodecProcessResult ProcessL(const CMMFBuffer& aSource, CMMFBuffer& aDest) = 0;

	/**
	Gets the max size of the source buffer passed into the
	CMMFSwCodec::ProcessL function. 

	Note that this means that this is the Max size of each buffer passed to the codec.  The actual 
	size of the data could be less than the max size. This is a virtual function that each derived 
	class must implement.

	@return The max size of the source buffer in bytes.
	*/
	virtual TUint SourceBufferSize() = 0;

	/**
	Gets the max size of the sink (destination) buffer passed into the
	CMMFSwCodec::ProcessL method.  

	Note that this means that this is the Max size of each buffer passed to the codec.  The actual 
	size of the data written to this buffer could be less than the max size. This is a virtual 
	function that each derived class must implement.

	@return The max size of the sink buffer in bytes.
	*/
	virtual TUint SinkBufferSize() = 0;

	/**
	@internalAll

	Function that needs to be overriden if the codec is a 'Null' codec
	ie. it does not perform any data type transformation.  The 'Null' codec
	should override this to return ETrue and provide a dummy
	ProcessL. The CMMFSwCodecWrapper will then use the same buffer
	for both the source and the sink. Null codecs should return the same
	buffer size for both the Source and SinkBufferSize methods.
	Since most CMMFSwCodec implementations will not be null codecs
	this method can be ignored.

	Would not normally expect 3rd parties to have to implement this.
	*/
	virtual TBool IsNullCodec() {return EFalse;};
	};


/**
@publishedPartner
@released

Custom interface class for play functionality created by the
CMMFSwCodecWrapper::CustomInterface() method. It provides
access to miscellaneous functionality such as volume settings.
Ports of the sw codec wrapper may do this differently eg. via EAP.
Note Users should not use RMdaDevSound::TCurrentSoundFormatBuf to send
across the sample rate and channels because 1) it would mean
mmfswcodecwrapper.h would have to include mdasound.h 2)
the buffer size and encoding members are not required and 3) 3rd party ports
may not use RMdaDevSound.

Note also that this interface is just a simple interface to get
and set values. No checking is perfomed on the values sent (hence the
Set methods do not return an error code).
*/
class MPlayCustomInterface
	{
public:
	virtual void SetVolume(TUint aVolume) = 0;
	virtual TUint Volume() = 0;
	virtual TUint BytesPlayed() = 0;
	virtual void SetVolumeRamp(const TTimeIntervalMicroSeconds& aRampDuration) = 0;
	virtual TTimeIntervalMicroSeconds& VolumeRamp() = 0;
	};


/**
@publishedPartner
@released

Custom interface class for record functionality created by the
CMMFSwCodecWrapper::CustomInterface() method. It provides
access to miscelaneous functionality such as volume settings.
Ports of the sw codec wrapper may do this differently eg. via EAP.
Note: Users shoule not use RMdaDevSound::TCurrentSoundFormatBuf so send
across the sample rate and channels because 1) it would mean
mmfswcodecwrapper.h would have to include mdasound.h 2)
the buffer size and encoding members are not required and 3) 3rd party ports
may not use RMdaDevSound.

Note also that this interface is just a simple interface to get
and set values. No checking is perfomed on the values sent (hence the
Set methods do not return an error code).
*/
class MRecordCustomInterface
	{
public:
	virtual void SetGain(TUint aGain) = 0;
	virtual TUint Gain() = 0;
	virtual TUint BytesRecorded() = 0; 
	};


/** 
@publishedAll
@released

Class to make a CMMFSwCodec into a CMMFHwDevice ECOM plugin.

Most of the code to make a CMMFSwCodec into a CMMFHwDevice ECOM plugin is provided
in CMMFSwCodecWrapper.  Someone writing a plugin derrived from CMMFSwCodecWrapper
only needs to provide the standard ECOM implementation code, constructors
and destructors and implement the Codec() function which should return the
appropriate CMMFSwCodec.

Third parties using CMMFSwCodecWrapper that do not use the RMdaDevSound API
to talk to the sound drivers would need to port the datapaths for their own
sound drivers. Third parties would also need to change the custom interfaces
where necessary.
*/
class CMMFSwCodecWrapper : public CMMFHwDevice
	{
public:
	IMPORT_C virtual ~CMMFSwCodecWrapper();
protected:
	IMPORT_C CMMFSwCodecWrapper();
	IMPORT_C virtual TInt Init(THwDeviceInitParams &aDevInfo);
	IMPORT_C virtual TInt Start(TDeviceFunc aFuncCmd, TDeviceFlow aFlowCmd);
	IMPORT_C virtual TInt Stop();
	IMPORT_C virtual TInt Pause();
	IMPORT_C virtual TAny* CustomInterface(TUid aInterfaceId);
	IMPORT_C virtual TInt ThisHwBufferFilled(CMMFBuffer& aFillBufferPtr);
	IMPORT_C virtual TInt ThisHwBufferEmptied(CMMFBuffer& aBuffer);
	IMPORT_C virtual TInt SetConfig(TTaskConfig& aConfig);
	IMPORT_C virtual TInt StopAndDeleteCodec();
	IMPORT_C virtual TInt DeleteCodec();

	/**
	This method must return the CMMFSwCodec used by the derived
	CMMFSwCodecWrapper class.  The method should create the CMMFSwCodec
	if it hasn't done so already.

	This is a virtual function that each derived class must implement.

	@return The CMMFSwCodec used by the derrived CMMFSwCodecWrapper
	*/
	virtual CMMFSwCodec& Codec() = 0;

private:
	TInt StartEncode();
	TInt StartDecode();
	TInt StartConvert();
	
protected: 
	/** 
	The software codec used
	*/
	CMMFSwCodec* iCodec;
	
	/** 
	The source buffer for the codec
	*/
	CMMFDataBuffer* iSourceBuffer;
	
	/** 
	The sink buffer for the codec
	*/
	CMMFDataBuffer* iSinkBuffer;
	
	/** 
	The datapath used to transfer the data
	*/
	CMMFSwCodecDataPath* iDataPath;
	
	/** 
	The play custom interface
	*/
	MPlayCustomInterface* iPlayCustomInterface;
	
	/** 
	The record custom interface
	*/
	MRecordCustomInterface* iRecordCustomInterface;
	
	/** 
	The buffer size of the sound device
	*/
	TUint iDeviceBufferSize;
	
	/**
	The sample rate of the sound device
	*/
	TInt iSampleRate;

	/**
	The number of channels of the sound device
	*/
	TInt iChannels;
	};

#endif

