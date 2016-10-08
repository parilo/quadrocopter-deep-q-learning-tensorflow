//
//  Lib.cpp
//  QuizGeek
//
//  Created by anton on 13.11.14.
//
//

#include "Lib.h"
#include <cstdlib>
#include "time.h"
#include <chrono>
#include "main.h"

#define _USE_MATH_DEFINES
#include <cmath>
//#include <string>
//#include <sstream>
#include <iostream>

void Lib::randomize () {
    std::srand (time(NULL));
}

int Lib::randInt (int min, int max) {
    return min + (std::rand() % (int)(max - min + 1));
}

float Lib::randFloat (float LO, float HI) {
    return LO + static_cast <float> (std::rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}



long long Lib::getTimestampInMillis () {
    using namespace std::chrono;
    return duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
}

float Lib::degToRad (float deg) {
	return M_PI * deg / 180;
}

float Lib::radToDeg (float rad) {
	return 180 * rad / M_PI;
}

void printVector (const std::vector<float>& v) {
	for (const auto& vi : v) {
		std::cout << vi << " ";
	}
	std::cout << std::endl;
}



//std::string qg::to_string(int value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(unsigned value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(long value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(unsigned long value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(long long value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(unsigned long long value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(float value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(double value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//std::string qg::to_string(long double value)
//{
//    std::ostringstream os ;
//    os << value ;
//    return os.str() ;
//}
//
//int qg::stoi (std::string str) {
//    std::stringstream ss;
//    ss << str ;
//    int integer;
//    ss >> integer;
//    return integer;
//}
//
//long long qg::stoll (std::string str) {
//    std::stringstream ss;
//    ss << str ;
//    long long integer;
//    ss >> integer;
//    return integer;
//}
//
//std::string qg::getFileNameFromUrl (std::string url) {
//    return url.substr (url.rfind('/') + 1);
//}
//
//std::string qg::getFileTypeFromUrl (std::string url) {
//    return url.substr (url.rfind('.') + 1);
//}
