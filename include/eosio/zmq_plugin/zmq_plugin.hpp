/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 *  @author cc32d9 <cc32d9@gmail.com>
 */
#pragma once
#include <eosio/http_plugin/http_plugin.hpp>
#include <eosio/chain_plugin/chain_plugin.hpp>
#include <appbase/application.hpp>

namespace zmqplugin {
  using namespace eosio;
  using namespace eosio::chain;
  using account_resource_limit = chain::resource_limits::account_resource_limit;

  // these structures are not defined in contract_types.hpp, so we define them here
  namespace syscontract {

    struct buyrambytes {
      account_name payer;
      account_name receiver;
      uint32_t bytes;
    };

    struct buyram {
      account_name payer;
      account_name receiver;
      asset quant;
    };

    struct sellram {
      account_name account;
      uint64_t bytes;
    };

    struct delegatebw {
      account_name from;
      account_name receiver;
      asset stake_net_quantity;
      asset stake_cpu_quantity;
      bool transfer;
    };

    struct undelegatebw {
      account_name from;
      account_name receiver;
      asset unstake_net_quantity;
      asset unstake_cpu_quantity;
    };

    struct refund {
      account_name owner;
    };

    struct regproducer {
      account_name producer;
      public_key_type producer_key;
      string url;
      uint16_t location;
    };

    struct unregprod {
      account_name producer;
    };

    struct regproxy {
      account_name proxy;
      bool isproxy;
    };

    struct voteproducer {
      account_name voter;
      account_name proxy;
      std::vector<account_name> producers;
    };

    struct claimrewards {
      account_name owner;
    };
  }

  namespace token {
    struct transfer {
      account_name from;
      account_name to;
      asset quantity;
      string memo;
    };

    struct issue {
      account_name to;
      asset quantity;
      string memo;
    };

    struct open {
      account_name owner;
      symbol symbol;
      account_name ram_payer;
    };
  }

  typedef std::map<account_name,std::map<symbol,std::set<account_name>>> assetmoves;

  struct resource_balance {
    name                       account_name;
    int64_t                    ram_quota  = 0;
    int64_t                    ram_usage = 0;
    int64_t                    net_weight = 0;
    int64_t                    cpu_weight = 0;
    account_resource_limit     net_limit;
    account_resource_limit     cpu_limit;
  };

  struct currency_balance {
    name                       account_name;
    name                       contract;
    asset                      balance;
    asset                      stake;
    asset                      refund;
    bool                       deleted = false;
  };

  struct voter_info {
    name                      owner;
    name                      proxy;
    std::vector<account_name> producers;
    int64_t                   staked;
    double                    last_vote_weight;
    double                    proxied_vote_weight;
    bool                      is_proxy;

    uint32_t                  reserved1 = 0;
    uint32_t                  reserved2 = 0;
    asset                     reserved3;
  };

  struct zmq_action_object {
    uint64_t                     global_action_seq;
    block_num_type               block_num;
    chain::block_timestamp_type  block_time;
    fc::variant                  action_trace;
    vector<resource_balance>     resource_balances;
    vector<voter_info>           voter_infos;
    vector<currency_balance>     currency_balances;
    uint32_t                     last_irreversible_block;
  };

  struct zmq_irreversible_block_object {
    block_num_type               irreversible_block_num;
    digest_type                  irreversible_block_digest;
  };

  struct zmq_fork_block_object {
    block_num_type                    invalid_block_num;
  };

  struct zmq_accepted_block_object {
    block_num_type               accepted_block_num;
    block_timestamp_type         accepted_block_timestamp;
    account_name                 accepted_block_producer;
    digest_type                  accepted_block_digest;
  };

  // see status definitions in libraries/chain/include/eosio/chain/block.hpp
  struct zmq_failed_transaction_object {
    string                                         trx_id;
    block_num_type                                 block_num;
    eosio::chain::transaction_receipt::status_enum status_name; // enum
    uint8_t                                        status_int;  // the same as status, but integer
  };

  struct zmq_accounts_info_object {
    vector<resource_balance>     resource_balances;
    vector<voter_info>           voter_infos;
    vector<currency_balance>     currency_balances;
  };
}

namespace eosio {

using namespace appbase;

class zmq_plugin : public appbase::plugin<zmq_plugin> {
public:
   zmq_plugin();
   virtual ~zmq_plugin();
 
   APPBASE_PLUGIN_REQUIRES((chain_plugin)(http_plugin))

   virtual void set_program_options(options_description&, options_description& cfg) override;
 
   void plugin_initialize(const variables_map& options);
   void plugin_startup();
   void plugin_shutdown();

private:
   std::shared_ptr<class zmq_plugin_impl> my;
};

}