// UVD9120.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "../YI9120Control/YI9120Control.h"
#include <iostream>
#include <thread>
#include <vector>

static std::vector<float> acqData;

void function_acquisition(float t, int count, const float* data)
{
	float avg = 0.0f;
	for (int i = 0; i < count; ++i)
	{
		avg += data[i];
	}
	avg /= count;

	acqData.push_back(avg);
}

int main()
{
	int id = createUVD(function_acquisition);

	if (connectUVD(id, "127.0.0.1", 2000))
	{
		turnOnD2Lamp(id, true);
		turnOnWLamp(id, false);
		setWavelength(id, 350);

		while (true)
		{
			int number;

			std::cout << std::endl << std::endl;
			std::cout << "1. Show current state" << std::endl;
			std::cout << "2. Show current absorbance" << std::endl;
			std::cout << "3. Start acquisition" << std::endl;
			std::cout << "4. Stop acqusition" << std::endl;
			std::cout << "5. AutoZero" << std::endl;
			std::cout << "6. Exit" << std::endl;
			std::cout << "190 ~ 1024. Set wavelength" << std::endl;

			std::cout << "Input number: ";
			std::cin >> number;

			if (number == 0 || number == 6)
				break;

			std::cout << "--------------------------" << std::endl;

			switch (number)
			{
			case 1:
				if (isUVDReady(id))			std::cout << "UVD is ready" << std::endl;
				else if (isUVDRunning(id))	std::cout << "UVD is running" << std::endl;
				else						std::cout << "UVD is in unknown state" << std::endl;
				break;
			case 2:
				std::cout << getAbsorbance(id) << " AU" << std::endl;
				break;
			case 3:
				acqData.clear();
				startAcquisition(id);
				break;
			case 4:
				stopAcquisition(id);
				std::cout << "Data count: " << (int)acqData.size() << std::endl;
				break;
			case 5:
				setAutoZero(id);
				break;
			default:
				if (number >= 190 && number <= 1024)
				{
					setWavelength(id, number);
				}
				break;
			}
		}

		closeUVD(id);
	}
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
