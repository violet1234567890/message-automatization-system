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
  std::thread spawnUser(uint32_t num) {
    userId = num;
    std::cout << " user id:" << num << '\n';
    return std::thread(&User::run, this, num);
  }
 private:
  void run(uint32_t num);
  inline static std::mutex printMut;
  MessageBuffer& buffer;
  void sendMessage(std::shared_ptr< Request > req, uint32_t num);
  std::shared_ptr< Request > generateRequest(uint32_t num);
  std::random_device rand{};
  std::mt19937 randEngine;
  std::uniform_int_distribution<uint16_t> dist;
  std::uniform_int_distribution<uint16_t> priorityGen;
  std::uniform_int_distribution<uint16_t> lengthGen;
  std::poisson_distribution<uint32_t> poisson;
  std::string generateString(uint8_t length);
  inline static uint32_t idCnt {};
  uint32_t declinedRequests{};
  uint32_t successfulRequests{};
  uint32_t userId{};
};
#endif
