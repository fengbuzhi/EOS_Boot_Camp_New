#!/bin/bash

Name="user"
Base_id=111111110

N=5 # Number of accounts to bid

for i in $(seq 1 $N);
do
  User=${Name}$i
  ID=$((Base_id+i))
  price=$(($RANDOM % 30 + 5)) #Generate a price between 5 to 30
  cleos push action auction placebid '[$User, $ID, $price]' -p User@active
done


