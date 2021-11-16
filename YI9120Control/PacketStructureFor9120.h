#pragma once

#include <string.h>

struct header
{
    int length;
    int id;
    int code;
    int index;
    int offset;
    int size;
};

#define PACKCODE_YL9020_INFORM		(0x90200)
#define PACKCODE_YL9020_CONFIG		(0x90201)
#define PACKCODE_YL9020_SETUP		(0x90202)
#define PACKCODE_YL9020_EVENT		(0x90203)
#define PACKCODE_YL9020_STATE		(0x90204)
#define PACKCODE_YL9020_SELFMSG		(0x90205)
#define PACKCODE_YL9020_DIAGDATA	(0x90206)
#define PACKCODE_YL9020_ACQDATA		(0x90207)
#define PACKCODE_YL9020_SCANDATA	(0x90208) 
#define PACKCODE_YL9020_SERVICE		(0x90209)
#define PACKCODE_YL9020_SVCDATA		(0x9020A)


#define YL9020_UVD_DIAGNOSIS_COUNT		(11)

typedef struct _YL9020_UVD_DIAGNOSIS	{
	unsigned char	btVCCCheck;		// 5V, 0: None, 1: Check
	unsigned char 	btVDDCheck;		// 15V, 0: None, 1: Check
	unsigned char	btVSSCheck;		// -15V, 0: None, 1: Check
	unsigned char 	btDCCheck;		// +12V, 0: None, 1: Check
	unsigned char	btWPowerCheck;		// 0: None, 1: Check
	unsigned char	btSignal1Check;		// 0: None, 1: Check
	unsigned char	btSignal2Check;		// 0: None, 1: Check

	unsigned char	btD2LampON;		// 0: None, 1: Check
	unsigned char 	btCalibration;		// 0: None, 1: Check, 2: 파장교정 실행
	unsigned char	btWLampON;		// 0: None, 1: Check
	unsigned char	btCutFilterCheck;	// 0: None, 1: Check

	_YL9020_UVD_DIAGNOSIS()	{
		memset(this, 1, sizeof(struct _YL9020_UVD_DIAGNOSIS));
	}
} YL9020_UVD_DIAGNOSIS;


typedef struct _YL9020_UVD_INFORM
{
	char 	Model[32];				// 기기 정보 ex) "UV Detector ver. 1.0.0"
	unsigned int version;			// 프로토콜 버전(100)
	// Diagnosis 관련 사항들이 추가되어야 함
	YL9020_UVD_DIAGNOSIS	Diagnosis;

	_YL9020_UVD_INFORM()	{
		memset(this, 0, sizeof(struct _YL9020_UVD_INFORM));
		version = 100;
	}
} YL9020_UVD_INFORM;


typedef struct _YL9020_UVD_CONFIG
{
	unsigned short	sCutWavelength0;	// Cut Filter 변환 파장
	unsigned short	sCutWavelength1;
	unsigned int	nD2LampElapse;		// D2 사용 시간
	unsigned int	nWLampElapse;		// W  사용 시간

	_YL9020_UVD_CONFIG()	{
		sCutWavelength0 = 350;
		sCutWavelength1 = 700;
		nD2LampElapse = 0;
		nWLampElapse = 0;
	}
} YL9020_UVD_CONFIG;


typedef struct _YL9020_UVD_EVENT
{
	float 	fTime;			// 실행 시간 [min]
	unsigned short 	sWavelengthA;	// 190 ~ 1024
	unsigned short	sWavelengthB;
	// Lamp 셋업
	unsigned char 	btD2Lamp;		// 0: OFF, 1:ON
	unsigned char	btWLamp;		// 0: OFF, 1:ON
	// Cut Filter 셋업
	unsigned char 	btCutFilter;	// 0: Auto, 1: None, 2: 1st Filter, 3: 2nd Filter, 4: Holium Filter
	// Data Process
	unsigned char	btPolarity;		// 0: Positive, 1:Negative
	unsigned char	btAutoZero;		// 0: None, 1: AZ
	// 외부 출력
	unsigned char	btMarkOut;		// 0: close, 1: open, 2: Pulse

	_YL9020_UVD_EVENT() {
		memset(this, 0, sizeof(struct _YL9020_UVD_EVENT));
		sWavelengthA = sWavelengthB = 254;
		btD2Lamp = 1;
		btWLamp = 1;
	}
} YL9020_UVD_EVENT;


