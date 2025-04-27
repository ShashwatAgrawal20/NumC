#!/usr/bin/env bash

echo "Cleaning project..."
make clean

echo "Building project..."
make

if [ ! -f numc_example ]; then
  echo "Executable numc_example not found!"
  exit 1
fi

echo "Running the executable to collect profiling data..."
./numc_example > /dev/null 2>&1

if [ ! -f gmon.out ]; then
  echo "gmon.out file not found! Profiling data might not have been generated."
  exit 1
fi

echo "Generating performance report with gprof..."
gprof numc_example gmon.out > gprof_report.txt

echo "Performance report generated: gprof_report.txt"
echo "Opening the report"
sleep 1
nvim gprof_report.txt
