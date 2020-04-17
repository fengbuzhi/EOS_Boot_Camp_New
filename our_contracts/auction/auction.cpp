#include <eosiolib/eosio.hpp>

using namespace eosio;

class auction : public eosio::contract
{

public:

      //FIXME: move it to another file
      //# of winners
      uint64_t N = 3;

      //Constructor
      auction(name s, name code, datastream<const char*> ds): _bids(s,0), contract(s, code, ds) {}

      //Calculate winners
      [[eosio::action]]
      void calculate_winners()
      {
	    winners_price = 0;
	    winners = "";

            //Get highest
            for(auto iterator = _bids.begin(); iterator != _bids.end(); ++iterator)
            {
                  if(iterator.bid > hb1)
                  {
                        hb1 = iterator.bid;
                        winner1 = iterator.owner;
                  }
            }

            //Sort the list first
            for(auto iterator = _bids.begin(); iterator != _bids.end(); ++iterator)
            {
                  if(iterator.bid > hb2 && iterator.bid != hb1)
                  {
                        hb2 = iterator.bid;
                        winner2 = iterator.owner;
                  }
            }
      }

      //Places a bid
      [[eosio::action]]
      void placebid(name owner, int64_t bid)
      {
            require_auth(owner);
            
            //Sync local variables from persistent storage
            calculate_winners();

            //Is this bid high enough? 
            if(hb1 >= bid)
            {
                  eosio::print("Your bid is too low, it's outbid. : Highest Bid: ", hb1, " : Second Highest Bid: ", hb2);
                  return;
            }

            //Set new highest bid (and last highest)
            hb2 = hb1;
            hb1 = bid;

            //If record exists delete iterator
            for(auto iterator = _bids.begin(); iterator != _bids.end();)
            {
                  if(it.owner == owner)
                  {   
                        _bids.erase(iterator);
                        break;
                  }
                  else
                  {
                        ++iterator;
                  }       
            }    

            //Create new bid record
            _bids.emplace(owner, [&](auto& rcrd)
            {
                  rcrd.owner = owner;
                  rcrd.bid = bid;
            });
      }

      //Who were the winners / who are the current winners?
      [[eosio::action]]
      void getwinners(name owner)
      {
            require_auth(owner);
            sync();
            eosio::print("The winning address: ", winner1, " : Highest Bid: ", hb1, " : Second Highest Bid: ", hb2);
      }
      
      //Dump memory (all bids and addresses)
      [[eosio::action]]
      void getbids(name owner)
      {
            require_auth(owner);
            for(auto iterator = _bids.begin(); iterator != _bids.end(); ++iterator)
                  eosio::print("Address: ", iterator.owner, " - Bid:", iterator.bid, " : ");
      }

private:

      uint64_t winners_price[N];
      name winners[N];
      
      struct [[eosio::table]] person
      {
            name student_id;
	    std::string first_name;
	    std::string last_name;
            uint64_t bid_price;
	    uint64_t rank;
            uint64_t primary_key() const{ return bid_price; }
      };

      typedef eosio::multi_index<"people"_n, person> price_index;
      price_index price_records;

};


