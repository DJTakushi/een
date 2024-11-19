#include <string>
#include <thread>

// #define USE_EDGE_MODULES
// #include "azureiot/iothub_module_client_ll.h"
// #include "azureiot/iothub_client_options.h"
// #include "azureiot/iothub_message.h"
// #include "azure_c_shared_utility/threadapi.h"
// #include "azure_c_shared_utility/crt_abstractions.h"
// #include "azure_c_shared_utility/platform.h"
// #include "azure_c_shared_utility/shared_util_options.h"
// #include "azureiot/iothubtransportmqtt.h"
// #include "azureiot/iothub.h"
#include "mosquitto_client.h"
#include "device_client.h"
#include "connection_i.h"
typedef std::map<std::string,std::shared_ptr<device_client>> device_map;

/** TODO: separate mosquitto and IotEdge content into inherited class */
class een{
  bool stable_{true}; // will be flagged false if problems detected
  std::string group_id_{"Sparkplug B Devices"};
  std::string edge_node_id_{"C Edge Node 1"};
  std::shared_ptr<mosquitto_client> mosq_client_;
  std::string mqtt_host_name_{"localhost"};
  uint mqtt_host_port_{1883};
  uint mqtt_host_keepalive_{60};

  std::string topic_nbirth_;
  std::string topic_ndeath_;
  std::string topic_ndata_;
  std::string topic_ncmd_;
  std::string topic_dcmd_;

  // IOTHUB_MODULE_CLIENT_LL_HANDLE iot_handle_{NULL};
  std::shared_ptr<connection_i> connection_;
  device_map device_map_;

  bool log_mosquitto_{false};

  void rec_local_data_msg(std::string& msg);
  void rec_local_config_msg(std::string& msg);

  // void setup_iot_hub();
  // void close_iot_hub();

  // static void PrintMessageInformation(IOTHUB_MESSAGE_HANDLE msg);
  // static IOTHUBMESSAGE_DISPOSITION_RESULT input1_message_callback (
  //                                             IOTHUB_MESSAGE_HANDLE msg,
  //                                             void* userContextCallback);
  // static IOTHUBMESSAGE_DISPOSITION_RESULT DefaultMessageCallback(
  //                                         IOTHUB_MESSAGE_HANDLE message,
  //                                         void* userContextCallback);

  void set_topics();
  std::thread een_work_thread_;
  bool is_active_{false};

 public:
  een(std::string config);
  ~een();
  void nbirth_send();
  void dbirth_send();

  void ndeath_send();
  void ddeath_send();

  void ncmd_rec();
  void dcmd_rec();
  void ndata__send();

  bool is_stable();
  void service_mqtt();
  void start_loop();
  // void service_iot_hub();
};