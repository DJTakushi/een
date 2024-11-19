#include <iostream>
#include "een.h"
#include "connection_factory.h"
een::een(std::string config){
  set_topics();
  std::list<std::string> subscriptions_ = { topic_ncmd_,
                                            topic_dcmd_};
  char host[40] = "localhost";
  char* mq_host = std::getenv("MQ_HOST");
  if (mq_host != NULL){
    strcpy(host,mq_host);
  }
  mosq_client_ = std::make_shared<mosquitto_client>(mq_host,
                                                    1883,
                                                    60,
                                                    subscriptions_,
                                                    topic_ndeath_);
  stable_ &= mosq_client_->is_stable();
  // setup_iot_hub();
  connection_ = connection_factory::create(kAzureIot);

  /** TODO : connect to all device modules and poplulate device_map_ */
  nbirth_send();
  dbirth_send();
}
een::~een(){
  // close_iot_hub();
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
void een::rec_local_config_msg(std::string& msg){
  try {
    nlohmann::json j = nlohmann::json::parse(msg);
    if(j.contains("name")){
      std::string name = j["name"];
      if(device_map_.find(name) != device_map_.end()){
        // TODO : should we reset config here?
      }
      else{
        device_map_[name] = std::make_shared<device_client>(group_id_,
                                                            edge_node_id_,
                                                            j);
        std::cout<< "device_client created : "<< name<<std::endl;
      }
      device_map_[name]->update(j);
      org_eclipse_tahu_protobuf_Payload* payload = device_map_[name]->get_ddata();
      if(payload != NULL){
        size_t buffer_length = 1024;
        uint8_t *binary_buffer = (uint8_t *)malloc(buffer_length * sizeof(uint8_t));
        size_t message_length = encode_payload(binary_buffer,
                                                buffer_length,
                                                payload);

        mosq_client_->publish(NULL,
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

// void een::setup_iot_hub(){
//   if (IoTHub_Init() == 0) {
//     iot_handle_ = IoTHubModuleClient_LL_CreateFromEnvironment(MQTT_Protocol);
//     if (iot_handle_ != NULL) {
//       IOTHUB_CLIENT_RESULT client_result;
//       client_result = IoTHubModuleClient_LL_SetInputMessageCallback(
//                                                     iot_handle_,
//                                                     "input1",
//                                                     input1_message_callback,
//                                                     (void*)this);
//       if (client_result == IOTHUB_CLIENT_OK) {
//         std::cout << "input1_message_callback registered" <<std::endl;
//       }
//       else{
//         std::cerr << "ERROR : IoTHubModuleClient_LL_SetInputMessageCallback()"<<std::endl;
//         stable_ = false;
//       }

//       client_result = IoTHubModuleClient_LL_SetMessageCallback(
//                                                     iot_handle_,
//                                                     DefaultMessageCallback,
//                                                     (void*)this);
//       if (client_result == IOTHUB_CLIENT_OK) {
//         std::cout << "DefaultMessageCallback registered" <<std::endl;
//       }
//       else{
//         std::cerr << "ERROR : IoTHubModuleClient_LL_SetMessageCallback()"<<std::endl;
//         stable_ = false;
//       }

//       if (client_result == IOTHUB_CLIENT_OK) {
//         std::cout << "iot_handle_ established" <<std::endl;
//       }
//     }
//     else{
//       std::cerr <<"ERROR: IoTHubModuleClient_LL_CreateFromEnvironment failed";
//       std::cerr << std::endl;
//       stable_ = false;
//     }
//   }
//   else{
//     std::cerr << "Failed to initialize the platform."<<std::endl;
//     stable_ = false;
//   }
// }

// void een::PrintMessageInformation(IOTHUB_MESSAGE_HANDLE msg){
//   IOTHUBMESSAGE_CONTENT_TYPE contentType;
//   contentType = IoTHubMessage_GetContentType(msg);
//   if (contentType != IOTHUBMESSAGE_BYTEARRAY) {
//     /* The transport will only create messages of type IOTHUBMESSAGE_BYTEARRAY,
//       never IOTHUBMESSAGE_STRING.*/
//     std::cout << "Warning: Unknown content type "<<(int)contentType;
//     std::cout <<" received for message.  Cannot display" << std::endl;
//   }
//   else {
//     unsigned const char* messageBody;
//     size_t messageBodyLength;

//     /* IoTHubMessage_GetByteArray retrieves a shallow copy of the data.
//     Caller must NOT free messageBody.*/
//     IOTHUB_MESSAGE_RESULT messageResult;
//     messageResult = IoTHubMessage_GetByteArray(msg,
//                                                 &messageBody,
//                                                 &messageBodyLength);
//     if (messageResult != IOTHUB_MESSAGE_OK) {
//       std::cout << " WARNING: messageBody = NULL" << std::endl;
//     }
//     else {
//       std::cout << " Received Binary message."<<std::endl;
//       std::cout << " Data: <<<"<<messageBody<<std::endl;
//     }
//   }

//     // Message properties
//     const char* messageId = IoTHubMessage_GetMessageId(msg);
//     if (messageId == NULL) {
//       messageId = "<null>";
//     }

//     const char* correlationId = IoTHubMessage_GetCorrelationId(msg);
//     if (correlationId == NULL) {
//       correlationId = "<null>";
//     }

//     const char* inputQueueName = IoTHubMessage_GetInputName(msg);
//     if (inputQueueName == NULL) {
//       inputQueueName = "<null>";
//     }

//     const char* connectionModuleId = IoTHubMessage_GetConnectionModuleId(msg);
//     if ((connectionModuleId ) == NULL) {
//       connectionModuleId = "<null>";
//     }

//     const char* connectionDeviceId;
//     connectionDeviceId = IoTHubMessage_GetConnectionDeviceId(msg);
//     if ((connectionDeviceId ) == NULL) {
//       connectionDeviceId = "<null>";
//     }

//     const char* tempAlertProperty;
//     tempAlertProperty = IoTHubMessage_GetProperty(msg, "temperatureAlert");
//     if ((tempAlertProperty ) == NULL) {
//       tempAlertProperty = "<null>";
//     }

//     std::cout << " Correlation ID: "<<correlationId<<std::endl;
//     std::cout << " InputQueueName: "<<inputQueueName<<std::endl;
//     std::cout << " connectionModuleId: "<<connectionModuleId<<std::endl;
//     std::cout << " connectionDeviceId: "<<connectionDeviceId<<std::endl;
//     std::cout << " temperatureAlert: "<<tempAlertProperty<<std::endl;
// }
// IOTHUBMESSAGE_DISPOSITION_RESULT een::input1_message_callback (
//                                                 IOTHUB_MESSAGE_HANDLE msg,
//                                                 void* userContextCallback) {
//   een* een_ = (een*)userContextCallback;

//   IOTHUBMESSAGE_CONTENT_TYPE contentType = IoTHubMessage_GetContentType(msg);
//   if (contentType == IOTHUBMESSAGE_BYTEARRAY) {
//     /*  IoTHubMessage_GetByteArray retrieves a shallow copy of the data.
//         Caller must NOT free messageBody.*/
//     IOTHUB_MESSAGE_RESULT messageResult;
//     unsigned const char* messageBody;
//     size_t messageBodyLength;
//     messageResult = IoTHubMessage_GetByteArray(msg, &messageBody,
//                                                 &messageBodyLength);
//     if (messageResult == IOTHUB_MESSAGE_OK) {
//       std::string body_str((const char*)messageBody,messageBodyLength);
//       een_->rec_local_config_msg(body_str);
//     }
//     else{
//       std::cout <<" WARNING: messageBody = NULL"<<std::endl;
//     }
//   }
//   else{
//     /*  The transport will only create messages of type IOTHUBMESSAGE_BYTEARRAY,
//       never IOTHUBMESSAGE_STRING.*/
//     std::cout << "Warning: Unknown content type "<<int(contentType)<< \
//         "[%d] received for message.  Cannot display"<<std::endl;

//   }
//   return IOTHUBMESSAGE_ACCEPTED;
// }
// IOTHUBMESSAGE_DISPOSITION_RESULT een::DefaultMessageCallback(
//     IOTHUB_MESSAGE_HANDLE message, void* userContextCallback) {
//   (void)userContextCallback;
//   std::cout <<"Message arrived sent to the default queue"<<std::endl;
//   PrintMessageInformation(message);
//   return IOTHUBMESSAGE_ACCEPTED;
// }
// void een::close_iot_hub() {
  // if (iot_handle_ != NULL) {
  //   IoTHubModuleClient_LL_Destroy(iot_handle_);
  // }
  // IoTHub_Deinit();
// }

bool een::is_stable(){
  return stable_;
}

void een::service_mqtt(){
  mosq_client_->service_mqtt();
}
// void een::service_iot_hub(){
//   // IoTHubModuleClient_LL_DoWork(iot_handle_);
// }