#!/bin/bash

cleos push action carpool login '["weicheng"]' -p weicheng@active
cleos push action carpool login '["aanuo"]' -p aanuo@active
cleos push action carpool login '["zejia"]' -p zejia@active

echo "Initial post"
cleos push action carpool addpost '["aanuo", "VT sports center", "5 mins to the court", 4, "5 dollars"]' -p aanuo@active
echo "Update post due to traffic conjestion."
cleos push action carpool editpost '["weicheng", 5, "10 mins to the court"]' -p weicheng@active
cleos push action carpool hopride '["zejia", 5, 2]' -p zejia@active

