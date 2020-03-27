CXXFLAGS = -Wall -g
OBJECTS = Task.o main.o TaskCreator.o Simulator.o Scheduler.o
SRC = Task.cpp main.cpp TaskCreator.cpp Simulator.cpp Scheduler.cpp
INC = Task.h
TARGET = simulator.elf

$(TARGET): $(OBJECTS) 
	g++ -o $@ $^

%.o: %.cpp $(INC)
	g++ $(CXXFLAGS) -c $< 

clean:
	rm -f $(OBJECTS) $(TARGET)

