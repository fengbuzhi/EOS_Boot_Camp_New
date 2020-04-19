#!/bin/bash

cd /workspace/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token
cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
eosio-cpp -I include -o eosio.token.wasm src/eosio.token.cpp --abigen
cleos set contract eosio.token . --abi eosio.token.abi -p eosio.token@active
cleos push action eosio.token create '[ "alice", "1000000000.0000 SYS"]' -p eosio.token@active
cleos create account eosio alice EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos push action eosio.token issue '[ "alice", "1000.0000 SYS", "memo" ]' -p alice@active
cleos push action eosio.token transfer '[ "alice", "daniel", "250.0000 SYS", "m" ]' -p alice@active
cleos get currency balance eosio.token daniel SYS
cleos push action eosio.token transfer '[ "alice", "bob", "250.0000 SYS", "m" ]' -p alice@active
cleos get currency balance eosio.token bob SYS
cleos push action eosio.token transfer '[ "alice", "sarah", "250.0000 SYS", "m" ]' -p alice@active
cleos get currency balance eosio.token sarah SYS
cleos push action eosio.token transfer '[ "alice", "ticket1", "10.0000 SYS", "m" ]' -p alice@active
cleos get currency balance eosio.token ticket1 SYS
cleos push action eosio.token transfer '[ "alice", "ticket2", "10.0000 SYS", "m" ]' -p alice@active
cleos get currency balance eosio.token ticket2 SYS

