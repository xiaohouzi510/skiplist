all : main

main : SkipList.cpp main.cpp Rank.cpp
	g++ -g -o $@ $^

clean :
	rm -rf main core.*