#ifndef SMO_MANAGER_HPP
#define SMO_MANAGER_HPP
#include <thread>
#include <fstream>
#include "messagebuffer.hpp"
#include "messageprocessor.hpp"

#define DEVICES 2
#define USERS 1

template <uint8_t MaxDevices>
class Manager
{
 public:
  Manager(MessageBuffer& buffer);
  void run();
  void printStatistic(std::ostream& out, Statistic & stat);
  std::thread spawnManager() {
    return std::thread(&Manager::run, this);
  }
  inline static std::array<bool, MaxDevices> freeDevices;
 private:
  std::shared_ptr<Request> selectRequestFromBuffer();
  std::optional<uint8_t> checkFreeDevice();
  std::array<MessageProcessor*, MaxDevices> devices {};
  MessageBuffer& buffer;
  uint16_t nextDeviceToSearch;
  uint32_t allRequests {};
};
template< uint8_t MaxDevices >
Manager< MaxDevices >::Manager(MessageBuffer & buffer) :
  buffer{buffer}
{
  for (uint8_t i = 0; i < MaxDevices; i++) {
    freeDevices[i] = true;
  }
}
template< uint8_t MaxDevices >
std::shared_ptr<Request> Manager< MaxDevices >::selectRequestFromBuffer()
{
  auto req = buffer.getRequest();
  while (req == nullptr) {
    req = buffer.getRequest();
  }
  std::cout << "MANAGER: get request with id: " << req->id << "\n";
  return req;
}
template< uint8_t MaxDevices >
std::optional<uint8_t> Manager< MaxDevices >::checkFreeDevice()
{
  bool isFound;
  for (uint8_t cnt = 0; cnt < MaxDevices; cnt++) {
    devMut.lock();
    bool isfree = freeDevices[nextDeviceToSearch];
    devMut.unlock();
    if (isfree) {
      isFound = true;
      break;
    }
    if (nextDeviceToSearch == MaxDevices - 1) {
        nextDeviceToSearch = 0;
    } else {
      nextDeviceToSearch++;
    }
  }
  return isFound ? std::optional(nextDeviceToSearch) : std::nullopt;
}
template< uint8_t MaxDevices >
void Manager< MaxDevices >::printStatistic(std::ostream & out, Statistic & stat)
{
  out << "MANAGER STATISTIC: success: " << stat.successRequests << " failed: " << stat.failedRequests
      << " average time: " << stat.averageTimePerRequest << '\n';
}
template< uint8_t MaxDevices >
void Manager< MaxDevices >::run()
{
  while (true)
  {
    auto devNum = checkFreeDevice();
    while (devNum == std::nullopt)
    {
      devNum = checkFreeDevice();
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    if (nextDeviceToSearch == MaxDevices - 1) {
      nextDeviceToSearch = 0;
    } else {
      nextDeviceToSearch++;
    }
    allRequests++;
    auto* device = devices[devNum.value()];
    std::thread deviceThread(device->spawnDevice(selectRequestFromBuffer(), devNum.value()));
    deviceThread.detach();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (allRequests % 20 == 0) {
      printStatistic(std::cout, MessageProcessor::getStatistic());
    }
  }
}
#endif
