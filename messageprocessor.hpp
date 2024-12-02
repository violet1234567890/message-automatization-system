#ifndef SMO_MESSAGEPROCESSOR_HPP
#define SMO_MESSAGEPROCESSOR_HPP
#include <cstdint>
#include "messagebuffer.hpp"
struct Statistic {
  // vector of time for all requests
  uint32_t successRequests;
  uint32_t failedRequests;
  bool isLastRequestSuccess;
  uint32_t averageTimePerRequest;
};
class MessageProcessor
{
 public:
  //Statistic& getStatistic();
  bool isFree {true};
  bool processRequest(std::shared_ptr<Request> request);
  std::thread spawnDevice(std::shared_ptr<Request> request){
    return std::thread(&MessageProcessor::processRequest, this, request);
  };
 private:
  //Statistic statistic;
};
#endif
