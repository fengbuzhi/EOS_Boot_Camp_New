#!/bin/bash

eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen

cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio vt EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio weicheng EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio aanuo EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio zejia EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio alice EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos set contract eosio.token . --abi eosio.token.abi -p eosio.token@active

cleos push action eosio.token create '[ "vt", "100000 VTOKEN"]' -p eosio.token@active

cleos push action eosio.token issue '[ "vt", "10000 VTOKEN", "Initial Token Offering" ]' -p vt@active

cleos push action eosio.token transfer '[ "vt", "weicheng", "200 VTOKEN", "Send 200 VTOKEN to Weicheng" ]' -p vt@active
cleos push action eosio.token transfer '[ "vt", "aanuo", "200 VTOKEN", "Send 200 VTOKEN to Aanuo" ]' -p vt@active
cleos push action eosio.token transfer '[ "vt", "zejia", "200 VTOKEN", "Send 200 VTOKEN to Zejia" ]' -p vt@active
cleos push action eosio.token transfer '[ "vt", "alice", "200 VTOKEN", "Send 200 VTOKEN to Alice" ]' -p vt@active
cleos push action eosio.token transfer '[ "vt", "bob", "200 VTOKEN", "Send 200 VTOKEN to Bob" ]' -p vt@active

cleos get currency balance eosio.token weicheng VTOKEN
cleos get currency balance eosio.token aanuo VTOKEN
cleos get currency balance eosio.token zejia VTOKEN
cleos get currency balance eosio.token alice VTOKEN
cleos get currency balance eosio.token bob VTOKEN
