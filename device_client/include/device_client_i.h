#pragma once
#include <sys/types.h>
#include <tahu.h>
#include "nlohmann/json.hpp"

class device_client_i {
 public:
  virtual org_eclipse_tahu_protobuf_Payload* get_dbirth() =  0;
  virtual org_eclipse_tahu_protobuf_Payload* get_ddeath() =  0;
  virtual org_eclipse_tahu_protobuf_Payload* get_ddata() =  0;
  virtual void dcmd_pass(std::string command) =  0;
  virtual void update(nlohmann::json& j) =  0;
  virtual std::string get_topic_ddata_() =  0;
};