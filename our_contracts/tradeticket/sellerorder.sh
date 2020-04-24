echo '==================================================================='
echo '===                    T R A D E  T I C K E T S              ==='
echo '==================================================================='

sleep 1

echo 'Account tradeticket runs the contract which lets users buy and sell tickets'
echo 'cleos get account tradeticket'
cleos get account tradeticket

echo 'cleos get account weicheng'
cleos get account weicheng

echo 'cleos get account zejia'
cleos get account zejia

echo 'cleos get account aanuo'
cleos get account aanuo

echo 'cleos get account ticket1'
cleos get account ticket1

echo 'cleos get account ticket2'
cleos get account ticket2

echo 'aanuo is going to buy ticket1 from weicheng'


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


echo 'weicheng will sell their ticket called ticket1 for 6 VTOKEN'
echo 'zejia will sell their ticket called ticket2 for 6 VTOKEN'

echo 'before: '

echo 'cleos get table eosio.token tradeticket accounts'
cleos get table eosio.token tradeticket accounts
echo 'cleos get table eosio.token weicheng accounts'
cleos get table eosio.token weicheng accounts

echo 'cleos get table eosio.token zejia accounts'
cleos get table eosio.token zejia accounts


echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders

echo 'cleos push action tradeticket sellticket ["weicheng","ticket1","6 VTOKEN"] -p ticket1'
cleos push action tradeticket sellticket '["weicheng","ticket1","6 VTOKEN","inseason"]' -p ticket1


echo 'cleos push action tradeticket sellticket ["zejia","ticket2","12 VTOKEN"] -p ticket2'
cleos push action tradeticket sellticket '["zejia","ticket2","12 VTOKEN","inseason"]' -p ticket2


sleep 1

echo 'after: '
echo 'cleos get table eosio.token tradeticket accounts'
cleos get table eosio.token tradeticket accounts

echo 'cleos get table eosio.token weicheng accounts'
cleos get table eosio.token weicheng accounts

echo 'cleos get table eosio.token zejia accounts'
cleos get table eosio.token zejia accounts

echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders

echo 'NOTE: no VTOKEN has been transferred yet!'
