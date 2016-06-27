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

bool Lib::getLineSegmentsIntersection (
	float p0_x, float p0_y, //first line points
	float p1_x, float p1_y,
	float p2_x, float p2_y, //sencond lise points
	float p3_x, float p3_y,
	float& i_x, float& i_y,
	float& t//intersection point in part of length of first line
) {
    float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom;
    s10_x = p1_x - p0_x;
    s10_y = p1_y - p0_y;
    s32_x = p3_x - p2_x;
    s32_y = p3_y - p2_y;

    denom = s10_x * s32_y - s32_x * s10_y;
    if (denom == 0)
        return false; // Collinear
    bool denomPositive = denom > 0;

    s02_x = p0_x - p2_x;
    s02_y = p0_y - p2_y;
    s_numer = s10_x * s02_y - s10_y * s02_x;
    if ((s_numer < 0) == denomPositive)
        return false; // No collision

    t_numer = s32_x * s02_y - s32_y * s02_x;
    if ((t_numer < 0) == denomPositive)
        return false; // No collision

    if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
        return false; // No collision
    // Collision detected
    t = t_numer / denom;
    i_x = p0_x + (t * s10_x);
    i_y = p0_y + (t * s10_y);

    return true;
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