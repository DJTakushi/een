#include "een_factory.h"
#include "een.h"

static std::shared_ptr<een_i> create_een(std::string config){
  return std::make_shared<een>(config);
}
