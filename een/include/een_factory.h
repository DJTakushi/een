#pragma once
#include <memory>
#include "een_i.h"
class een_factory{
 public:
  static std::shared_ptr<een_i> create_een(std::string config);
};