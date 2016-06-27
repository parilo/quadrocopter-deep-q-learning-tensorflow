//
//  Lib.h
//  QuizGeek
//
//  Created by anton on 13.11.14.
//
//

#ifndef __QuizGeek__Lib__
#define __QuizGeek__Lib__


class Lib {

public:
	
	static void randomize ();
	static int randInt (int min, int max);
	static float randFloat (float LO, float HI);
	static long long getTimestampInMillis ();
	
	static float degToRad (float deg);
	static float radToDeg (float rad);
	
	static bool getLineSegmentsIntersection (
		float p0_x, float p0_y, //first line points
		float p1_x, float p1_y,
		float p2_x, float p2_y, //sencond lise points
		float p3_x, float p3_y,
		float& i_x, float& i_y,
		float& t//intersection point in part of length of first line
	);
	
};

//    std::string to_string(int value);
//    std::string to_string(unsigned value);
//    std::string to_string(long value);
//    std::string to_string(unsigned long value);
//    std::string to_string(long long value);
//    std::string to_string(unsigned long long value);
//    std::string to_string(float value);
//    std::string to_string(double value);
//    std::string to_string(long double value);
//    
//    int stoi (std::string str);
//    long long stoll (std::string str);
//    
//    /// get file name with type for a given url
//    std::string getFileNameFromUrl (std::string url);
//    
//    /// get file name type for a given url
//    std::string getFileTypeFromUrl (std::string url);
	
#endif /* defined(__QuizGeek__Lib__) */
