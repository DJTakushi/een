#include "een_factory.h"
#include "een.h"

std::shared_ptr<een_i> een_factory::create_een(std::string config){
  return std::make_shared<een>(config);
}
