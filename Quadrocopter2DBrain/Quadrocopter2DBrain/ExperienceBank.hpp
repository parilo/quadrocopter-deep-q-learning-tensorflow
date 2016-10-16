//
//  ExperienceBank.hpp
//  Quadrocopter2DBrain
//
//  Created by anton on 17/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef ExperienceBank_hpp
#define ExperienceBank_hpp

#include "ExperienceItem.hpp"

class ExperienceBank {
public:
	
	ExperienceBank ();
	
	void store (const ExperienceItem& expItem);
	void store (const std::vector<const ExperienceItem*>& minibatch);
	
	void setName (const std::string& name);
	
	int getSize () const;
	int getCurrentIndex () const;
	const ExperienceItem& getRandomExp () const;
	
private:

	std::string name;
	
	std::vector<ExperienceItem> experience;
	
	int size = 0;
	int experienceI = 0;
	static const int maxExperience = 3000000;

};

void getMinibatch (
	const ExperienceBank& bank,
	std::vector<const ExperienceItem*>& minibatch,
	int minibatchSize
);

#endif /* ExperienceBank_hpp */
