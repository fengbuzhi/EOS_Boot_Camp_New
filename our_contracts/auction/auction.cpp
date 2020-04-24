#include <math.h>

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
//#include <eosio.token/eosio.token.hpp>
#include <eosio/print.hpp>
#include <eosio/time.hpp>
//#include <eosiolib/public_key.hpp>

//#include "abieos_numeric.hpp"

#define VToken symbol("VTOKEN", 0)
#define N 3  //# of winners (equal to the number of vailable football tickets for bidding. It can be changed as needed)

using namespace eosio;
using namespace std;

class [[eosio::contract("auction")]] auction : public eosio::contract
{

private:

  //Initialization of winner info
  int64_t winners_price[N] = {0};
  int64_t last_winner_price = {0};
  name winners[N];
  //name winners[N] = {name("VT")};
  
  /***Structured data types***/

  struct permission_level_weight {
    eosio::permission_level permission;
    uint16_t                weight;
  };

  struct wait_weight {
    uint32_t wait_sec;
    uint16_t weight;
  };


  /***Tables***/
  struct [[eosio::table]] person
  {
    name user_name;
    int64_t user_id;
    asset funds;
    asset bid_price;
    //Use auxi_price to sort the table in desceding order
    uint64_t auxi_price;
    int64_t primary_key() const{ return user_name.value; };
    uint64_t get_price() const{ return bid_price.symbol.raw(); };
    uint64_t get_auxi() const{ return auxi_price; };
  };

  struct [[eosio::table]] ticket
  {
    name holder;
    name ticket;
    uint64_t ticket_price;

    int64_t primary_key() const{ return holder.value; };
    int64_t get_ticket() const{ return ticket.value; };
  };

  struct [[eosio::table]] account {
    asset balance;

    uint64_t primary_key()const { return balance.symbol.code().raw(); }
  };

  typedef eosio::multi_index<"accounts"_n,account> accounts;

  typedef eosio::multi_index<
    "people"_n, person, 
    indexed_by<"byprice"_n, const_mem_fun<person, uint64_t, &person::get_price>>,
    indexed_by<"byauxi"_n, const_mem_fun<person, uint64_t, &person::get_auxi>>
    > bid_index;

  typedef eosio::multi_index<
    "tickets"_n, ticket
    //indexed_by<"byticket"_n, const_mem_fun<person, int64_t, &ticket::get_ticket>>
    > holder_indx;

  //Aanu Database Declaration

  TABLE carpoolog{

      name username;
      uint64_t log_id;
      uint64_t car_space;
      string loc_desc;
      string loc_param;
      string cost_jorn;
      uint64_t delete_post;

      auto primary_key() const {return log_id;}
      auto by_username() const {return username.value;}
  };
  typedef multi_index <name("carpoolt"), carpoolog> carpool_index;

  TABLE joinride{

      name username;
      uint64_t jridx;
      uint64_t carpoolid;
      uint64_t size_req;

      auto primary_key() const {return jridx;}
      auto by_username() const {return username.value;}
  };
  typedef multi_index <name("joinrt"),joinride> joinride_index;

  /***functions***/

  void updateholder( name user, name ticket_to_buy, uint64_t ticket_price ){
    //Each person can only buy one ticket
    holder_indx holder_records(get_self(), get_first_receiver().value);
    auto iterator = holder_records.find(user.value);
    if ( iterator == holder_records.end() )
    {
      holder_records.emplace(user, [&]( auto& row ) {
	    row.holder = user;
	    row.ticket = ticket_to_buy;
	    row.ticket_price = ticket_price;
      });
    }
    else {
      holder_records.modify(iterator, user, [&]( auto& row ) {
	    row.holder = user;
        row.ticket = ticket_to_buy;
        row.ticket_price = ticket_price;
      });      
    }
  }

public:

  //Constructor
  auction(name s, name code, datastream<const char*> ds): contract(s, code, ds) {}

  //Calculate winners


    ACTION eraseme(){
    //require_auth(get_self());
  bid_index _users(get_self(), get_first_receiver().value);
  auto msg_itr = _users.begin();

        while (msg_itr != _users.end()) {
            msg_itr = _users.erase(msg_itr);
        }
}

  [[eosio::action]]
  void calwinners( )
  {

    bid_index _bid_records( get_self(), get_first_receiver().value) ;

    //Sort the list in descending order
    auto pridx = _bid_records.get_index<"byauxi"_n>();

    //Get the N best/highest bid prices
    uint64_t i = 0;
    for(auto& iterator : pridx)
    {
      if ( i<N )
      {
        winners_price[i] = iterator.bid_price.amount;
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
  void placebid(name user, int64_t user_id, asset bid_price)
  {
    require_auth(user);

    auto sym = bid_price.symbol;

    check( bid_price.symbol == VToken, "Symbol precision mismatch." );

    action(
      permission_level{get_self(),"active"_n},
      "eosio.token"_n, // For online demo/UI, change this to the contract where the eosio.token is set
      "checkbid"_n,
      std::make_tuple( user, bid_price )
    ).send();		    
    
    //Initialize the last winner price
    last_winner_price = 0;
    //Calculate the N top bid prices and bidders at present
    calwinners();

    //If the present bid price is too low, return 
    if( last_winner_price >= bid_price.amount )
    {
      //Need to bid 2 more dollars than the last_winner_price
      print("Sorry, your bid cannot be accepted. You need to bid at the price of ", last_winner_price + 2, " or higher. Thanks!");
      return;
    }

    bid_index _bid_records( get_self(), get_first_receiver().value) ;

    auto iterator = _bid_records.find(user.value);
    if ( iterator == _bid_records.end() )
    {
      _bid_records.emplace(user, [&]( auto& row ) {
	row.user_name = user;
	row.user_id = user_id;
	row.bid_price = bid_price;
        //limit the bid price to be less than 10000
	row.auxi_price = 10000-bid_price.amount;
      });
    }
    else
    {
      _bid_records.modify(iterator, user, [&]( auto& row ) {
	row.user_name = user;
        row.user_id = user_id;
	row.bid_price = bid_price;
        //limit the bid price to be less than 10000   
	row.auxi_price = 10000-bid_price.amount;
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

    bid_index _bid_records( get_self(), get_first_receiver().value) ;

    //Sort the list in descending order
    auto pridx = _bid_records.get_index<"byauxi"_n>();

    auto indx = 1;
    for(auto& iterator : pridx)
      print_f("{bidder %}:{name: %, bid_price: %} | ", indx++, iterator.user_name, iterator.bid_price.amount);
  }

  [[eosio::action]]
  void buyticket(name user, name ticketinfo, asset bid_price)
  {

    //check( issuer == "vtsport"_n, "Only Virginia Tech can distribute tickets!" );
    //action(
    //  permission_level{get_self(), "active"_n};
    //  get_self(),
    //  "giveauth"_n,
    //  std::make_tuple( user, new_public_key )
    //).send();

    require_auth(user);

    bid_index _bid_records( get_self(), get_first_receiver().value );

    auto iterator = _bid_records.find(user.value);

    //Not sure why this is needed, but an test shows that we need it
    calwinners();

    //if ( iterator == _bid_records.end() ){
      updateholder( user, ticketinfo, iterator->bid_price.amount );
    //}
    
    //If using iterator->bid_price here, then not working due to a key right issue. Not sure of why
    action(
      permission_level{user, "active"_n},
      "eosio.token"_n, // For online demo/UI, change this to the contract where the eosio.token is set
      "transfer"_n,
      std::make_tuple( user,
                       "vt"_n,
                       bid_price,
                       std::string("Initial bidding payment") )
    ).send();
  }

};
