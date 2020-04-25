#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/time.hpp>

#include "abieos_numeric.hpp"

//#include "/workspace/EOS_Boot_Camp_New/our_contracts/tradeticket/eosio.token/include/eosio.token/eosio.token.hpp"
//#include "/workspace/EOS_Boot_Camp_New/our_contracts/tradeticket/eosio.token/src/eosio.token.cpp"

#define VTOKEN symbol("VTOKEN", 0)
#define RAMCORE_SYMBOL symbol("RAMCORE", 4)
#define RAM_SYMBOL symbol("RAM", 0)


using namespace eosio;


CONTRACT tradeticket : public eosio::contract {
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
       std::vector<permission_level_weight>  tickets;
       std::vector<wait_weight>              waits;

    };

    /****************************************************************************
     *                                T A B L E S
     ***************************************************************************/

    TABLE sell_order {
      name       ticket_for_sale;
      name       seller;
      asset      price_ask{0, VTOKEN};
      time_point time_stamp;

      uint64_t primary_key() const { return ticket_for_sale.value; }
    };

    TABLE buy_order {
      name       ticket_for_sale;
      name       buyer;
      asset      price_ask{0, VTOKEN};
      time_point time_stamp;

      uint64_t primary_key() const { return ticket_for_sale.value; }
    };


    typedef eosio::multi_index< "sellorders"_n, sell_order > sell_orders;
    sell_orders _sell_orders;

    typedef eosio::multi_index< "buyorders"_n, buy_order > buy_orders;
    buy_orders _buy_orders;

    /****************************************************************************
     *                            F U N C T I O N S
     ***************************************************************************/

    //time_point current_time_point() {
    //  const static time_point ct{ microseconds{ static_cast<int64_t>( current_time() ) } };
    //  return ct;
    //}

    void create_sell_order( name  seller,
                            name  ticket_for_sale,
                            asset price_ask ) {
      /*sell_orders _sell_orders( _self, seller.value );*/
      check( _sell_orders.find( ticket_for_sale.value ) == _sell_orders.end(),
                    "you are already selling this ticket" );

      /* contract ticket pays for ram */
      auto order = _sell_orders.emplace( _self, [&]( auto& s ) {
        s.ticket_for_sale = ticket_for_sale;
        s.seller           = seller;
        s.price_ask        = price_ask;
        //s.time_stamp       = current_time_point();
      });
    }


    void create_buy_order( name  buyer,
                            name  ticket_for_sale,
                            asset price_ask ) {
      /*buy_orders _buy_orders( _self, seller.value );*/
      check( _buy_orders.find( ticket_for_sale.value ) == _buy_orders.end(),
                    "you are already buying this ticket" );

      /* contract ticket pays for ram */
      auto order = _buy_orders.emplace( _self, [&]( auto& s ) {
        s.ticket_for_sale = ticket_for_sale;
        s.buyer           = buyer;
        s.price_ask        = price_ask;
        //s.time_stamp       = current_time_point();
      });
    }


  public:
    using contract::contract;

    // constructor
    tradeticket( name receiver, name code, datastream<const char*> ds ):
                  contract( receiver, code, ds ),
                  _sell_orders( receiver, receiver.value ) ,
                  _buy_orders( receiver, receiver.value ) {}

    /****************************************************************************
     *                              A C T I O N S
     ***************************************************************************/


    ACTION sellticket( name seller, name ticket_for_sale, asset price_ask, std::string stage) {
    
      require_auth( ticket_for_sale );

      check( stage == "inseason",
                    "You can not in football season. You cannot trade your ticket" );

      check( is_account( ticket_for_sale ),
                    "ticket_for_sale does not exist");
      check( seller != ticket_for_sale,
                    "cannot sell your own ticket" );

      check( price_ask.is_valid(), "invalid price_ask" );
      check( price_ask.amount > 0, "quantity must be positive" );
      check( price_ask.symbol == VTOKEN,
                    "symbol precision mismatch" );

      create_sell_order( seller, ticket_for_sale, price_ask );
    }

    ACTION buyticket( name buyer,
                       name ticket_to_buy,
                       std::string new_public_key, 
                       std::string stage) {
      require_auth( buyer );

      check( stage == "inseason",
                    "You can not in football season. You cannot trade your ticket" );

      check( is_account( ticket_to_buy ),
                    "ticket_for_sale does not exist");
      check( buyer != ticket_to_buy,
                    "cannot buy your own ticket" );

      // find ticket for sale / seller / price
      auto order = _sell_orders.find( ticket_to_buy.value );

      check( order != _sell_orders.end(),
                    "could not find ticket for sale" );

      // buyer sends money (price) to seller
      print( order->seller, " is selling '", order->ticket_for_sale,
             "' to ", buyer, " for ", order->price_ask, "\n" );

      // For incentivised trading.
      eosio::asset revenue;
      revenue.amount = 1;
      revenue.symbol = VTOKEN;
      auto money_to_seller = order->price_ask - revenue;

      action(
         permission_level{ buyer, "active"_n },
         "eosio.token"_n,
         "transfer"_n,
         std::make_tuple( buyer,
                          order->seller,
                          money_to_seller,
                          std::string("inseason") )
      ).send();

      //Send this revenue to vtfootball
      //Need to add an account for vtfootball and set eh right
      action(
         permission_level{ buyer, "active"_n },
         "eosio.token"_n,
         "transfer"_n,
         std::make_tuple( buyer,
                          "vtfootball"_n,
                          revenue,
                          std::string("revenue") )
      ).send();

      //Update the authority
      action(
         permission_level{ order->ticket_for_sale, "active"_n },
         get_self(),
         "giveauth"_n,
         std::make_tuple( order->ticket_for_sale, new_public_key )
      ).send();
      create_buy_order( buyer, ticket_to_buy, order->price_ask);
      _sell_orders.erase( order );


    }

    ACTION giveauth( const name user, const std::string new_owner_pubkey ) {
      // permission
      // parent
      // auth
      print("*** giveauth ***\n");
      std::array<uint8_t, 33> owner_pubkey_char;

      const std::string owner_key_str = new_owner_pubkey.substr(0, 53);
      print("new key: ", owner_key_str, "\n");

      // create new public key
      const abieos::public_key owner_pubkey =
            abieos::string_to_public_key( owner_key_str );

      std::copy( owner_pubkey.data.begin(),
                 owner_pubkey.data.end(),
                 owner_pubkey_char.begin() );

      // create new authority
      const auto owner_auth = authority{ 1,
        {
          {/*key_weight*/
            {abieos::key_type::k1, owner_pubkey_char}, 1
          }
        },
        {/*tickets*/},
        {/*waits*/}
      };

      // TODO: Change this from changing the "active" permission, to
      //       changing the "owner" permission!
      action(
         permission_level{ user, "active"_n },
         "eosio"_n,
         "updateauth"_n,
         std::make_tuple( user, "active"_n, "owner"_n, owner_auth)
      ).send();

    }

};

// specify the contract name, and export a public action: update
EOSIO_DISPATCH( tradeticket, (sellticket)(buyticket)(giveauth) )
