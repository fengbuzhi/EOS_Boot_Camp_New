#include <math.h>
#include <eosiolib/eosio.hpp>
//FIXME: move it to another file
//# of winners (equal to the number of football tickets)
#define N 3


using namespace eosio;

class auction : public eosio::contract
{

public:

  //Constructor
  auction(name s, name code, datastream<const char*> ds): contract(s, code, ds), bid_records(s, 0) {}

  //Calculate winners
  [[eosio::action]]
  void calwinners( )
  {

    //FIXME: Comment it since not sure of whether it works
    //Sort the list first
    //auto m = bid_records.get_index();

    //FIXME: Now it just finds the N lowest bid price
    //Get the N best/highest bid prices
    uint64_t i = 0;
    for(auto& iterator : bid_records)
    {
      if (i<N)
      {
        winners_price[i] = iterator.bid_price;
	//Update last winner price every time
	last_winner_price = winners_price[i];
	winners[i] = iterator.user_name;
	i++;
      }
      else
      {
	break;
      }
    }
  }

  //Places a bid
  [[eosio::action]]
  void placebid(name user, int64_t user_id, uint64_t bid_price)
  {
    require_auth(user);
    
    //Get the last winner price
    uint64_t last_winner_price = 0;
    calwinners();

    //If the present bid price is too low, return 
    if(last_winner_price >= bid_price)
    {
      print("Sorry, your bid cannot be accepted. You need to bid at the price of ", ceil(1.1*last_winner_price), " or higher. Thanks!");
      return;
    }

    //FIXME: Now it should use the user_id to search, but here it uses bid_price, which is the primary key.
    auto iterator = bid_records.find(bid_price);
    if ( iterator == bid_records.end() )
    {
      bid_records.emplace(user, [&]( auto& row ) {
	row.user_name = user;
	row.user_id = user_id;
	row.bid_price = bid_price;
      });
    }
    else
    {
      bid_records.modify(iterator, user, [&]( auto& row ) {
	row.user_name = user;
        row.user_id = user_id;
	row.bid_price = bid_price;
      });
    }
  }

  //Print winners
  [[eosio::action]]
  void printwinners(name user)
  {
    require_auth(user);
    calwinners();
    for(auto i = 0; i != N; i++)
      print("The winner's name: ", winners[i], ", bid price: ", winners_price[i]);
  }
  
  //Print all bid prices
  [[eosio::action]]
  void printbids(name user)
  {
    require_auth(user);
    for(auto iterator = bid_records.begin(); iterator != bid_records.end(); ++iterator)
      print("The bidder's name: ", iterator->user_name, ", bid price: ", iterator->bid_price);
  }

private:

  uint64_t winners_price[N] = {0};
  uint64_t last_winner_price = {0};
  name winners[N];
  //name winners[N] = {"VT"};
  
  struct [[eosio::table]] person
  {
    name user_name;
    int64_t user_id;
    uint64_t bid_price;
    uint64_t primary_key() const{ return bid_price; }
  };

  typedef eosio::multi_index<
    "people"_n, person
    > bid_index;
  bid_index bid_records;

};


