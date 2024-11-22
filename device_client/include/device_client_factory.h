#pragma once
#include "device_client_i.h"
class device_client_factory{
 public:
  static std::shared_ptr<device_client_i> create_device_client(
                                            std::string group_id,
                                            std::string node_id,
                                            nlohmann::json& config);
};