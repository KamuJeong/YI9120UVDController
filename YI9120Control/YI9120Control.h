#pragma once

#ifdef UVD9120_EXPORTS
#define UVD9120_API __declspec(dllexport)
#else
#define UVD9120_API __declspec(dllimport)
#endif


/// <summary>
/// 데이터 수집 콜백 함수 - startAcquisition() 이후 stopAcqusition() 까지 200msec 마다 호출됨 
/// </summary>

// m : 시간
// count: 흡광도 데이터 갯수 (25로 고정)
// data: 흡광도 데이터 array
typedef void (*fn_acquire)(float m, int count, const float* data);


/// <summary>
/// UVD Controller 객체를 생성하고 해당 descriptor를 얻음
/// </summary>
/// <param name="fn">데이터 수집 콜백</param>
/// <returns>UVD descriptor</returns>
extern "C" UVD9120_API int createUVD(fn_acquire fn);

/// <summary>
/// 통신 연결
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <param name="ip">IP 주소, 기본값 10.10.10.20</param>
/// <param name="port">포트 넘버, 기본값 4242</param>
/// <returns>성공하면 true, 그렇지 않으면 false</returns>
extern "C" UVD9120_API bool connectUVD(int device, const char * ip, int port);

/// <summary>
/// 통신 연결 상태 확인
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <returns>연결되어 있으면 true, 그렇지 않으면 false</returns>
extern "C" UVD9120_API bool isUVDConnected(int device);

/// <summary>
/// 준비 상태 확인 - 램프 점등 및 파장 변경은 준비 상태에서만 가능
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <returns>준비 상태면 true, 그렇지 않으면 false</returns>
extern "C" UVD9120_API bool isUVDReady(int device);

/// <summary>
/// 수집 상태 확인
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <returns>수집 중 이면 true, 그렇지 않으면 false</returns>
extern "C" UVD9120_API bool isUVDRunning(int device);

/// <summary>
/// 중수소 램프 점등
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <param name="on">ON이면 true, OFF면 false</param>
extern "C" UVD9120_API void turnOnD2Lamp(int device, bool on);

/// <summary>
/// 텅스텐 램프 점등
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <param name="on">ON이면 true, OFF면 false</param>
extern "C" UVD9120_API void turnOnWLamp(int device, bool on);

/// <summary>
/// 현재 파장 확인
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <returns>현재 파장</returns>
extern "C" UVD9120_API const int getWavelength(int device);

/// <summary>
/// 파장 설정
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <param name="wave">파장 (190~1024)</param>
extern "C" UVD9120_API void setWavelength(int device, int wave);

/// <summary>
/// 현재 흡광도
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
/// <returns>흡광도</returns>
extern "C" UVD9120_API const float getAbsorbance(int device);

/// <summary>
/// AutoZero - 영점 조정
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
extern "C" UVD9120_API void setAutoZero(int device);

/// <summary>
/// 수집 시작 - createUVD()에 설정한 콜백함수가 불려지기 시작
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
extern "C" UVD9120_API void startAcquisition(int device);

/// <summary>
/// 수집 종료
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
extern "C" UVD9120_API void stopAcquisition(int device);

/// <summary>
/// UVD Controller 객체를 해제
/// </summary>
/// <param name="device">createUVD()에서 얻은 descriptor</param>
extern "C" UVD9120_API void closeUVD(int device);
 