The Bear and Honeybees Problem
------------------------

	Description	: 
 	Given are n honeybees and a hungry bear. They share a pot of honey. 
 	The pot is initially empty; its capacity is H portions of honey. The bear sleeps until the pot is full, 	
 	then eats all the honey and goes back to sleep. Each bee repeatedly gathers one portion of honey and puts 
 	it in the pot; the bee who fills the pot awakens the bear. Syncronization is handled by semaphores and bear and honeybees 
 	are simulated by workers threads using the pthreads library.
 	
 	Author : 
 	Christian Magnerfelt

	Email :
	magnerf@kth.se
	
	Usage :
		./BearHoneybees { num of honeybees } { amount of honey }
	
	Compiler:
		requires gcc 4.4.7
	
	Build :
		make debug
		make release
