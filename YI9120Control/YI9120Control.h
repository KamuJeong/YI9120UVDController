#pragma once

#ifdef UVD9120_EXPORTS
#define UVD9120_API __declspec(dllexport)
#else
#define UVD9120_API __declspec(dllimport)
#endif


/// <summary>
/// ������ ���� �ݹ� �Լ� - startAcquisition() ���� stopAcqusition() ���� 200msec ���� ȣ��� 
/// </summary>

// m : �ð�
// count: ���� ������ ���� (25�� ����)
// data: ���� ������ array
typedef void (*fn_acquire)(float m, int count, const float* data);


/// <summary>
/// UVD Controller ��ü�� �����ϰ� �ش� descriptor�� ����
/// </summary>
/// <param name="fn">������ ���� �ݹ�</param>
/// <returns>UVD descriptor</returns>
extern "C" UVD9120_API int createUVD(fn_acquire fn);

/// <summary>
/// ��� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <param name="ip">IP �ּ�, �⺻�� 10.10.10.20</param>
/// <param name="port">��Ʈ �ѹ�, �⺻�� 4242</param>
/// <returns>�����ϸ� true, �׷��� ������ false</returns>
extern "C" UVD9120_API bool connectUVD(int device, const char * ip, int port);

/// <summary>
/// ��� ���� ���� Ȯ��
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <returns>����Ǿ� ������ true, �׷��� ������ false</returns>
extern "C" UVD9120_API bool isUVDConnected(int device);

/// <summary>
/// �غ� ���� Ȯ�� - ���� ���� �� ���� ������ �غ� ���¿����� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <returns>�غ� ���¸� true, �׷��� ������ false</returns>
extern "C" UVD9120_API bool isUVDReady(int device);

/// <summary>
/// ���� ���� Ȯ��
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <returns>���� �� �̸� true, �׷��� ������ false</returns>
extern "C" UVD9120_API bool isUVDRunning(int device);

/// <summary>
/// �߼��� ���� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <param name="on">ON�̸� true, OFF�� false</param>
extern "C" UVD9120_API void turnOnD2Lamp(int device, bool on);

/// <summary>
/// �ֽ��� ���� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <param name="on">ON�̸� true, OFF�� false</param>
extern "C" UVD9120_API void turnOnWLamp(int device, bool on);

/// <summary>
/// ���� ���� Ȯ��
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <returns>���� ����</returns>
extern "C" UVD9120_API const int getWavelength(int device);

/// <summary>
/// ���� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <param name="wave">���� (190~1024)</param>
extern "C" UVD9120_API void setWavelength(int device, int wave);

/// <summary>
/// ���� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
/// <returns>����</returns>
extern "C" UVD9120_API const float getAbsorbance(int device);

/// <summary>
/// AutoZero - ���� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
extern "C" UVD9120_API void setAutoZero(int device);

/// <summary>
/// ���� ���� - createUVD()�� ������ �ݹ��Լ��� �ҷ����� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
extern "C" UVD9120_API void startAcquisition(int device);

/// <summary>
/// ���� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
extern "C" UVD9120_API void stopAcquisition(int device);

/// <summary>
/// UVD Controller ��ü�� ����
/// </summary>
/// <param name="device">createUVD()���� ���� descriptor</param>
extern "C" UVD9120_API void closeUVD(int device);
 