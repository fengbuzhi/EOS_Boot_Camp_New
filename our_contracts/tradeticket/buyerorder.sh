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
cleos get currency balance eosio.token weicheng SYS
cleos get currency balance eosio.token zejia SYS
cleos get currency balance eosio.token aanuo SYS


echo 'cleos push action tradeticket buyticket ["aanuo","ticket2","EOS5wNaiDJqWBFGYS4RB5nks7ZYf72przz2QPcHz46kJrEvccfGZZ"] -p aanuo'
cleos push action tradeticket buyticket '["aanuo","ticket2","EOS5wNaiDJqWBFGYS4RB5nks7ZYf72przz2QPcHz46kJrEvccfGZZ","inseason"]' -p aanuo
sleep 1
echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders
cleos get table tradeticket tradeticket buyorders

echo 'cleos get account ticket1'
cleos get account ticket1
echo 'cleos get account ticket1'
cleos get account ticket2
cleos get currency balance eosio.token weicheng SYS
cleos get currency balance eosio.token zejia SYS
cleos get currency balance eosio.token aanuo SYS