#ifndef SMO_MANAGER_HPP
#define SMO_MANAGER_HPP
#include <thread>
#include "messagebuffer.hpp"
#include "messageprocessor.hpp"

template <uint8_t MaxDevices>
class Manager
{
 public:
  Manager(MessageBuffer& buffer);
  void run();
  //void printStatistic(std::ostream& out);
  std::thread spawnManager() {
    return std::thread(&Manager::run, this);
  }
 private:
  std::shared_ptr<Request> selectRequestFromBuffer();
  std::optional<uint8_t> checkFreeDevice();
  //void getFreeDevice();
  std::array<MessageProcessor, MaxDevices> devices {};
  MessageBuffer& buffer;
  uint8_t nextDeviceToSearch;
};
//template< uint8_t MaxDevices >
//void Manager< MaxDevices >::getFreeDevice()
//{
//}
template< uint8_t MaxDevices >
Manager< MaxDevices >::Manager(MessageBuffer & buffer) :
  buffer{buffer}
{}
template< uint8_t MaxDevices >
std::shared_ptr<Request> Manager< MaxDevices >::selectRequestFromBuffer()
{
  auto req = buffer.getRequest();
  while (req == nullptr) {
    req = buffer.getRequest();
  }
  std::cout << "MANAGER: get request\n";
  return req;
}
template< uint8_t MaxDevices >
std::optional<uint8_t> Manager< MaxDevices >::checkFreeDevice()
{
  bool isFound;
  for (uint8_t cnt = 0; cnt < MaxDevices; cnt++) {
    if (devices[nextDeviceToSearch].isFree) {
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
//template< uint8_t MaxDevices >
//void Manager< MaxDevices >::printStatistic(std::ostream & out)
//{
//}
template< uint8_t MaxDevices >
void Manager< MaxDevices >::run()
{
  while (true)
  {
    //get statistic
    auto devNum = checkFreeDevice();
    while (devNum == std::nullopt)
    {
      devNum = checkFreeDevice();
    }
    nextDeviceToSearch++;
    auto device = devices[devNum.value()];
    std::thread deviceThread(device.spawnDevice(selectRequestFromBuffer()));
    deviceThread.join();
  }
}
#endif
