#include "device_client_factory.h"
#include "device_client.h"

std::shared_ptr<device_client_i> device_client_factory::create_device_client(
                                                      std::string group_id,
                                                      std::string node_id,
                                                      nlohmann::json& config){
  return std::make_shared<device_client>(group_id, node_id, config);
}
