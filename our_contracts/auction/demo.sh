#!/bin/bash

cleos push action auction placebid '["weicheng", 999997777, 100]' -p weicheng@active
cleos push action auction placebid '["aanuo", 111111111, 200]' -p aanuo@active
cleos push action auction placebid '["zejia", 222222222, 300]' -p zejia@active

cleos push action auction printwinners '["weicheng"]' -p weicheng@active
