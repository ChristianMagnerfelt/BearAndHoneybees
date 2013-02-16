debug: bear_honeybees.cpp
	g++ -g -Wall -Wextra -O0 -o BearHoneybees bear_honeybees.cpp -lpthread -std=c++0x
	
release: bear_honeybees.cpp
	g++ -Wall -Wextra -O3 -o BearHoneybees bear_honeybees.cpp -lpthread -std=c++0x
clean:
	rm BearHoneybees
