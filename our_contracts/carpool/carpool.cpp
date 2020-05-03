#include <math.h>
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/print.hpp>
#include <eosio/time.hpp>
#include "abieos_numeric.hpp"

#define RAMCORE_SYMBOL symbol("RAMCORE", 4)
#define RAM_SYMBOL symbol("RAM", 0)

#define VToken symbol("VTOKEN", 0)

using namespace eosio;
using namespace std;



class [[eosio::contract("carpool")]] carpool: public eosio::contract {

    public:

    using contract::contract;

    carpool( name receiver, name code, datastream<const char*> ds ):
                  contract( receiver, code, ds ){}

    /****************************************************************************
     *                              A C T I O N S
     ***************************************************************************/
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
        ACTION addpost(name user_name, string loc_desc, string post_title,string loc_param, uint64_t car_size, asset cost_trip){
            require_auth(user_name);
            carpool_index _cpool_index(get_self(), get_first_receiver().value);

            //You cannot do any check here
            _cpool_index.emplace(user_name, [&](auto& row){
                row.username = user_name;
                row.log_id = _cpool_index.available_primary_key();
                row.loc_desc = loc_desc;
                row.loc_param = loc_param;
                row.post_title = post_title;
                row.cost_jorn = cost_trip;
                row.car_space = car_size;
                row.delete_post = 0;
                
            });

            print("Trip Successfully Posted");

        }

        ACTION editpost(name username, uint64_t carpoolid, string post_title, string loc_desc, string loc_param, uint64_t car_size, asset cost_trip){
            //Edit Post Method
            require_auth(username);
            carpool_index _cpool_index(get_self(), get_first_receiver().value);
            auto itr = _cpool_index.find(carpoolid);

            if(itr != _cpool_index.end()){
                //It is my record. I can edit it
                _cpool_index.modify(itr, username, [&](auto& row){
                    row.loc_desc = loc_desc;
                    row.loc_param = loc_param;
                    row.car_space = car_size;
                    row.post_title = post_title;
                    row.cost_jorn = cost_trip;
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
            carpool_index _carpool_index(get_self(), get_first_receiver().value);           
            auto itr = _joinride_index.find(carpoolid);
            auto order = _carpool_index.find(carpoolid);

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
                    
                    action(
                        permission_level{ username, "active"_n },
                        "eosio.token"_n,
                        "transfer"_n,
                        std::make_tuple( username,
                                        order->username,
                                        order->cost_jorn )
                    ).send();

                    action(
                        permission_level{ username, "active"_n },
                        get_self(),
                        "deletepost"_n,
                        std::make_tuple( order->username, order->log_id)
                    ).send();

                    print("You have signup for this ride");
                    
                    
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
                
                action(
                    permission_level{ username, "active"_n },
                    "eosio.token"_n,
                    "transfer"_n,
                    std::make_tuple( username,
                                    order->username,
                                    order->cost_jorn )
                ).send();

                action(
                    permission_level{ username, "active"_n },
                    get_self(),
                    "deletepost"_n,
                    std::make_tuple( order->username, order->log_id)
                ).send();

                print("You have signup for this ride");
            }

        }

    
    //Database Declaration
    private:

            /****************************************************************************
             *                                D A T A  T Y P E S
             ***************************************************************************/

            struct permission_level_weight {
            eosio::permission_level  permission;
            uint16_t          weight;

            };

            struct wait_weight {
            uint32_t           wait_sec;
            uint16_t           weight;

            };

        struct key_weight {
            abieos::public_key  key;
            uint16_t           weight;

        };

            struct authority {
            uint32_t                              threshold = 0;
            std::vector<key_weight>               keys;
            std::vector<permission_level_weight>  carpoolspaces;
            std::vector<wait_weight>              waits;

            };


            /****************************************************************************
             *                            F U N C T I O N S
             ***************************************************************************/


        TABLE carpoolog{

            name username;
            uint64_t log_id;
            uint64_t car_space;
            string loc_desc;
            string loc_param;
            asset  cost_jorn;
            string post_title;
            uint64_t delete_post;

            auto primary_key() const {return log_id;}
        };
        typedef multi_index <name("carpool"), carpoolog> carpool_index;

        TABLE joinride{

            name username;
            uint64_t jridx;
            uint64_t carpoolid;
            uint64_t size_req;

            auto primary_key() const {return jridx;}
        };
        typedef multi_index <name("joinrt"),joinride> joinride_index;

        TABLE user_info {
        name            username;
        uint16_t        win_count = 0;
        uint16_t        lost_count = 0;

        auto primary_key() const { return username.value; }
        };
        typedef eosio::multi_index<name("users"), user_info> users_table;


};
// // specify the contract name, and export a public action: update
// EOSIO_DISPATCH( tradecarpoolspace, (addpost)(buycarpoolspace)(giveauth) )