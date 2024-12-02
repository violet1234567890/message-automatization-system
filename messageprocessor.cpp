#include <iostream>
#include <chrono>
#include <thread>
#include "messageprocessor.hpp"
//Statistic & MessageProcessor::getStatistic() {
//  return statistic;
//}
bool MessageProcessor::processRequest(std::shared_ptr<Request> request) {
  isFree = false;
  std::cout << "DEVICE: Processing request\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  isFree = true;
  if (request->messageData[0] == 'A' || request->messageData[0] == 'M') {
    std::cout << "DEVICE: decline happened\n";
  }
  return true;
}
