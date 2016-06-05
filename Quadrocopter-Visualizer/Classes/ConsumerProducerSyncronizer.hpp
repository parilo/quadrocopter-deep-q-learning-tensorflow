//
//  ConsumerProducerSyncronizer.hpp
//  Quadrocopter-Visualizer
//
//  Created by anton on 29/05/16.
//
//

#ifndef ConsumerProducerSyncronizer_hpp
#define ConsumerProducerSyncronizer_hpp

#include <functional>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ConsumerProducerSyncronizer {
public:

	ConsumerProducerSyncronizer (int numOfProducers);
	
	void reportProducerDone (int index);
	void reportConsumerDone ();
	void waitProducers ();
	void waitConsumer (int index);

private:

	int numOfProducers;

	std::mutex mtxProducersDone;
	std::vector<bool> producersDone;
	std::mutex mtxConsumer;
    std::condition_variable cvConsumer;

	std::mutex mtxProducersMayProceed;
	std::vector<bool> producersMayProceed;
	std::mutex mtxProducers;
    std::condition_variable cvProducers;

	std::mutex mtxLastProducerWait;

	bool isProducersDone ();
	bool isProducerMayProceed (int index);
	void setProducerMayProceed (int index, bool mayProceed);
	void setProducersMayProceed ();
	void setProducersNotDone ();

};

#endif /* ConsumerProducerSyncronizer_hpp */
