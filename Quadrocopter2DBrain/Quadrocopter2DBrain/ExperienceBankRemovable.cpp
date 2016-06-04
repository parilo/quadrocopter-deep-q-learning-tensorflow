//
//  ExperienceBankRemovable.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 22/05/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "ExperienceBankRemovable.hpp"
#include "Lib.hpp"

ExperienceBankRemovable::ExperienceBankRemovable () {
	experience.reserve(maxExperience);
	present.reserve(maxExperience);
	expItemPosById.reserve(maxExperience);
}

//static std::mutex storeExperienceMutex;

void ExperienceBankRemovable::store (const ExperienceItem& expItem) {
//	std::lock_guard<std::mutex> lock(storeExperienceMutex);
	
	if (expItemPosById.find(expItem.id) != expItemPosById.end()) {
		return;
	}
	
	int pos;
	if (experience.size() < maxExperience) {
		pos = (int) experience.size ();
		experience.push_back(expItem);
		present.push_back(1);
	} else {
		pos = experienceI;
		experience [experienceI] = expItem;
		present [experienceI] = 1;
		experienceI++;
		experienceI %= maxExperience;
	}

	expItemPosById [expItem.id] = pos;
	size++;

//	std::cerr << "--- ExperienceBank " << name << ": " << experienceI << " / " << experience.size() << std::endl;
}

void ExperienceBankRemovable::store (const std::vector<const ExperienceItem*>& minibatch) {
	for (auto item : minibatch) {
		store (*item);
	}
}

void ExperienceBankRemovable::remove (const ExperienceItem& expItem) {
//	std::lock_guard<std::mutex> lock(storeExperienceMutex);
	
	auto posIt = expItemPosById.find(expItem.id);

	if ( posIt != expItemPosById.end() ) {
		int pos = posIt->second;
		present [pos] = 0;
		size--;
		expItemPosById.erase(posIt);
	}
}

void ExperienceBankRemovable::remove (const std::vector<const ExperienceItem*>& minibatch) {
	for (auto item : minibatch) {
		remove (*item);
	}
}

int ExperienceBankRemovable::getSize () const {
	return size;
}

void ExperienceBankRemovable::setName (const std::string& name) {
	this->name = name;
}

const ExperienceItem& ExperienceBankRemovable::getRandomExp () const {
	int pos;
	do {
		pos = Lib::randInt(0, (int)experience.size()-1);
	} while (present [pos] == 0);
	return experience [pos];
}

void getMinibatch (
	const ExperienceBankRemovable& bank,
	std::vector<const ExperienceItem*>& minibatch,
	int minibatchSize
) {
	minibatch.clear();

	for (int i=0; i<minibatchSize; i++) {
		minibatch.push_back (&bank.getRandomExp());
	}
}

void getMinibatchWithProbabilities (
	const ExperienceBankRemovable& bank1,
	const ExperienceBankRemovable& bank2,
	const ExperienceBankRemovable& bank3,
	float probability1,
	float probability2,
	float probability3,
	std::vector<const ExperienceItem*>& minibatch,
	int minibatchSize
) {
	float p1 = probability1;
	float p2 = p1 + probability2;
	float p3 = p2 + probability3;
	
	int size1 = bank1.getSize();
	int size2 = bank2.getSize();
	int size3 = bank3.getSize();

	minibatch.clear();

	for (int i=0; i<minibatchSize; i++) {
		float rand = Lib::randFloat (0.0, 1.0);
		if (rand < p1 && size1 > 50 ) {
			minibatch.push_back (&bank1.getRandomExp());
		} else if (rand < p2 && size2 > 50 ) {
			minibatch.push_back(&bank2.getRandomExp());
		} else if (rand < p3 && size3 > 50 ) {
			minibatch.push_back(&bank3.getRandomExp());
		}
	}
}

void getMinibatchWithProbabilities (
	const ExperienceBankRemovable& bank1,
	const ExperienceBankRemovable& bank2,
	float probability1,
	float probability2,
	std::vector<const ExperienceItem*>& minibatch,
	int minibatchSize
) {
	float p1 = probability1;
	float p2 = p1 + probability2;
	
	int size1 = bank1.getSize();
	int size2 = bank2.getSize();

	minibatch.clear();

	for (int i=0; i<minibatchSize; i++) {
		float rand = Lib::randFloat (0.0, 1.0);
		if (rand < p1 && size1 > 50 ) {
			minibatch.push_back (&bank1.getRandomExp());
		} else if (rand < p2 && size2 > 50 ) {
			minibatch.push_back(&bank2.getRandomExp());
		}
	}
}
