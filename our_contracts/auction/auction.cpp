#include <eosiolib/eosio.hpp>

using namespace eosio;

class auction : public eosio::contract
{

public:

  //FIXME: move it to another file
  //# of winners (as the total # of ticket is a finite number)
  uint64_t N = 3;

  //Constructor
  auction(name s, name code, datastream<const char*> ds): _bids(s,0), contract(s, code, ds) {}

  //Calculate winners
  [[eosio::action]]
  void calculate_winners( )
  {
    //Initialization
    winners_price = 0;
    last_winner_price = 0;
    winners = "VT";

    //Sort the list first
    auto m = bid_records.get_index();

    //FIXME: Now it just finds the N lowest bid price
    //Get the N best/highest bid prices
    uint64_t i = 0;
    for(auto &iterator : m)
    {
      if (i<N)
      {
        winners_price[i] = iterator.bid_price;
	last_winner_price = winners_price[i]
	winners[i] = iterator.student_name;
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
  void placebid(name user, long int student_id, uint64_t bid_price)
  {
    require_auth(user);
    
    //Get the last winner price
    uint64_t last_winner_price = 0;
    calculate_winners( last_winner_price );

    //If the present bid price is too low, return 
    if(last_winner_price >= bid)
    {
      print("Sorry, your bid cannot be accepted. You need to bid at the price of", ceil(1.1*lat_winner_price), " or higher. Thanks!");
      return;
    }

    //FIXME: The primary key is the bid_price, and here it uses student_name. Not sure whether it works or not
    auto iterator = bid_records.find(student_name)
    if ( iterator == bid_records.end() )
    {
      bid_records.emplace(user, [&]( auto& row ) {
	row.name = user;
	row.student_id = student_id;
	row.bid_price = bid_price;
      });
    }
    else {
      bid_records.modify(iterator, user, [&]( auto& row ){
	row.name = user;
        row.student_id = student_id;
	row.bid_price = bid_price;
      }
    }
  }

  //Print winners
  [[eosio::action]]
  void printwinners(name user)
  {
    require_auth(user);
    calculate_winners();
    for(auto i = 0; i != N; i++)
      print("The winning's name: ", winners[i], ", bid price: ", winners_price[i]);
  }
  
  //Print all bid prices
  [[eosio::action]]
  void printbids(name user)
  {
    require_auth(user);
    for(auto iterator = bid_records.begin(); iterator != bid_records.end(); ++iterator)
      print("The bidder's name: ", iterator.name, ", bid price: ", iterator.bid_price);
  }

private:

  uint64_t winners_price[N];
  uint64_t last_winner_price
  name winners[N];
  
  struct [[eosio::table]] person
  {
    name student_name;
    long int student_id;
    uint64_t bid_price;
    uint64_t primary_key() const{ return bid_price; }
  };

  typedef eosio::multi_index<
    "people"_n, person
    > bid_index;
  bid_index bid_records;

};


