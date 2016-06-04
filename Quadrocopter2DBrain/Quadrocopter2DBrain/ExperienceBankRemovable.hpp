//
//  ExperienceBankRemovable.hpp
//  Quadrocopter2DBrain
//
//  Created by anton on 22/05/16.
//  Copyright © 2016 anton. All rights reserved.
//

#ifndef ExperienceBankRemovable_hpp
#define ExperienceBankRemovable_hpp

#include "ExperienceItem.hpp"
#include <unordered_map>

class ExperienceBankRemovable {
public:
	
	ExperienceBankRemovable ();
	
	void store (const ExperienceItem& expItem);
	void store (const std::vector<const ExperienceItem*>& minibatch);
	void remove (const ExperienceItem& expItem);
	void remove (const std::vector<const ExperienceItem*>& minibatch);
	
	void setName (const std::string& name);
	
	int getSize () const;
	const ExperienceItem& getRandomExp () const;
	
private:

	std::string name;
	
	std::vector<ExperienceItem> experience;
	std::vector<short> present;//1 - if present, 0 - if not
	std::unordered_map<int, int> expItemPosById;
	int size = 0;
	
	int experienceI = 0;

	static const int maxExperience = 1000000;

};

void getMinibatch (
	const ExperienceBankRemovable& bank,
	std::vector<const ExperienceBankRemovable*>& minibatch,
	int minibatchSize
);

void getMinibatchWithProbabilities (
	const ExperienceBankRemovable& bank1,
	const ExperienceBankRemovable& bank2,
	const ExperienceBankRemovable& bank3,
	float probability1,
	float probability2,
	float probability3,
	std::vector<const ExperienceItem*>& minibatch,
	int minibatchSize
);

void getMinibatchWithProbabilities (
	const ExperienceBankRemovable& bank1,
	const ExperienceBankRemovable& bank2,
	float probability1,
	float probability2,
	std::vector<const ExperienceItem*>& minibatch,
	int minibatchSize
);

#endif /* ExperienceBankRemovable_hpp */
