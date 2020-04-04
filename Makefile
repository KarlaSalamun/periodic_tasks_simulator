CXXFLAGS = -Wall -Wextra -g
SRC = $(wildcard *.cpp)
INC = $(wildcard *.h)
OBJECTS = $(SRC:.cpp=.o)
TARGET = simulator.elf

$(TARGET): $(OBJECTS) 
	g++ -o $@ $^

%.o: %.cpp $(INC)
	g++ $(CXXFLAGS) -c $< 

clean:
	rm -f $(OBJECTS) $(TARGET)

