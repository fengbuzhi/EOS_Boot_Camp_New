#!/bin/bash

cleos push action auction placebid '["alice", 122121121, "8 VTOKEN"]' -p alice@active
cleos push action auction placebid '["weicheng", 999997777, "10 VTOKEN"]' -p weicheng@active
cleos push action auction placebid '["aanuo", 111111111, "12 VTOKEN"]' -p aanuo@active
cleos push action auction placebid '["zejia", 222222222, "14 VTOKEN"]' -p zejia@active

cleos push action auction printbids '["weicheng"]' -p weicheng@active
cleos push action auction printwinners '["weicheng"]' -p weicheng@active

cleos push action auction buyticket '["weicheng", "ticket1", "10 VTOKEN"]' -p weicheng@active
cleos push action auction buyticket '["aanuo", "ticket2", "12 VTOKEN"]' -p aanuo@active
cleos push action auction buyticket '["zejia", "ticket3", "14 VTOKEN"]' -p zejia@active

echo "After buying a ticket"
cleos get currency balance eosio.token weicheng VTOKEN
cleos get currency balance eosio.token aanuo VTOKEN
cleos get currency balance eosio.token zejia VTOKEN

echo "Show the tickets table"
cleos get table auction auction tickets
