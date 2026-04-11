#!/bin/bash

# Test script for jst - tests all JSON files with all themes

echo "========================================="
echo "Testing jst with all JSON files and themes"
echo "========================================="
echo ""

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counters
TOTAL=0
PASSED=0
FAILED=0

# Test function
test_file() {
    local json_file=$1
    local theme_file=$2
    
    TOTAL=$((TOTAL + 1))
    
    if [ -f "$theme_file" ]; then
        ./jst "$json_file" -C "$theme_file" > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✓${NC} $json_file -C $theme_file"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} $json_file -C $theme_file"
            FAILED=$((FAILED + 1))
        fi
    else
        ./jst "$json_file" > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}✓${NC} $json_file (default)"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} $json_file (default)"
            FAILED=$((FAILED + 1))
        fi
    fi
}

# Get all JSON files in tests directory
JSON_FILES=$(find tests -name "*.json" 2>/dev/null | sort)

# Get all theme files in themes directory
THEME_FILES=$(find themes -name "*.json" 2>/dev/null | sort)

echo "Testing basic functionality..."
echo "-----------------------------------------"
for json in $JSON_FILES; do
    test_file "$json"
done

echo ""
echo "Testing with themes..."
echo "-----------------------------------------"
for json in $JSON_FILES; do
    for theme in $THEME_FILES; do
        test_file "$json" "$theme"
    done
done

echo ""
echo "========================================="
echo "Test Results:"
echo "========================================="
echo -e "Total:   $TOTAL"
echo -e "${GREEN}Passed:  $PASSED${NC}"
echo -e "${RED}Failed:  $FAILED${NC}"
echo "========================================="

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed!${NC}"
    exit 1
fi