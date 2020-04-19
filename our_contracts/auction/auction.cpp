#include <math.h>

#include <eosio/eosio.hpp>
#include <eosio/print.hpp>

#include <auction.h>

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

    //Sort the list in descending order
    auto pridx = bid_records.get_index<"byauxi"_n>();

    //Get the N best/highest bid prices
    uint64_t i = 0;
    for(auto& iterator : pridx)
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
    
    //Initialize the last winner price
    last_winner_price = 0;
    //Calculate the N top bid prices and bidders at present
    calwinners();

    //If the present bid price is too low, return 
    if(last_winner_price >= bid_price)
    {
      print("Sorry, your bid cannot be accepted. You need to bid at the price of ", ceil(1.1*last_winner_price), " or higher. Thanks!");
      return;
    }

    auto iterator = bid_records.find(user.value);
    if ( iterator == bid_records.end() )
    {
      bid_records.emplace(user, [&]( auto& row ) {
	row.user_name = user;
	row.user_id = user_id;
	row.bid_price = bid_price;
        //limit the bid price to be less than 10000
	row.auxi_price = 10000-bid_price;
      });
    }
    else
    {
      bid_records.modify(iterator, user, [&]( auto& row ) {
	row.user_name = user;
        row.user_id = user_id;
	row.bid_price = bid_price;
        //limit the bid price to be less than 10000   
	row.auxi_price = 10000-bid_price;
      });
    }
  }

  //Print winners
  [[eosio::action]]
  void printwinners(name user)
  {
    require_auth(user);
    calwinners();
    for(auto i = 0; i != N; i++){
      print_f("{winner %}:{name: %, bid_price: %} | ", i+1, winners[i], winners_price[i]);
    }
  }
  
  //Print all bid prices
  [[eosio::action]]
  void printbids(name user)
  {
    require_auth(user);

    //Sort the list in descending order
    auto pridx = bid_records.get_index<"byauxi"_n>();

    auto indx = 0;
    for(auto& iterator : pridx)
      print_f("{bidder %}:{name: %, bid_price: %} | ", indx++, iterator.user_name, iterator.bid_price);
  }

private:

  //Initialization of winner info
  int64_t winners_price[N] = {0};
  int64_t last_winner_price = {0};
  name winners[N];
  //name winners[N] = {name("VT")};
  
  struct [[eosio::table]] person
  {
    name user_name;
    int64_t user_id;
    uint64_t bid_price;
    //Use auxi_price to sort the table in desceding order
    uint64_t auxi_price;
    int64_t primary_key() const{ return user_name.value; };
    uint64_t get_price() const{ return bid_price; };
    uint64_t get_auxi() const{ return auxi_price; };
  };

  typedef eosio::multi_index<
    "people"_n, person, 
    indexed_by<"byprice"_n, const_mem_fun<person, uint64_t, &person::get_price>>,
    indexed_by<"byauxi"_n, const_mem_fun<person, uint64_t, &person::get_auxi>>
    > bid_index;
  bid_index bid_records;
};
