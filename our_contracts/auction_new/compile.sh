#!/bin/bash
cd /workspace/EOS_Boot_Camp_New/our_contracts/auction

#Compile auction
eosio-cpp --abigen auction.cpp -o auction.wasm