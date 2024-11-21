#pragma once
#include <map>
#include <string>
#include "device_client_i.h"
#include "attribute_host.h"
class device_client : public device_client_i {
  std::string device_id_;
  std::string group_id_;
  std::string edge_node_id_;
  attribute_host attribute_host_;
  nlohmann::json config_;
  std::string topic_ddata_;
  void set_topics();
 public:
  device_client(std::string group_id,
                std::string node_id,
                nlohmann::json& config);
  org_eclipse_tahu_protobuf_Payload* get_dbirth();
  org_eclipse_tahu_protobuf_Payload* get_ddeath();
  org_eclipse_tahu_protobuf_Payload* get_ddata();
  void dcmd_pass(std::string command);
  void update(nlohmann::json& j);
  std::string get_topic_ddata_();
};