#include "een_factory.h"
#include "een.h"

std::shared_ptr<een_i> een_factory::create_een(std::string config,
                                                connection_type conn_type,
                                                std::string address,
                                                uint port){
  return std::make_shared<een>(config, conn_type, address, port);
}
