#pragma once
#include <memory>
#include "een_i.h"
#include "connection_factory.h"
class een_factory{
 public:
  static std::shared_ptr<een_i> create_een(std::string config,
                                          std::string spb_address,
                                          ec::connection_type local_conn_type,
                                          std::string local_address,
                                          uint local_port);
};