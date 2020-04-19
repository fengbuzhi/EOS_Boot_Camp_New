#!/bin/bash

cleos push action auction placebid '["weicheng", 999997777, 10]' -p weicheng@active
cleos push action auction placebid '["aanuo", 111111111, 12]' -p aanuo@active
cleos push action auction placebid '["zejia", 222222222, 14]' -p zejia@active

cleos push action auction printwinners '["weicheng"]' -p weicheng@active
