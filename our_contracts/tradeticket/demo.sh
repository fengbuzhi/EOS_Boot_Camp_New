#!/bin/bash
cd /workspace/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
cleos set contract eosio.token . --abi eosio.token.abi -p eosio.token@active
cleos push action eosio.token create '[ "vtfootball", "1000000000 VTOKEN"]' -p eosio.token@active
cleos create account eosio vtfootball EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos push action eosio.token issue '[ "vtfootball", "100000000 VTOKEN", "memo" ]' -p vtfootball@active

cleos push action eosio.token transfer '[ "vtfootball", "weicheng", "250 VTOKEN", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token weicheng VTOKEN
cleos push action eosio.token transfer '[ "vtfootball", "zejia", "250 VTOKEN", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token zejia VTOKEN
cleos push action eosio.token transfer '[ "vtfootball", "aanuo", "250 VTOKEN", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token aanuo VTOKEN
cleos push action eosio.token transfer '[ "vtfootball", "ticket1", "10 VTOKEN", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token ticket1 VTOKEN
cleos push action eosio.token transfer '[ "vtfootball", "ticket2", "10 VTOKEN", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token ticket2 VTOKEN

