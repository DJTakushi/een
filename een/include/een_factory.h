#pragma once
#include <memory>
#include "een_i.h"
#include "connection_factory.h"
class een_factory{
 public:
  static std::shared_ptr<een_i> create_een(std::string config,
                                          connection_type conn_type,
                                          std::string address,
                                          uint port);
};