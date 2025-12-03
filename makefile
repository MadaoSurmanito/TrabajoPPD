CXX = gcc
CXXFLAGS = -fopenmp -lm

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXE = programa

%.o: %.cu
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ 

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(OBJ) $(EXE)