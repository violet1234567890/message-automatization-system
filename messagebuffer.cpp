#include <iostream>
#include "messagebuffer.hpp"
std::shared_ptr< Request > MessageBuffer::getRequest()
{
  bufferMutex.lock();
  if (takenPlaces) {
    uint8_t maxPriority = 0;
    message_time maxTime;
    uint32_t chosenOne = 0;
    for (uint32_t i = 0; i < bufferCapacity; i++) {
      if (buffer[i].has_value()) {
        if (i == 0) {
          maxTime = buffer[0].value()->time;
        }
        if (buffer[i].value()->priority > maxPriority) {
          maxPriority = buffer[i].value()->priority;
          maxTime = buffer[i].value()->time;
          chosenOne = i;
        } else if (buffer[i].value()->priority == maxPriority) {
          if (buffer[i].value()->time > maxTime) {
            maxTime = buffer[i].value()->time;
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
bool MessageBuffer::putRequest(std::shared_ptr< Request > req) //return declined request?
{
  //std::cout << req->messageData << '\n';

  if (takenPlaces < bufferCapacity) {
    bufferMutex.lock();
    for (uint32_t i = 0; i < bufferCapacity; i++) {
      if (not buffer[i].has_value()) {
        buffer[i] = req;
        takenPlaces++;
        bufferMutex.unlock();
        //std::cout << sizeof(Request) << '\n';
        //std::cout << i << '\n';
        return true;
      }
    }
  }
  else {
    uint32_t decline = chooseRequestToDecline();
    declinedRequests++;
    bufferMutex.lock();
    buffer[decline] = req;
    std::cout << "BUFFER: decline request\n";
    bufferMutex.unlock();
    return false;
  }
  return false;
}
uint32_t MessageBuffer::chooseRequestToDecline()
{
  bufferMutex.lock();
  message_time minTime = buffer[0].value()->time;
  uint32_t chosenOne = 0;
  for (uint32_t i = 1; i < bufferCapacity; i++) {
    if (buffer[i].has_value()) {
      if (buffer[i].value()->time < minTime) {
        minTime = buffer[i].value()->time;
        chosenOne = i;
      }
    }
  }
  bufferMutex.unlock();
  return chosenOne;
}
