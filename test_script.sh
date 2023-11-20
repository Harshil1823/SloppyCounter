#!/bin/bash

# Function to run the simulation
run_simulation() {
  echo "Running simulation with parameters: $@"
  time ./sloppySim "$@"
  echo "-----------------------------------"
}

# Test Cases
# Adjust these as needed for your specific testing requirements

# Test Case 1: Small number of threads
run_simulation 4 5 11 100 false true

# Test Case 2: Small number of threads and CPU/bound
run_simulation 4 5 11 100 true true

# Test Case 3: Large number of threads and IO bound 
run_simulation 30 5 11 100 false true

# Test Case 4: Large number of threads and CPU bound
run_simulation 30 5 11 100 true true


# Add more test cases as needed...

echo "Testing complete!"
