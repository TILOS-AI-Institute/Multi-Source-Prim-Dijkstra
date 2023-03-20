## Prerequisites

1. C++ 11 or higher  
2. OpenMP  
3. Boost Library  

## Usage

In this directory, you will find an implementation of STT (stt+DAS) and PDRev (PD+HVW+DAS) that support multi-source. 

### STT: 
Navigate to the *STT* folder.  
RNG.cpp is a script that randomly generates T nets of size N in the $[0…M] \times [0…M]$ bounding box.

Compile the RNG script with:   

    g++ -std=c++11 RNG.cpp -O3 -o rng

Generate random points using the following command:  

    ./rng -M 1000 -N n -T t > test.txt

To use MSPD, navigate to the *MSPD* folder.  
Compile MSPD with:  

    g++ -std=c++11 STT.cpp -O3 -o STT_MSPD

Use MSPD with:  

    ./STT_MSPD -f test.in -out test.out

To use MSS, navigate to the *MSS* folder.  
Compile MSS with:  

    g++ -std=c++11 STT.cpp -O3 -o STT_MSS

Use MSS with:

    ./STT_MSS -f test.in -out test.out -lambda l -KNN k


### PDRev: 
Navigate to the *PDRev* folder.  
RNG.cpp is a script that randomly generates T nets of size N in the $[0…M] \times [0…M]$ bounding box.

Compile the RNG script with:   

    g++ -std=c++11 RNG.cpp -O3 -o rng

Generate random points using the following command:  

    ./rng -M 1000 -N n -T t > test.txt

To use MSPD, navigate to the *MSPD* folder.  
Compile MSPD with:  

    g++ -std=c++11 main.cpp graph.cpp argument.cpp mymeasure.cpp mystring.cpp -fopenmp -O3 -o PDRev_MSPD

Use MSPD with:  

    ./PDRev_MSPD -f test.in -out test.out

To use MSS, navigate to the *MSS* folder.  
Compile MSS with:  

    g++ -std=c++11 main.cpp graph.cpp argument.cpp mymeasure.cpp mystring.cpp -fopenmp -O3 -o PDRev_MSS

Use MSS with:

    ./PDRev_MSS -f test.in -out test.out -lambda l -KNN k


