# --- Compiler and Flags ---
#OMP_NUM_THREADS - enviroment variable for declraing the number of threads 
CXX = g++
CXXFLAGS = -std=c++17 -O2 -I$(HOME)/opencv/install/include/opencv4
LDFLAGS  = -L$(HOME)/opencv/install/lib64 -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -fopenmp

# --- Target executable ---
TARGET = compare

# --- Source files ---
SRC = compare.cpp

# --- Default rule ---
all: $(TARGET)

# --- Build the executable ---
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# --- Clean rule ---
clean:
	rm -f $(TARGET)


