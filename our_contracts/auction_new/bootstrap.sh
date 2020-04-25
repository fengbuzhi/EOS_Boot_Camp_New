#!/bin/bash

#Create account
cleos create account eosio auction EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
#cleos create account eosio weicheng EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
#cleos create account eosio aanuo EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
#cleos create account eosio zejia EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
#cleos create account eosio alice EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
#cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 

#Add the eosio.code permission to the contract's account's active permission
cleos set account permission auction active --add-code
cleos set account permission weicheng active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"auction","permission":"eosio.code"},"weight":1}]}' owner -p weicheng
cleos set account permission aanuo active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"auction","permission":"eosio.code"},"weight":1}]}' owner -p aanuo
cleos set account permission zejia active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"auction","permission":"eosio.code"},"weight":1}]}' owner -p zejia

cleos set account permission ticket1 active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"auction","permission":"eosio.code"},"weight":1}]}' owner -p ticket1
cleos set account permission ticket2 active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"auction","permission":"eosio.code"},"weight":1}]}' owner -p ticket2
cleos set account permission ticket3 active '{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"auction","permission":"eosio.code"},"weight":1}]}' owner -p ticket3

#Deploy the contract 
cleos set contract auction . --abi auction.abi -p auction@active
