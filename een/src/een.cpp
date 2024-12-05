#include <iostream>
#include "tahu.h"
#include "een.h"
#include "device_client_factory.h"
#include "connection_factory.h"
een::een(std::string config,
      std::string spb_address,
      connection_type conn_type,
      std::string local_address,
      uint local_port) : een_i(config) {
  set_topics();
  spb_mosq_client_ = std::make_shared<connection_mqtt>(spb_address.c_str(),
                                                        1883,
                                                        60);
  spb_mosq_client_->subscriptions_add(topic_ncmd_);
  spb_mosq_client_->subscriptions_add(topic_dcmd_);
  spb_mosq_client_->set_will_topic(topic_ndeath_);
  spb_mosq_client_->initialize();

  stable_ &= spb_mosq_client_->is_stable();
  local_conn_ = connection_factory::create(conn_type,local_address,local_port);
  local_conn_->subscriptions_add("een");
  local_conn_->initialize();
  process_local_message_loop_start();

  /** TODO : connect to all device modules and poplulate device_map_ */
  nbirth_send();
  dbirth_send();
}
een::~een(){
  local_conn_->close();
}
void een::set_topics(){
  std::string topic_base = "spBv1.0/"+group_id_;
  topic_nbirth_ = topic_base + "/NBIRTH/" + edge_node_id_;
  topic_ndeath_ = topic_base + "/NDEATH/" + edge_node_id_;
  topic_ndata_ = topic_base + "/NDATA/" + edge_node_id_;
  topic_ncmd_ = topic_base + "/NCMD/" + edge_node_id_+"/#";
  topic_dcmd_ = topic_base + "/DCMD/" + edge_node_id_+"/#";
}

void een::nbirth_send(){
  /** TODO :  */
}
void een::dbirth_send(){
  /** TODO :  */
}

void een::ndeath_send(){
  /** TODO :  */
}
void een::ddeath_send(){
  /** TODO :  */
}
void een::ncmd_rec(){
  /** TODO :  */
}
void een::dcmd_rec(){
  /** TODO :  */
}
void een::ndata__send(){
  /** TODO :  */
}
void een::rec_local_data_msg(std::string& msg){
  /** TODO :  */
}
void een::process_local_message_loop(){
  while(is_active_){
    std::string msg;
    {
      /* wait for lock message to be avaialble;
      lock mutex while poping message*/
      std::unique_lock lk(local_conn_->mutex);
      local_conn_->cv.wait(lk, [this] { return this->local_conn_->message_available(); });
      msg = local_conn_->get_received_message();
    }
    rec_local_config_msg(msg);
  }
}

void een::process_local_message_loop_start(){
  is_active_=true;
  process_local_message_thread_ = std::thread([this](){
    this->process_local_message_loop();
  });
}

void een::rec_local_config_msg(std::string& msg){
  try {
    nlohmann::json j = nlohmann::json::parse(msg);
    if(j.contains("name")){
      std::string name = j["name"];
      if(device_map_.find(name) != device_map_.end()){
        // TODO : should we reset config here?
      }
      else{
        device_map_[name] = device_client_factory::create_device_client(
                                                            group_id_,
                                                            edge_node_id_,
                                                            j);
        std::cout<< "device_client created : "<< name<<std::endl;
      }
      device_map_[name]->update(j);
      /** TODO : bring payload generation from device_client->attribute_host to here in order to isolate tahu usage to a single place in the code */
      org_eclipse_tahu_protobuf_Payload* payload = device_map_[name]->get_ddata();
      if(payload != NULL){
        size_t buffer_length = 1024;
        uint8_t *binary_buffer = (uint8_t *)malloc(buffer_length * sizeof(uint8_t));
        size_t message_length = encode_payload(binary_buffer,
                                                buffer_length,
                                                payload);

        spb_mosq_client_->publish(NULL,
                                  device_map_[name]->get_topic_ddata_().c_str(),
                                  message_length,
                                  binary_buffer,
                                  0,
                                  false);

        free(binary_buffer);
        delete payload;
      }
    }
    else{
      std::cout << "'name' missing from message : " << msg << std::endl;
    }
  }
  catch (nlohmann::json::exception& e) {
    std::cerr << "nlohmann::json::exception : " << e.what() << std::endl;
  }
}

bool een::is_stable(){
  return stable_;
}