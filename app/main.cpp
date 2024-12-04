#include <iostream>
#include <thread>
#include "een_factory.h"
#include "argument_helper.h"

void exit_application(int signum) {
  std::cout  << "exiting sub application..."<<std::endl;
  exit(0);
}
void sig_int_handler(int signum) {
  std::cout << std::endl;
  std::cout << "ctrl+c pressed, exiting..."<<std::endl;
  exit_application(1);
}

static std::string time_rfc_3339() {
  /** https://en.cppreference.com/w/cpp/chrono/c/strftim */
  std::time_t time = std::time({});
  char timeString[std::size("yyyy-mm-ddThh:mm:ssZ")];
  std::strftime(std::data(timeString), std::size(timeString),
                "%FT%TZ", std::gmtime(&time));
  return timeString;
}

int main(int argc, char* argv[]) {
  /** print app version determined from root CMakeLists.txt */
  std::cout  << time_rfc_3339() <<" : ";
  std::cout << std::string(EEN_VERSION) << " starting..." <<  std::endl;

  std::string spb_address = argument_helper::get_spb_address(argc,argv);
  connection_type type = argument_helper::get_connection_type(argc,argv);
  std::string local_address = argument_helper::get_address(argc, argv);
  uint local_port = argument_helper::get_port(argc, argv);

  std::shared_ptr<een_i> een_ = een_factory::create_een("",
                                                        spb_address,
                                                        type,
                                                        local_address,
                                                        local_port);

  while(een_->is_stable()){
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  std::cout << "exiting main.  goodbye."<<std::endl;
}