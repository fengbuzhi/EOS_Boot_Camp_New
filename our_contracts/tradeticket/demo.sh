#!/bin/bash
cd /workspace/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
cleos set contract eosio.token . --abi eosio.token.abi -p eosio.token@active
cleos push action eosio.token create '[ "vtfootball", "1000000000.0000 SYS"]' -p eosio.token@active
cleos create account eosio vtfootball EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos push action eosio.token issue '[ "vtfootball", "100000000.0000 SYS", "memo" ]' -p vtfootball@active

cleos push action eosio.token transfer '[ "vtfootball", "weicheng", "250.0000 SYS", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token weicheng SYS
cleos push action eosio.token transfer '[ "vtfootball", "zejia", "250.0000 SYS", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token zejia SYS
cleos push action eosio.token transfer '[ "vtfootball", "aanuo", "250.0000 SYS", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token aanuo SYS
cleos push action eosio.token transfer '[ "vtfootball", "ticket1", "10.0000 SYS", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token ticket1 SYS
cleos push action eosio.token transfer '[ "vtfootball", "ticket2", "10.0000 SYS", "m" ]' -p vtfootball@active
cleos get currency balance eosio.token ticket2 SYS

