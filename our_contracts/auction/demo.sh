#!/bin/bash
echo "Begin auction ticket1"

cleos push action auction placebid '["zejia", "10 VTOKEN"]' -p zejia@active
cleos push action auction placebid '["aanuo",  "12 VTOKEN"]' -p aanuo@active
cleos push action auction placebid '["weicheng", "14 VTOKEN"]' -p weicheng@active

# cleos push action auction printbids '["weicheng"]' -p weicheng@active
cleos push action auction printwinners '["ticket1"]' -p auction@active

cleos get table auction auction bidorders
cleos get account ticket1
cleos get account weicheng
cleos get currency balance eosio.token weicheng VTOKEN

cleos push action auction clearticket '["ticket1","EOS7nXVLLrEda6Xr8FG3hxyc47LsC19Us9avhUK3uEPiPghdJPG2c","inseason"]' -p vtfootball


echo "After clear auction ticket1"
cleos get currency balance eosio.token weicheng VTOKEN
cleos get table auction auction bidorders
cleos get account ticket1





sleep 30




echo "Begin auction ticket2"

cleos push action auction placebid '["alice", "15 VTOKEN"]' -p alice@active
cleos push action auction placebid '["bob", "19 VTOKEN"]' -p bob@active
cleos push action auction placebid '["zejia",  "50 VTOKEN"]' -p zejia@active

# cleos push action auction printbids '["weicheng"]' -p weicheng@active
cleos push action auction printwinners '["ticket2"]' -p auction@active

cleos get table auction auction bidorders
cleos get account ticket1
cleos get account zejia
cleos get currency balance eosio.token zejia VTOKEN

cleos push action auction clearticket '["ticket2","EOS8548cE8JioJT9ZRRdPqQ3KR6QmN6fH5ssE2ULKiBfbrmz7uYnX","inseason"]' -p vtfootball


echo "After clear auction ticket2"
cleos get currency balance eosio.token zejia VTOKEN
cleos get table auction auction bidorders
cleos get account ticket2





sleep 30





echo "Begin trading tickets"
echo '==================================================================='
echo '===                    T R A D E  T I C K E T S              ==='
echo '==================================================================='


echo '-------------------------------------------------------------------'

echo "seller needs to add tradeticket@eosio.code permission to their "
echo "active authority.  This lets the smart contract act on behalf of"
echo " the ticket1 account.  This smart contract needs this "
echo "permission because will switch the keys of the ticket1 to "
echo "the keys of the buyer."

# TODO: Do this with the VTOKENtem contract?
echo "add tradeticket@eosio.code permission to ticket1@active"
 cleos set account permission ticket1 active \
 '{"threshold": 1,"keys": [{"key": "EOS7nXVLLrEda6Xr8FG3hxyc47LsC19Us9avhUK3uEPiPghdJPG2c","weight": 1}],"accounts": [{"permission":{"actor":"tradeticket","permission":"eosio.code"},"weight":1}]}' \
owner -p ticket1


cleos set account permission ticket2 active \
'{"threshold": 1,"keys": [{"key": "EOS8548cE8JioJT9ZRRdPqQ3KR6QmN6fH5ssE2ULKiBfbrmz7uYnX","weight": 1}],"accounts": [{"permission":{"actor":"tradeticket","permission":"eosio.code"},"weight":1}]}' \
owner -p ticket2


echo 'weicheng will sell their ticket called ticket1 for 40 VTOKEN'
echo 'zejia will sell their ticket called ticket2 for 80 VTOKEN'


echo 'cleos get table eosio.token tradeticket accounts'
cleos get table eosio.token tradeticket accounts
echo 'cleos get table eosio.token weicheng accounts'
cleos get table eosio.token weicheng accounts

echo 'cleos get table eosio.token zejia accounts'
cleos get table eosio.token zejia accounts


echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders

echo 'cleos push action tradeticket sellticket ["weicheng","ticket1","40 VTOKEN"] -p ticket1'
cleos push action tradeticket sellticket '["weicheng","ticket1","40 VTOKEN","inseason"]' -p ticket1


echo 'cleos push action tradeticket sellticket ["zejia","ticket2","80 VTOKEN"] -p ticket2'
cleos push action tradeticket sellticket '["zejia","ticket2","80 VTOKEN","inseason"]' -p ticket2

echo 'Both zejia and weicheng submit selling order willing to sell tickets '
echo 'cleos get table eosio.token tradeticket accounts'
cleos get table eosio.token tradeticket accounts

echo 'cleos get table eosio.token weicheng accounts'
cleos get table eosio.token weicheng accounts

echo 'cleos get table eosio.token zejia accounts'
cleos get table eosio.token zejia accounts

echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders




sleep 30



echo 'aanu see the ticket1 is for sale and want to buy ticket1 from weicheng'
echo 'aanuo will buy the account from the tradeticket contract'

echo "=== add tradeticket@eosio.code permission to aanuo@active ==="
cleos set account permission aanuo active \
'{"threshold": 1,"keys": [{"key": "EOS5wNaiDJqWBFGYS4RB5nks7ZYf72przz2QPcHz46kJrEvccfGZZ","weight": 1}],"accounts": [{"permission":{"actor":"tradeticket","permission":"eosio.code"},"weight":1}]}' \
owner -p aanuo

echo 'cleos get account aanuo'
cleos get account aanuo

echo 'cleos push action tradeticket buyticket ["aanuo","ticket1","EOS5wNaiDJqWBFGYS4RB5nks7ZYf72przz2QPcHz46kJrEvccfGZZ"] -p aanuo'
cleos push action tradeticket buyticket '["aanuo","ticket1","EOS5wNaiDJqWBFGYS4RB5nks7ZYf72przz2QPcHz46kJrEvccfGZZ","inseason"]' -p aanuo
sleep 1
echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders
cleos get table tradeticket tradeticket buyorders
echo 'cleos get account ticket1'
cleos get account ticket1
echo 'cleos get account ticket1'
cleos get account ticket2
cleos get currency balance eosio.token weicheng VTOKEN
cleos get currency balance eosio.token zejia VTOKEN
cleos get currency balance eosio.token aanuo VTOKEN






sleep 30




echo 'aanu see the ticket2 is for sale and want to buy ticket2 from zejia'

echo 'cleos push action tradeticket buyticket ["aanuo","ticket2","EOS5wNaiDJqWBFGYS4RB5nks7ZYf72przz2QPcHz46kJrEvccfGZZ"] -p aanuo'
cleos push action tradeticket buyticket '["aanuo","ticket2","EOS5wNaiDJqWBFGYS4RB5nks7ZYf72przz2QPcHz46kJrEvccfGZZ","inseason"]' -p aanuo
sleep 1
echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders
cleos get table tradeticket tradeticket buyorders

echo 'cleos get account ticket1'
cleos get account ticket1
echo 'cleos get account ticket2'
cleos get account ticket2
cleos get currency balance eosio.token weicheng VTOKEN
cleos get currency balance eosio.token zejia VTOKEN
cleos get currency balance eosio.token aanuo VTOKEN
echo 'Get VT account balance to see whether it gets the revenue'
cleos get currency balance eosio.token vtfootball VTOKEN
