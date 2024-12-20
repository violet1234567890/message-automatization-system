#ifndef SMO_MESSAGEPROCESSOR_HPP
#define SMO_MESSAGEPROCESSOR_HPP
#include <cstdint>
#include <vector>
#include "messagebuffer.hpp"
struct Statistic {
  std::vector<std::pair<uint32_t, std::chrono::milliseconds>> dur{};
  uint32_t successRequests {};
  uint32_t failedRequests {};
  double averageTimePerRequest {};
  uint64_t sumDur {};
};
extern std::mutex devMut;

class MessageProcessor
{
 public:
  static Statistic& getStatistic();
  bool processRequest(std::shared_ptr<Request> request, uint8_t devNum);
  std::thread spawnDevice(std::shared_ptr<Request> request, uint8_t devNum){
    //allTime = std::chrono::system_clock::now();
    return std::thread(&MessageProcessor::processRequest, this, request, devNum);
  };
 private:
  inline static Statistic statistic;
  inline static uint64_t allTime{};
};
#endif
