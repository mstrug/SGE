// source\server\mmfnullcodec.h
// Copyright (c) 2002 Symbian Ltd
//

//all functions are exported from the DLL and are virtual to allow derrived CMMFNullCodecs

#ifndef __MMF_SERVER_NULLCODEC_H__
#define __MMF_SERVER_NULLCODEC_H__

#include <mmfdatasource.h>
#include <mmfdatasink.h>
#include <mmfcodec.h>
#include <mmfdatabuffer.h>
#include <mmfutilities.h>

class MDataSource;
class MDataSink;
class CMMFBuffer;

/**
@internalComponent

Codec that does not modify or copy buffer content.

Cannot derrive from CMMFCodec because sink buffer can switch buffers.
*/
class CMMFNullCodec : public CBase 
	{
public:
	static CMMFNullCodec* NewL();
	~CMMFNullCodec();
	void AssignBuffers(MDataSource* aSource, MDataSink* aSink,
		CMMFBuffer** aSourceBuffer, TBool &aSrcBufRef,
		CMMFBuffer** aSinkBuffer, TBool &aSnkBufRef, TMediaId aMediaId);
	void DeleteBuffers();
	TCodecProcessResult ProcessL(CMMFBuffer& aSourceBuffer, CMMFBuffer** aSinkBuffer);	
private:
	CMMFNullCodec() {};
private:
	CMMFBuffer** iSinkBuffer1; //sink buffer is double buffered -so have 2 buffers
	CMMFBuffer** iSinkBuffer2;
	CMMFBuffer** iSourceBuffer1; //source buffer is double buffered -so have 2 buffers
	CMMFBuffer** iSourceBuffer2;
	CMMFBuffer** iSinkBuffer; //this is set to point to whichever sink buffer is in use
	CMMFBuffer** iSourceBuffer; //this is the pointer to whichever source buffer is in use
	};

#endif


