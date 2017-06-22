#!/usr/bin/env bash

echo "Running executable on test data..."
./cmake-build-debug/compressibility TESTS/"test1"
./cmake-build-debug/compressibility TESTS/"test2"
cd TESTS
    echo "Starting data comparison"
    cmp --silent test1.OUT results1 || echo "ERROR: TEST 1 RESULTS ARE WRONG -- NOT RELATED TO HE or H2 LOOPS"
    cmp --silent test2.OUT results2 || echo "ERROR: TEST 2 RESULTS ARE WRONG -- CHECK HE and H2 LOOPS"
    for test in test1.OUT test2.OUT; do
        echo "Making graphs for ${test}"
        split -l 11 ${test} DATA
        python ../graph_most.py DATAaa ${test} ${test}
        python ../graph_most.py DATAab ${test} ${test}
        python ../graph_most.py DATAac ${test} ${test}
        python ../graph_most.py DATAad ${test} ${test}
        python ../graph_most.py DATAae ${test} ${test}
        python ../graph_most.py DATAaf ${test} ${test}
        rm DATA*
    done
cd ..
echo "Done! If there isn't any text in ALL-CAPS above, results are as expected."
echo "cd TESTS to see example graphs from test data"
