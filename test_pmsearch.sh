#!/bin/bash

# Colors for pretty output
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# 1. Rebuild the project
echo "Building project..."
make clean && make

if [ $? -ne 0 ]; then
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi

# 2. Create test files
echo "Creating test data..."
echo -n "ARISTA--ARISTA--ARISTA" > test_pattern.txt
touch empty.txt
echo "This is a secret message" > secret.txt

# 3. Define a helper function to run tests
# Usage: run_test "name" "pattern" "file" "expected_count"
run_test() {
    local name=$1
    local pattern=$2
    local file=$3
    local expected=$4

    echo -n "Testing $name... "
    
    # Run pmsearch and count the number of output lines
    local count=$(./pmsearch "$pattern" "$file" 2>/dev/null | grep -c "$file")
    
    if [ "$count" -eq "$expected" ]; then
        echo -e "${GREEN}PASS${NC} ($count matches)"
    else
        echo -e "${RED}FAIL${NC} (Expected $expected, got $count)"
    fi
}

# 4. Execute the Test Suite
echo "---------------------------"
run_test "Multiple Matches" "ARISTA" "test_pattern.txt" 3
run_test "Single Match" "secret" "secret.txt" 1
run_test "No Match" "banana" "secret.txt" 0
run_test "Empty File" "anything" "empty.txt" 0
run_test "Case Sensitivity" "arista" "test_pattern.txt" 0
echo "---------------------------"

# 5. Clean up test files
rm test_pattern.txt empty.txt secret.txt


########### MULTIPLE FILES ############

# 1. Create a suite of different files
echo "Preparing multi-file test environment..."
echo "needle" > file1.txt
echo "needle needle" > file2.txt
echo "haystack" > file3.txt
echo "needle needle needle" > file4.txt

# 2. Enhanced Test Function
# Usage: run_multi_test "Name" "Pattern" "Expected Total Matches" "Files..."
run_multi_test() {
    local name=$1
    local pattern=$2
    local expected_total=$3
    shift 3
    local files=("$@")

    echo -n "Testing $name... "
    
    # Capture the full output
    local output=$(./pmsearch "$pattern" "${files[@]}" 2>/dev/null)
    
    # LOGIC FIX: 
    # We count lines that start with any filename followed by ':0x'
    # This ignores your "Hello..." header and the "Target string..." line.
    local actual_total=$(echo "$output" | grep -E "^.*:0x[0-9A-Fa-f]+" | wc -l)
    
    if [ "$actual_total" -eq "$expected_total" ]; then
        echo -e "${GREEN}PASS${NC} (Total matches:$actual_total)"
    else
        echo -e "${RED}FAIL${NC} (Expected $expected_total, got $actual_total)"
        echo "Check the raw output below:"
        echo "----------------------"
        echo "$output"
        echo "----------------------"
    fi
}

echo "---------------------------------------"

# Test 1: Single file (the baseline)
run_multi_test "Single File" "needle" 1 "file1.txt"

# Test 2: Multiple files with varying counts
# file1 (1) + file2 (2) + file4 (3) = 6 total
run_multi_test "Multiple Files (Mix)" "needle" 6 "file1.txt" "file2.txt" "file4.txt"

# Test 3: Files with zero matches included in the list
# file1 (1) + file3 (0) = 1 total
run_multi_test "Mix with No-Match File" "needle" 1 "file1.txt" "file3.txt"

# Test 4: All files
run_multi_test "Full Suite" "needle" 6 "file1.txt" "file2.txt" "file3.txt" "file4.txt"

echo "---------------------------------------"

# 3. Cleanup
rm file1.txt file2.txt file3.txt file4.txt