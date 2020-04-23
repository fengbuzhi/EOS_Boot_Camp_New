# echo '=== hello world ==='
# cleos push action tradeticket hello '[""]' -p tradeticket

echo '==================================================================='
echo '===                    T R A D E  T I C K E T S              ==='
echo '==================================================================='

sleep 1

echo '1. '
echo 'Account tradeticket runs the contract which lets users buy and sell accounts'
echo 'cleos get account tradeticket'
cleos get account tradeticket

echo 'Account daniel is selling is second account ticket1'
echo 'cleos get account daniel'
cleos get account daniel

cleos get account bob

echo 'cleos get account ticket1'
cleos get account ticket1

echo 'cleos get account ticket2'
cleos get account ticket2

echo 'sarah is going to buy ticket1 from daniel'
echo 'cleos get account sarah'
cleos get account sarah


echo '-------------------------------------------------------------------'

echo "seller needs to add tradeticket@eosio.code permission to their "
echo "active authority.  This lets the smart contract act on behalf of"
echo " the ticket1 account.  This smart contract needs this "
echo "permission because will switch the keys of the ticket1 to "
echo "the keys of the buyer."

# TODO: Do this with the system contract?
echo "add tradeticket@eosio.code permission to ticket1@active"
cleos set account permission ticket1 active \
'{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"tradeticket","permission":"eosio.code"},"weight":1}]}' \
owner -p ticket1


cleos set account permission ticket2 active \
'{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"tradeticket","permission":"eosio.code"},"weight":1}]}' \
owner -p ticket2


echo '2. '
echo 'daniel will sell their ticket called ticket1 for 6.0000 SYS'
echo 'bob will sell their ticket called ticket2 for 6.0000 SYS'
echo 'before: '
echo 'cleos get table eosio.token tradeticket accounts'
cleos get table eosio.token tradeticket accounts
echo 'cleos get table eosio.token daniel accounts'
cleos get table eosio.token daniel accounts

echo 'cleos get table eosio.token bob accounts'
cleos get table eosio.token bob accounts


echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders

echo 'cleos push action tradeticket sellticket ["daniel","ticket1","6.0000 SYS"] -p ticket1'
cleos push action tradeticket sellticket '["daniel","ticket1","6.0000 SYS","inseason"]' -p ticket1


echo 'cleos push action tradeticket sellticket ["bob","ticket2","12.0000 SYS"] -p ticket2'
cleos push action tradeticket sellticket '["bob","ticket2","12.0000 SYS","inseason"]' -p ticket2


sleep 1

echo 'after: '
echo 'cleos get table eosio.token tradeticket accounts'
cleos get table eosio.token tradeticket accounts

echo 'cleos get table eosio.token daniel accounts'
cleos get table eosio.token daniel accounts

echo 'cleos get table eosio.token bob accounts'
cleos get table eosio.token bob accounts

echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders

echo 'NOTE: no SYS has been transferred yet!'
