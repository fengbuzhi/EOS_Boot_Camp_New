#include <math.h>

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
//#include <eosio.token/eosio.token.hpp>
#include <eosio/print.hpp>
#include <eosio/time.hpp>
#include <eosio/public_key.hpp>

#include <abieos_numeric.hpp>

#include <auction.h>

#define VToken symbol("VTOKEN", 0)

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
      "eosio.token"_n,
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

    auto iterator = bid_records.find(user.value);
    if ( iterator == bid_records.end() )
    {
      bid_records.emplace(user, [&]( auto& row ) {
	row.user_name = user;
	row.user_id = user_id;
	row.bid_price.amount = bid_price.amount;
        //limit the bid price to be less than 10000
	row.auxi_price = 10000-bid_price.amount;
      });
    }
    else
    {
      bid_records.modify(iterator, user, [&]( auto& row ) {
	row.user_name = user;
        row.user_id = user_id;
	row.bid_price.amount = bid_price.amount;
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

    //Sort the list in descending order
    auto pridx = bid_records.get_index<"byauxi"_n>();

    auto indx = 1;
    for(auto& iterator : pridx)
      print_f("{bidder %}:{name: %, bid_price: %} | ", indx++, iterator.user_name, iterator.bid_price.amount);
  }

  [[eosio::action]]
  void distribute(name issuer)
  {
    check( issuer == "vtsport"_n, "Only Virginia Tech can distribute tickets!" );
    //action(
    //  permission_level{get_self(), "active"_n};
    //  get_self(),
    //  "giveauth"_n,
    //  std::make_tuple( user, new_public_key )
    //).send();

    //sort the list in descending order
    auto pridx = bid_records.get_index<"byauxi"_n>();

    auto indx = 1;
    auto ticket_to_send = "ticket";
    for(auto& iterator : pridx){
      tiket_to_send = strcat("ticket", (char)indx);
      update_holder( iterator.user_name, eosio::string_to_name(ticket_to_send), winners_price[indx-1] );
      indx++;
      if(indx > N)
	break;
    }
  }

  [[eosio::action]]
  void giveauth(const name user, const std::string new_owner_pubkey)
  {
    print("*** giveauth ***\n");
    std::array<char, 33> owner_pubkey_char;

    const std::string owner_key_str = new_owner_pubkey.substr(0, 53);
    print("new key: ", owner_key_str, "\n");

    // create new public key
    const abieos::public_key owner_pubkey =
          abioes::string_to_public_key( owner_key_str );
 
    std::copy( owner_pubkey.data.begin(),
               owner_pubkey.data.end(),
	       owner_pubkey_char.begin());

    //Create new authority
    const auto owner_auth = authority{ 1,
      {
        {/*key_weight*/
	  {(unit8_t)abieos::key_type::k1, owner_pubkey_char}, 1
	}
      },
      {/*tickets*/},
      {/*waits*/}
    };

    //TODO: Change this from changing the "active" permission, to
    //changing the "owner" permission
    action(
      permission_level{ user, "active"_n },
      "eosio"_n,
      "updateauth"_n,
      std::make_tuple( user, "active"_n, "owner"_n, owner_auth )
    ).send();

  }

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

  struct key_weight {
    eosio::public_key key;
    uint16_t          weight;
  }

  /***Tables***/

  struct authority {
    uint32_t                             threshold = 0;
    std::vector<key_weight>              keys;
    std::vector<permission_level_weight> tickets;
    std::vector<wait_weight>             waits;
  };

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

    int64_t primary_key() const{ return holder.value };
    uint64_t get_ticket() const{ return ticket.value };
  }

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
  bid_index bid_records;

  typedef eosio::multi_index<
    "tickets"_n, ticket,
    indexed_by<"byticket"_n, const_mem_fun<person, uint64_t, &ticket::get_ticket>>
  > holder_indx;
  holder_indx holder_records;

  /***functions***/

  void update_holder( name user, name ticket_to_send, uint64_t price ){
    //Each person can only buy one ticket
    auto iterator = holder_records.find(user.value);
    if ( iterator == holder_records.end() )
    {
      holder_records.emplace(user, [&]( auto& row ) {
	row.holder = user;
	row.ticket = ticket_to_send;
	row.ticket_price = price;
      });
    }
    else
    {
      bid_records.modify(iterator, user, [&]( auto& row ) {
	row.holder = user;
	row.ticket = ticket_to_send;
	row.ticket_price = price;
      });
    }
  }

};

