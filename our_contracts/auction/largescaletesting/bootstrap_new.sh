#!/bin/bash

Public_key=EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

Name="user"

N=5 # Number of accounts

#Create accounts
cleos create account eosio auction $Public_key

for i in $(seq 1 $N);
do
  User=${Name}$i
  cleos create account eosio ${User} $Public_key
done

#Deploy the contract
cleos set contract auction . --abi auction.abi -p auction@active
