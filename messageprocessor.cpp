#include <iostream>
#include <chrono>
#include <thread>
#include "messageprocessor.hpp"
#include "manager.hpp"
#include "messagebuffer.hpp"


Statistic & MessageProcessor::getStatistic() {
  return statistic;
}
std::mutex devMut;
bool MessageProcessor::processRequest(std::shared_ptr<Request> request, uint8_t devNum) {
  auto start = std::chrono::steady_clock::now();

  devMut.lock();
  Manager<DEVICES>::freeDevices[devNum] = false;
  devMut.unlock();
  std::cout << "DEVICE " << (int)devNum << ": Processing request " << request->id << "\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(200 + (uint32_t)(std::rand()) % 500));

  if (request->messageData[0] == 'A' || request->messageData[0] == 'M') {
    std::cout << "DEVICE: decline happened\n";
    statistic.failedRequests++;
  } else {
    statistic.successRequests++;
  }
  auto end = std::chrono::steady_clock::now();
  statMut.lock();
  statisticByUser[request->source].durationsWait.emplace_back(
    std::chrono::duration_cast<std::chrono::milliseconds>(start - request->time).count());
  statisticByUser[request->source].durationsProcess.emplace_back(
    std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
  statMut.unlock();
  statistic.dur.emplace_back(request->id, std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
  statistic.sumDur += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  statistic.averageTimePerRequest = statistic.sumDur / (statistic.successRequests + statistic.failedRequests);
  devMut.lock();
  Manager<DEVICES>::freeDevices[devNum] = true;
  devMut.unlock();
  return true;
}
