#!/bin/bash

cleos push action auction placebid '["weicheng", 999997777, "10 VTOKEN"]' -p weicheng@active
cleos push action auction placebid '["aanuo", 111111111, "12 VTOKEN"]' -p aanuo@active
cleos push action auction placebid '["zejia", 222222222, "14 VTOKEN"]' -p zejia@active

cleos push action auction printbids '["weicheng"]' -p weicheng@active
cleos push action auction printwinners '["weicheng"]' -p weicheng@active

cleos push action auction buyticket '["zejia", "ticket1"]' -p zejia@active
