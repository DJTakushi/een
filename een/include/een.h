#include <string>
#include <thread>

#include "connection_mqtt.h"
#include "device_client_i.h"
#include "connection_i.h"
#include "connection_factory.h"
#include "een_i.h"
typedef std::map<std::string,std::shared_ptr<device_client_i>> device_map;

class een : public een_i {
  bool stable_{true}; // will be flagged false if problems detected
  std::string group_id_{"Sparkplug B Devices"};
  std::string edge_node_id_{"C Edge Node 1"};
  std::shared_ptr<ec::connection_mqtt> spb_mosq_client_;
  std::string mqtt_host_name_{"localhost"};
  uint mqtt_host_port_{1883};
  uint mqtt_host_keepalive_{60};

  std::string topic_nbirth_;
  std::string topic_ndeath_;
  std::string topic_ndata_;
  std::string topic_ncmd_;
  std::string topic_dcmd_;

  std::shared_ptr<ec::connection_i> local_conn_;
  device_map device_map_;

  bool log_mosquitto_{false};

  void rec_local_data_msg(std::string& msg);
  void rec_local_config_msg(std::string& msg);

  void set_topics();
  bool is_active_{false};
  std::thread process_local_message_thread_;
  void process_local_message_loop();

  void nbirth_send();
  void dbirth_send();

  void ndeath_send();
  void ddeath_send();

  void ncmd_rec();
  void dcmd_rec();
  void ndata__send();
  void process_local_message_loop_start();
 public:
  een(std::string config,
      std::string spb_address,
      ec::connection_type conn_type,
      std::string local_address,
      uint local_port);
  ~een();

  bool is_stable();
};