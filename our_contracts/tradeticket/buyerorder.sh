
echo '3. '
echo 'sarah will buy the account from the tradeticket contract'

echo "=== add tradeticket@eosio.code permission to sarah@active ==="
cleos set account permission sarah active \
'{"threshold": 1,"keys": [{"key": "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV","weight": 1}],"accounts": [{"permission":{"actor":"tradeticket","permission":"eosio.code"},"weight":1}]}' \
owner -p sarah

echo 'cleos get account sarah'
cleos get account sarah

echo 'cleos push action tradeticket buyaccount ["sarah","ticket1","EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"] -p sarah'
cleos push action tradeticket buyaccount '["sarah","ticket1","EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p sarah
sleep 1
echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders
cleos get table tradeticket tradeticket buyorders
echo 'cleos get account ticket1'
cleos get account ticket1
echo 'cleos get account ticket1'
cleos get account ticket2
cleos get currency balance eosio.token daniel SYS
cleos get currency balance eosio.token bob SYS
cleos get currency balance eosio.token sarah SYS


echo 'cleos push action tradeticket buyaccount ["sarah","ticket2","EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"] -p sarah'
cleos push action tradeticket buyaccount '["sarah","ticket2","EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV"]' -p sarah
sleep 1
echo 'cleos get table tradeticket tradeticket sellorders'
cleos get table tradeticket tradeticket sellorders
cleos get table tradeticket tradeticket buyorders

echo 'cleos get account ticket1'
cleos get account ticket1
echo 'cleos get account ticket1'
cleos get account ticket2
cleos get currency balance eosio.token daniel SYS
cleos get currency balance eosio.token bob SYS
cleos get currency balance eosio.token sarah SYS