typedef struct _YL9020_UVD_SETUP
{
	// 명령 코드
	unsigned char	btCommand;	// 0: None, 1: AutoZero, 2: Acquisition Start, 3: Scan Start, 
								// 4: Diagnosis Start, 5: Stop, 6: Reset Error, 7: init
								// 0xff, 0xfe, 0xfd: only software
	enum {
		XUVDCmdNone = 0,
		XUVDCmdAutoZero,
		XUVDCmdRun,
		XUVDCmdScan,
		XUVDCmdDiagnosis,
		XUVDCmdStop,
		XUVDCmdReset,
		XUVDCmdInit,
	};
	
	struct YL9020_UVD_CHANNEL	{
		float			fTimeConst;		// 시정수 [sec]
		unsigned char	btAutoZero;		// 0: None, 1: 수집시작시, 2: 파장 변경시
		unsigned char	btAutoOffset;	// 0: None, 1: 파장변경시, 2: 극 변환시 
		unsigned char	btDataFilter;	// 0: Moving Average, 1: RC, 2: Bessel

		YL9020_UVD_CHANNEL() {
			fTimeConst = 1.0f;
			btAutoZero = 1;
			btAutoOffset = 3;
			btDataFilter = 0;
		}
	};
	struct YL9020_UVD_CHANNEL	ChannelSetA;
	struct YL9020_UVD_CHANNEL	ChannelSetB;

	unsigned int		nProgramCount;
	YL9020_UVD_EVENT	InitEvent;

	// 외부 입력
	unsigned char	btAZExtIn;			// 0: None, 1: 0>1, 2: 1>0
	unsigned char	btLampExtIn;		// 0: None, 1: 0>1, 2: 1>0
	unsigned char	btChartExtIn;		// 0: None, 1: 0>1, 2: 1>0
	unsigned char	btStartExtIn;		// 0: None, 1: 0>1, 2: 1>0
	unsigned char	btReadyExtIn;		// 0: level low, 1: level high;

	unsigned char	btReadyExtOut;		// 0: close, 1: open
	unsigned char	btStartExtOut;		// 0: close, 1: open

	unsigned char	btDefaultMarkOut;	// 0: close, 1: open
	unsigned int	nExtoutTime;		// msec (100 - 100000)

	// Signal output
	struct YL9020_UVD_SIGOUT	{
		unsigned char btSource;			// 0: 채널 A, 1: 채널 B, 2: A+B, 3: A-B, 4: B-A 
		unsigned char btType;			// 0: 흡광도, 1: 투과도( 10^(-흡광도) ), 2: 참조광량, 3: 샘플광량
		float fConversion;				// btType == 0: Volt/Abs,	btType == 1: Volt,	btType == 2 or 3: Volt/nA
		float fOffset;					// Volt
										// 출력전압 = ( x(btSource, btType) * fConversion ) + fOffset;
		YL9020_UVD_SIGOUT()	{
			memset(this, 0, sizeof(struct YL9020_UVD_SIGOUT ));
			fConversion = 1.0f;
		}
	} SigOutSet[2];

	// Scan setup
	struct YL9020_UVD_SCAN	{	
		unsigned short	sStartWavelength ;	// 시작 파장
		unsigned short	sStopWavelength ;	// 종료 파장
		unsigned char	btScanInterval ;	// 스캔 간격
		unsigned char	btSamplingCount ;	// 샘플링 횟수(255를 넘지않는다는 조건)

		YL9020_UVD_SCAN()	{
			sStartWavelength = 190;
			sStopWavelength = 900;
			btScanInterval = 1;
			btSamplingCount = 1;
		}
	} ScanSet;

	// Diagnosis Setup
	YL9020_UVD_DIAGNOSIS DiagnosisSet;

	_YL9020_UVD_SETUP() {
		memset( this, 0, sizeof(struct _YL9020_UVD_SETUP));
		ChannelSetA = ChannelSetB = YL9020_UVD_CHANNEL();
		InitEvent = YL9020_UVD_EVENT();
		btStartExtIn = 2;
		nExtoutTime = 1000;
		SigOutSet[0] = SigOutSet[1] = YL9020_UVD_SIGOUT();
		SigOutSet[1].btSource = 1;
		ScanSet = YL9020_UVD_SCAN();

		DiagnosisSet = YL9020_UVD_DIAGNOSIS();
	}
} YL9020_UVD_SETUP;

