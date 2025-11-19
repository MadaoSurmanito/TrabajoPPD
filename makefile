CXX = gcc
CXXFLAGS = -fopenmp

SRC = $(wildcard *.c)
OBJ = $(SRC:.cu=.o)
EXE = programa

%.o: %.cu
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ 

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(OBJ) $(EXE)