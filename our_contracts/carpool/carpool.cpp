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

class [[eosio::contract("carpool")]] carpool: public eosio::contract {

    public:
        carpool(name receiver, name code, datastream<const char*> ds):
        contract(receiver, code, ds){}
        
        //Methods Declaration

        //Apply to use the system
        ACTION createacc(){

        }

        //Drivers Review.

        ACTION driverreview(){

        }

        //Passengers Review
        ACTION preview(){
            
        }

        ACTION login(name username){
            require_auth(username);
            users_table _users(get_self(), get_first_receiver().value);
            // Create a record in the table if the player doesn't exist in our app yet
            auto user_iterator = _users.find(username.value);
            if (user_iterator == _users.end()) {
                user_iterator = _users.emplace(username,  [&](auto& new_user) {
                new_user.username = username;
                });
            } 
        }

        //Add a Ride
        ACTION addpost(name user_name, carpoolid, string loc_desc, string loc_param, uint64_t car_size, string cost_trip){
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
    
    //Database Declaration
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

        TABLE user_info {
        name            username;
        uint16_t        win_count = 0;
        uint16_t        lost_count = 0;

        auto primary_key() const { return username.value; }
        };
        typedef eosio::multi_index<name("users"), user_info> users_table;
     
        TABLE account {
          asset balance;

          uint64_t primary_key()const { return balance.symbol.code().raw(); }
        };
        typedef eosio::multi_index<"accounts"_n,account> accounts;

};
