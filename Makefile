CXX = g++
CXXFLAGS = -Wall -std=c++17 -pthread -Iinclude

TARGETS = main director supplier worker

# Domyślna reguła (uruchamiana po wpisaniu 'make')
all: $(TARGETS)

main: src/main.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o main src/main.cpp src/utils.cpp

director: src/director.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o director src/director.cpp src/utils.cpp

supplier: src/supplier.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o supplier src/supplier.cpp src/utils.cpp

worker: src/worker.cpp src/utils.cpp
	$(CXX) $(CXXFLAGS) -o worker src/worker.cpp src/utils.cpp

clean:
	# Usuwamy binarki, pliki .exe (dla Windowsa), ale też logi i pliki stanu!
	rm -f $(TARGETS) *.o *.exe simulation_log.txt warehouse_state.dat