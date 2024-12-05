#include "een_factory.h"
#include "een.h"

std::shared_ptr<een_i> een_factory::create_een(std::string config,
                                            std::string spb_address,
                                            ec::connection_type local_conn_type,
                                            std::string local_address,
                                            uint local_port){
  return std::make_shared<een>(config,
                                spb_address,
                                local_conn_type,
                                local_address,
                                local_port);
}