struct _YL9020_UVD_SIGNAL	{
	float 	fAbsorbance;
	float	fReferenceE;
	float	fSampleE;
};

typedef struct _YL9020_UVD_STATE		// 기기와 연결된 후  1초마다 자동 수신
{
	unsigned char 	btState;			// 0: Initializing 1: Ready, 2: Run, 3: Scanning,
										// 4: Diagnosis, 5: Fault
	enum {
		XUVDStateInit = 0,
		XUVDStateReady,
		XUVDStateRun,
		XUVDStateScan,
		XUVDStateDiagnosis,
		XUVDStateFault
	};

	unsigned char 	btD2Lamp;			// 0: OFF, 1:ON
	unsigned char	btWLamp;			// 0: OFF, 1:ON
	unsigned char 	btCutFilter;		// 0: Unknown, 1: None, 2: 1st Filter, 3: 2nd Filter, 4: Holium Filter
	unsigned int	uErrorCode;

	unsigned int	nEventStep;		// 현재 실행 중 인 Step 번호 
	float			fElapseTime;	// Running Time[min]

	unsigned short 	sWavelengthA;
	unsigned short 	sWavelengthB;

	_YL9020_UVD_SIGNAL SignalA;
	_YL9020_UVD_SIGNAL SignalB;

	// 외부 단자 상태
	unsigned char	btReadyIn;			// 0: not ready, 1: ready

	_YL9020_UVD_STATE()	{
		memset(this, 0, sizeof(struct _YL9020_UVD_STATE));
		sWavelengthA = sWavelengthB = 254;
	}
} YL9020_UVD_STATE;


typedef struct _YL9020_UVD_ACQDATA
{
	float index;
	_YL9020_UVD_SIGNAL SignalA[25];
	_YL9020_UVD_SIGNAL SignalB[25];
} YL9020_UVD_ACQDATA;


typedef struct _YL9020_UVD_SCANDATA
{
	unsigned short sWavelength;
	float fAbsorbance;
	float fReferenceE;
	float fSampleE;
	
	_YL9020_UVD_SCANDATA()	{
		memset(this, 0, sizeof(struct _YL9020_UVD_SCANDATA));
	}
} YL9020_UVD_SCANDATA;

typedef struct _YL9020_UVD_DIAGDATA
{
	unsigned int	uDiagnosis;
	float			fValue;
	unsigned char	btPass;			// 0: reject, 1: Pass
	
	_YL9020_UVD_DIAGDATA()	{
		memset(this, 0, sizeof(struct _YL9020_UVD_DIAGDATA));
	}
} YL9020_UVD_DIAGDATA;


typedef struct _YL9020_UVD_SELFMSG
{
	unsigned char btMessage;		// 1: State, 2: D2Lamp, 3: WLamp, 
									// 4: ExtIn, 5: ExtOut, 6: Error
	enum {
		XUVDMsgNone = 0,
		XUVDMsgState,
		XUVDMsgD2lamp,
		XUVDMsgWLamp,
		XUVDMsgExtIn,
		XUVDMsgExtOut,
		XUVDMsgError
	};

	unsigned short sOldValue, sNewValue;
	enum {
		XUVDMsgValD2Off = 0,
		XUVDMsgValD2On,
		XUVDMsgValD2Fail,

		XUVDMsgValWOff = 0,
		XUVDMsgValWOn,

		XUVDMsgValEInAZ	= 0,
		XUVDMsgValEInLamp,
		XUVDMsgValEInChart,
		XUVDMsgValEInStart,
		XUVDMsgValEInStop,
		XUVDMsgValEInReady,
		XUVDMsgValEInNotReady,

		XUVDMsgValEOutMkOff = 0,
		XUVDMsgValEOutMkOn,
		XUVDMsgValEOutMkPulse,
		XUVDMsgValEOutReady,
		XUVDMsgValEOutNotReady
	};

	unsigned int uErrorCode;
	enum {
		XUVDMsgErrNone = 0,
		XUVDMsgErrConfig,
		XUVDMsgErrSetup,
		XUVDMsgErrService,
		XUVDMsgErrD2Lamp,
		XUVDMsgErrLeak = 7,
		XUVDMsgErrPower,
	};

	_YL9020_UVD_SELFMSG()	{
		memset(this, 0, sizeof(struct _YL9020_UVD_SELFMSG));
	}
} YL9020_UVD_SELFMSG;


