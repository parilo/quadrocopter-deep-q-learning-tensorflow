//
//  ExperienceBank.cpp
//  Quadrocopter2DBrain
//
//  Created by anton on 17/04/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "ExperienceBank.hpp"
#include "Lib.hpp"

ExperienceBank::ExperienceBank () {
	experience.resize(maxExperience);
}

//static std::mutex storeExperienceMutex;

void ExperienceBank::store (const ExperienceItem& expItem) {
//	std::lock_guard<std::mutex> lock(storeExperienceMutex);
	
	experience [experienceI] = expItem;
	experienceI++;
	experienceI %= maxExperience;
	if (size < maxExperience) size++;

//	std::cerr << "--- ExperienceBank " << name << ": " << experienceI << " / " << experience.size() << std::endl;
}

void ExperienceBank::store (const std::vector<const ExperienceItem*>& minibatch) {
	for (auto item : minibatch) {
		store (*item);
	}
}

int ExperienceBank::getSize () const {
	return size;
}

int ExperienceBank::getCurrentIndex () const {
	return experienceI;
}

void ExperienceBank::setName (const std::string& name) {
	this->name = name;
}

const ExperienceItem& ExperienceBank::getRandomExp () const {
	return experience [Lib::randInt(0, size-1)];
}

void getMinibatch (
	const ExperienceBank& bank,
	std::vector<const ExperienceItem*>& minibatch,
	int minibatchSize
) {
	minibatch.clear();

	for (int i=0; i<minibatchSize; i++) {
		minibatch.push_back (&bank.getRandomExp());
	}
}
