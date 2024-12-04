#include <iostream>
#include "messagebuffer.hpp"
std::shared_ptr< Request > MessageBuffer::getRequest()
{
  bufferMutex.lock();
  if (takenPlaces) {
    uint8_t maxPriority = 0;
    uint32_t maxId = 0;
    uint32_t chosenOne = 0;
    for (uint32_t i = 0; i < bufferCapacity; i++) {
      if (buffer[i].has_value()) {
        if (buffer[i].value()->priority > maxPriority) {
          maxPriority = buffer[i].value()->priority;
          maxId = buffer[i].value()->id;
          chosenOne = i;
        } else if (buffer[i].value()->priority == maxPriority) {
          if (buffer[i].value()->id >= maxId) {
            maxId = buffer[i].value()->id;
            chosenOne = i;
          }
        }
      }
    }
    auto req = std::make_shared<Request>(*buffer[chosenOne].value());
    buffer[chosenOne].reset();
    takenPlaces--;
    successfulRequests++;
    bufferMutex.unlock();
    return req;
  } else {
    bufferMutex.unlock();
    return {nullptr};
  }
}
bool MessageBuffer::putRequest(std::shared_ptr< Request > req)
{
  std::cout << "Requests in queue: ";
  for (int i = 0; i < bufferCapacity; i++) {
    if (buffer[i].has_value()) {
      std::cout << buffer[i].value()->id << " ";
    }
  }
  std::cout << '\n';
  if ((declinedRequests + successfulRequests) % 20 == 0) {
    std::cout << "BUFFER STATISTIC: successful requests: " << successfulRequests
      << " declined requests: " << declinedRequests << '\n';
  }
  if (takenPlaces < bufferCapacity) {
    bufferMutex.lock();
    for (uint32_t i = 0; i < bufferCapacity; i++) {
      if (not buffer[i].has_value()) {
        buffer[i] = req;
        takenPlaces++;
        bufferMutex.unlock();
        return true;
      }
    }
  }
  else {
    uint32_t decline = chooseRequestToDecline();
    declinedRequests++;
    bufferMutex.lock();
    std::cout << "BUFFER: decline request with id: " << buffer[decline].value()->id << "\n";
    buffer[decline] = req;
    bufferMutex.unlock();
    return false;
  }
  return false;
}
uint32_t MessageBuffer::chooseRequestToDecline()
{
  bufferMutex.lock();
  uint32_t minId = std::numeric_limits<uint32_t>::max();
  uint32_t chosenOne = 0;
  for (uint32_t i = 0; i < bufferCapacity; i++) {
    if (buffer[i].has_value()) {
      if (buffer[i].value()->id < minId) {
        minId = buffer[i].value()->id;
        chosenOne = i;
      }
    }
  }
  bufferMutex.unlock();
  return chosenOne;
}
