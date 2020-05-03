#!/bin/bash
cd /home/aanu/workdirectory/eosio.cdt/bootcamp/EOS_Boot_Camp_New/our_contracts/auction

#Compile auction
eosio-cpp --abigen auction.cpp -o auction.wasm

cd /home/aanu/workdirectory/eosio.cdt/bootcamp/EOS_Boot_Camp_New/our_contracts/tradeticket

#Compile auction
eosio-cpp --abigen tradeticket.cpp -o tradeticket.wasm

cd /home/aanu/workdirectory/eosio.cdt/bootcamp/EOS_Boot_Camp_New/our_contracts/auction

cleos create account eosio auction EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio weicheng EOS7nXVLLrEda6Xr8FG3hxyc47LsC19Us9avhUK3uEPiPghdJPG2c
cleos create account eosio zejia EOS8548cE8JioJT9ZRRdPqQ3KR6QmN6fH5ssE2ULKiBfbrmz7uYnX
cleos create account eosio tradeticket EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cd /home/aanu/workdirectory/eosio.cdt/bootcamp/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
cleos set contract eosio.token . --abi eosio.token.abi -p eosio.token@active
cleos push action eosio.token create '[ "vtfootball", "1000000000 VTOKEN"]' -p eosio.token@active
cleos create account eosio vtfootball EOS7dJcV4RBZ9YBrQuMwCSSu1RYt1r2TFdrHUXpA7EVzyv6yj6Wg7
cleos push action eosio.token issue '[ "vtfootball", "100000000 VTOKEN", "Initial Token Offering" ]' -p vtfootball@active

cleos push action eosio.token transfer '[ "vtfootball", "weicheng", "200 VTOKEN", "Send 200 VTOKEN to Weicheng" ]' -p vtfootball@active
cleos push action eosio.token transfer '[ "vtfootball", "zejia", "200 VTOKEN", "Send 200 VTOKEN to Zejia" ]' -p vtfootball@active

cleos set account permission auction active --add-code

cleos set account permission vtfootball active \
'{"threshold": 1,"keys": [{"key": "EOS7dJcV4RBZ9YBrQuMwCSSu1RYt1r2TFdrHUXpA7EVzyv6yj6Wg7","weight": 1}],"accounts": [{"permission":{"actor":"auction","permission":"eosio.code"},"weight":1}]}' \
owner -p vtfootball

#Deploy the contract 
cleos set contract auction . --abi auction.abi -p auction@active

cd /home/aanu/workdirectory/eosio.cdt/bootcamp/EOS_Boot_Camp_New/our_contracts/tradeticket
#Deploy the contract 
cleos set contract tradeticket . --abi tradeticket.abi -p tradeticket@active

cd /home/aanu/workdirectory/eosio.cdt/bootcamp/EOS_Boot_Camp_New/our_contracts/auction
