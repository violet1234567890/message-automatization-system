#ifndef SMO_USER_HPP
#define SMO_USER_HPP
#include <memory>
#include <random>
#include <thread>
#include "messagebuffer.hpp"
class User
{
 public:
  User(MessageBuffer& buffer);
  std::thread spawnUser() {
    return std::thread(&User::run, this);
  }
 private:
  void run();
  MessageBuffer& buffer;
  void sendMessage(std::shared_ptr< Request > req);
  std::shared_ptr< Request > generateRequest();
  //как осуществить генерацию по нужному закону?
  std::random_device rand;
  std::mt19937 randEngine;
  std::uniform_int_distribution<uint16_t> dist;
  std::uniform_int_distribution<uint16_t> priorityGen;
  std::uniform_int_distribution<uint16_t> lengthGen;
  std::string generateString(uint8_t length);
};
#endif
