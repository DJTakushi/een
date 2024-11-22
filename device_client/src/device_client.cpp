#include <iostream>
#include "device_client.h"

device_client::device_client(std::string group_id,
                              std::string node_id,
                              nlohmann::json& config)
                              : group_id_(group_id),
                                edge_node_id_(node_id),
                                config_(config){
  device_id_ = config["name"];
  update(config);
  set_topics();
}
void device_client::set_topics(){
  topic_ddata_ = "spBv1.0/";
  topic_ddata_ += group_id_ + "/DDATA/";
  topic_ddata_ += edge_node_id_+"/"+device_id_;
}

org_eclipse_tahu_protobuf_Payload* device_client::get_dbirth(){
  // TODO :
  org_eclipse_tahu_protobuf_Payload* dbirth_payload;
  return dbirth_payload;
}
org_eclipse_tahu_protobuf_Payload* device_client::get_ddeath(){
  // TODO :
  org_eclipse_tahu_protobuf_Payload* ddeath_payload;
  return ddeath_payload;
}
org_eclipse_tahu_protobuf_Payload* device_client::get_ddata(){
  org_eclipse_tahu_protobuf_Payload* ddata_payload = new org_eclipse_tahu_protobuf_Payload;
  get_next_payload(ddata_payload);

  for(auto attr : attribute_host_.get_updated_attributes()){
    size_t attr_size;
    uint64_t tahu_datatype;
    switch (attr->get_datatype()){
      case kInteger:
        tahu_datatype = METRIC_DATA_TYPE_INT64;
        attr_size = sizeof(uint64_t);
        break;
      case kDouble:
        tahu_datatype = METRIC_DATA_TYPE_DOUBLE;
        attr_size = sizeof(double);
        break;
      case kString:
        tahu_datatype = METRIC_DATA_TYPE_STRING;
        attr_size = sizeof(std::string);
        break;
      default:
        std::cout << "unknown attribute type!" << std::endl;
        break;
    }

    add_simple_metric(ddata_payload,
                      attr->get_name().c_str(),
                      true,
                      0, // alias; should maybe be removed
                      tahu_datatype,
                      false,
                      false,
                      attr->get_value(),
                      attr_size);
  }

  if(ddata_payload->metrics_count <= 0){
    delete ddata_payload;
    ddata_payload = NULL;
  }
  return ddata_payload;
}
void device_client::dcmd_pass(std::string command){
  // TODO :
}
void device_client::update(nlohmann::json& j){
  if(j.contains("attributes")){
    attribute_host_.update_attributes_from_array(j["attributes"]);
  }
  else{
    std::cout << "attributes section not found in msg :"<< j.dump()<<std::endl;
  }
}
std::string device_client::get_topic_ddata_(){
  return topic_ddata_;
}
