eosio-cpp -I /workspace/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token/include -o /workspace/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token/eosio.token.wasm /workspace/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token/src/eosio.token.cpp --abigen

cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio vtfootball EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
cleos create account eosio weicheng EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio aanuo EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio zejia EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio alice EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio bob EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos create account eosio ticket1 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio ticket2 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV 
cleos create account eosio ticket3 EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV

cleos set contract eosio.token /workspace/EOS_Boot_Camp_New/existing_contracts/eosio.contracts/contracts/eosio.token/ --abi eosio.token.abi -p eosio.token@active

cleos push action eosio.token create '[ "vtfootball", "100000 VTOKEN"]' -p eosio.token@active

cleos push action eosio.token issue '[ "vtfootball", "10000 VTOKEN", "Initial Token Offering" ]' -p vtfootball@active

cleos push action eosio.token transfer '[ "vtfootball", "weicheng", "200 VTOKEN", "Send 200 VTOKEN to Weicheng" ]' -p vtfootball@active
cleos push action eosio.token transfer '[ "vtfootball", "aanuo", "200 VTOKEN", "Send 200 VTOKEN to Aanuo" ]' -p vtfootball@active
cleos push action eosio.token transfer '[ "vtfootball", "zejia", "200 VTOKEN", "Send 200 VTOKEN to Zejia" ]' -p vtfootball@active
cleos push action eosio.token transfer '[ "vtfootball", "alice", "200 VTOKEN", "Send 200 VTOKEN to Alice" ]' -p vtfootball@active
cleos push action eosio.token transfer '[ "vtfootball", "bob", "200 VTOKEN", "Send 200 VTOKEN to Bob" ]' -p vtfootball@active

cleos push action eosio.token transfer '[ "vtfootball", "ticket1", "2000 VTOKEN", "Send 2000 VTOKEN to ticket1" ]' -p vtfootball@active
cleos push action eosio.token transfer '[ "vtfootball", "ticket2", "2000 VTOKEN", "Send 2000 VTOKEN to ticket2" ]' -p vtfootball@active
cleos push action eosio.token transfer '[ "vtfootball", "ticket3", "2000 VTOKEN", "Send 2000 VTOKEN to ticket3" ]' -p vtfootball@active

cleos get currency balance eosio.token weicheng VTOKEN
cleos get currency balance eosio.token aanuo VTOKEN
cleos get currency balance eosio.token zejia VTOKEN
cleos get currency balance eosio.token alice VTOKEN
cleos get currency balance eosio.token bob VTOKEN
