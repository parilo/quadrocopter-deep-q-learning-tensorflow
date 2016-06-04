//
//  ConsumerProducerSyncronizer.cpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 29/05/16.
//
//

#include "ConsumerProducerSyncronizer.hpp"

ConsumerProducerSyncronizer::ConsumerProducerSyncronizer (int numOfProducers) :
	numOfProducers(numOfProducers)
{
	producersDone.assign(numOfProducers, false);
	producersMayProceed.assign(numOfProducers, false);
}

void ConsumerProducerSyncronizer::reportProducerDone (int index) {
	mtxProducersDone.lock();
	producersDone [index] = true;
	mtxProducersDone.unlock();
	std::unique_lock<std::mutex> lock(mtxConsumer);
	cvConsumer.notify_one();
}

void ConsumerProducerSyncronizer::reportConsumerDone () {
	setProducersNotDone();
	setProducersMayProceed();
	std::unique_lock<std::mutex> lock(mtxProducers);
	cvProducers.notify_all();
}

void ConsumerProducerSyncronizer::waitProducers () {
	std::unique_lock<std::mutex> lock(mtxConsumer);
	while (!isProducersDone()) {
		cvConsumer.wait(lock);
	}
}

void ConsumerProducerSyncronizer::waitConsumer (int index) {
	std::unique_lock<std::mutex> lock(mtxProducers);
	while (!isProducerMayProceed(index)) {
		cvProducers.wait(lock);
	}
	setProducerMayProceed(index, false);
}

bool ConsumerProducerSyncronizer::isProducersDone () {
	std::lock_guard<std::mutex> lock (mtxProducersDone);
	for (int i=0; i<numOfProducers; i++) {
		if (!producersDone [i]) return false;
	}
	return true;
}

void ConsumerProducerSyncronizer::setProducersMayProceed () {
	std::lock_guard<std::mutex> lock(mtxProducersMayProceed);
	producersMayProceed.assign(numOfProducers, true);
}

void ConsumerProducerSyncronizer::setProducersNotDone () {
	std::lock_guard<std::mutex> lock(mtxProducersDone);
	producersDone.assign(numOfProducers, false);
}

bool ConsumerProducerSyncronizer::isProducerMayProceed (int index) {
	std::lock_guard<std::mutex> lock(mtxProducersMayProceed);
	return producersMayProceed [index];
}

void ConsumerProducerSyncronizer::setProducerMayProceed (int index, bool mayProceed) {
	std::lock_guard<std::mutex> lock(mtxProducersMayProceed);
	producersMayProceed [index] = mayProceed;
}
