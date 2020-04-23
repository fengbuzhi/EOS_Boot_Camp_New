#include <math.h>

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
//#include <eosio.token/eosio.token.hpp>
#include <eosio/print.hpp>

#include <auction.h>

#define VToken symbol("TNT", 0)

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

  //Methods Declaration

        //Apply to use the system
        ACTION createaccount(){

        }

        //Drivers Review.

        ACTION driverreview(){

        }

        //Passengers Review
        ACTION preview(){
            
        }

        //Add a Ride
        ACTION addpost(name user_name, string loc_desc, string loc_param, uint64_t car_size, string cost_trip){
            require_auth(user_name);
            carpool_index _cpool_index(get_self(), get_first_receiver().value);

            //You cannot do any check here
            _cpool_index.emplace(user_name, [&](auto& row){
                row.username = user_name;
                row.log_id = _cpool_index.available_primary_key();
                row.loc_desc = loc_desc;
                row.loc_param = loc_param;
                row.cost_jorn = cost_trip;
                row.car_space = car_size;
                row.delete_post = 0;
                
            });
            print("Trip Successfully Posted");
        }

        ACTION editpost(name username, uint64_t carpoolid, string loc_desc){
            //Edit Post Method
            require_auth(username);
            carpool_index _cpool_index(get_self(), get_first_receiver().value);
            auto itr = _cpool_index.find(carpoolid);

            if(itr->username == username){
                //It is my record. I can edit it
                _cpool_index.modify(itr, username, [&](auto& row){
                    row.loc_desc = loc_desc;
                });
                print("Your post has been edited");
            }
            else{
                //This is not my record, deny access
                print("You cannot edit this record. It does not belong to you");
            }

        }

        ACTION deletepost(name username, uint64_t carpoolid){
            //Delete post Method. Set delete_post = 1
            require_auth(username);
            carpool_index _cpool_index(get_self(), get_first_receiver().value);
            auto itr = _cpool_index.find(carpoolid);

            if(itr != _cpool_index.end()){
                _cpool_index.modify(itr, username, [&](auto& row){
                    row.delete_post = 1;
                });
            }
            else{
                print("Record does not exist");
            }

        }

        ACTION hopride(name username, uint64_t carpoolid, uint64_t space_req){
            //Join existing listing
            require_auth(username);
            joinride_index _joinride_index(get_self(), get_first_receiver().value);
            auto itr = _joinride_index.find(carpoolid);

            //Check if the post exist in the db

            if(itr != _joinride_index.end()){
                //Check if the record is tied to the same user
                if(itr -> username != username){
                    //Different user posting. Insert
                     _joinride_index.emplace(username, [&](auto& row){
                        row.username = username;
                        row.jridx = _joinride_index.available_primary_key();
                        row.carpoolid = carpoolid;
                        row.size_req = space_req;
                    });
                print("You have signup for this ride");
                }
                else{
                    //Same user posting. Tell person to use the edit or cancel button
                    print("You have already signed up for this post! Do you want to edit?");
                }
            }
            else{
                //It means nobody has signed up for the trip. Insert the record fast
                _joinride_index.emplace(username, [&](auto& row){
                    row.username = username;
                    row.jridx = _joinride_index.available_primary_key();
                    row.carpoolid = carpoolid;
                    row.size_req = space_req;
                });
                print("You have signup for this ride");
            }

        }

        ACTION cancelride(){
            //Cancel ride 
        }

        ACTION edithopride(){
            //Edit specifics of rides you agree to share
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
    asset funds;
    asset bid_price;
    //Use auxi_price to sort the table in desceding order
    uint64_t auxi_price;
    int64_t primary_key() const{ return user_name.value; };
    uint64_t get_price() const{ return bid_price.symbol.raw(); };
    uint64_t get_auxi() const{ return auxi_price; };
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
  bid_index bid_records;

        //Aanu Database Declaration
    private:

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
};

