/*!
 *	\brief			A simulator of the 'The Bear and Honeybees Problem'
 *	\description	
 *	\author			Christian Magnerfelt
 *	\email			magnerf@kth.se
 */
#include <cstdlib>

#include <pthread.h>
#include <semaphore.h>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

const bool IS_SHARED = 1;
const std::size_t MAX_HONEYBEES = 100;
const std::size_t MAX_HONEY = 100;

std::size_t g_numHoneybees;
std::size_t g_numHoney;

sem_t full;			// Semaphore counter for amount of honey in pot
sem_t empty;		// Semaphore flag if pot is empty

void * honeyBee(void * param);
void * bear(void * param);

std::vector<int> g_pot;
sem_t mutex;

/*
 *	\params	argv	{ program name } { number of honeybees } { amount of honey }
 */
int main(int argc, const char * argv[])
{
	
	// Read and store arguments
	g_numHoneybees = MAX_HONEYBEES;
	g_numHoney = MAX_HONEY;
	if(argc > 1)
	{
		// Store arguments in a vector
		std::vector<std::string> args(argv, argv + argc);
	
		std::istringstream ss (args[1]);
		ss >> g_numHoneybees;
		if(ss.fail())
			g_numHoneybees = MAX_HONEYBEES; // If input fails set to max
			
		if(argc > 2)
		{
			std::istringstream ss (args[2]);
			ss >> g_numHoney;
			if(ss.fail())
				g_numHoney = MAX_HONEY;	// If input fails set to max
		}
			
		std::cout << "Number of Honeybees : " << g_numHoneybees << std::endl;
		std::cout << "Amount of Honey : " << g_numHoney << std::endl;
	}
	
	// Initialize threads attributes
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	
	// Initialize semaphores
	sem_init(&full, IS_SHARED, 0);
	sem_init(&empty, IS_SHARED, g_numHoney);
	sem_init(&mutex, IS_SHARED, 1);
	
	// Fills pot with honey
	g_pot.reserve(g_numHoney);
	
	// Init workers
	std::vector<pthread_t> workers(g_numHoneybees + 1);
	
	// Create bear worker thread
	pthread_create(&workers[0], &attr, bear, reinterpret_cast<void*>(0));
	
	// Create honeybee worker threads
	for(std::size_t i = 1; i < workers.size(); ++i)
	{
		pthread_create(&workers[i], &attr, honeyBee, reinterpret_cast<void*>(i));
	}
	
	// join all workers
	for(std::size_t i = 0; i < workers.size(); ++i)
	{
		pthread_join(workers[i], NULL);
	}
	return 0;
}
/*
 *	\brief	Eat honey from pot
 */
void * bear(void * param)
{
	std::size_t id = reinterpret_cast<std::size_t>(param);
	while(true)
	{
		// Wait until pot is full
		sem_wait(&full);
		
		// Mutal exclusion on shared resource pot
		sem_wait(&mutex);
		g_pot.clear();
		std::cout << "Bear " << id << " : Ate " << g_numHoney << " amount of Honey" << std::endl; 	
		sem_post(&mutex);
		
		// Signal honeybees that pot is empty
		for(std::size_t i = 0; i < g_numHoney; ++i)
		{
			sem_post(&empty);
		}
	}
	return 0;
}
/*
 *	\brief	Fills pot with honey
 */
void * honeyBee(void * param)
{
	std::size_t id = reinterpret_cast<std::size_t>(param);
	while(true)	
	{
		// Wait until the pot is empty
		sem_wait(&empty);
		
		// Mutal exclusion on shared resource pot
		sem_wait(&mutex);
	
		g_pot.push_back(rand() % 10);	// Add honey with random quality ( Symbolic )
		std::cout << "honeyBee " << id << " : added one unit of honey to the pot" << std::endl;
		
		// Signal bear if pot is full
		if(g_pot.size() == g_numHoney)
		{
			sem_post(&full);
			std::cout << "honeyBee " << id << " : The honey is served!" << std::endl;
		}
		sem_post(&mutex);	
	}
	return 0;
}
