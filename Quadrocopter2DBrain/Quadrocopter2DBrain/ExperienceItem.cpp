//
//  ExperienceItem.cpp
//  QuadrocopterBrain
//
//  Created by anton on 30/01/16.
//  Copyright © 2016 anton. All rights reserved.
//

#include "ExperienceItem.hpp"

int idSeq = 1;

ExperienceItem::ExperienceItem () :
	id (idSeq++),
	prevStates(ObservationSeqLimited()),
	nextStates(ObservationSeqLimited()),
	reward(0),
	rewardLambda(0),
	action(0)
	{}

ExperienceItem::ExperienceItem (
	const ObservationSeqLimited& prevStates,
	const ObservationSeqLimited& nextStates,
	double reward,
	long action
):

id(idSeq++),
prevStates(prevStates),
nextStates(nextStates),
reward(reward),
rewardLambda(reward),
action(action)

{}

ExperienceItem::ExperienceItem (
	const ObservationSeqLimited& prevStates,
	const ObservationSeqLimited& nextStates,
	double reward,
	const std::vector<float>& actionCont
):

id(idSeq++),
prevStates(prevStates),
nextStates(nextStates),
reward(reward),
rewardLambda(reward),
actionCont(actionCont)

{}

ExperienceItem::ExperienceItem (
	const ObservationSeqLimited& prevStates,
	const ObservationSeqLimited& nextStates,
	const ObservationSeqLimited& reward,
	const ObservationSeqLimited& actionCont
) :
	id(idSeq++),
	prevStates(prevStates),
	nextStates(nextStates),
	lstmRewards(reward),
	lstmActions(actionCont)
{}
