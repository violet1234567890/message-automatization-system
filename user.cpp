#include <iostream>
#include "user.hpp"

User::User(MessageBuffer& buffer) :
  buffer{buffer},
  randEngine(rand()),
  dist(65, 90),
  priorityGen(1, 3),
  lengthGen(0, 10)
  {};

void User::run() {
  while (true) {
    auto req = generateRequest();
    //std::cout << "aaaa\n";
    sendMessage(req);
  }
}
void User::sendMessage(std::shared_ptr< Request > req)
{
  buffer.putRequest(req);
  std::cout << "USER: put request\n";
  std::this_thread::sleep_for(std::chrono::milliseconds(200));
}

std::string User::generateString(uint8_t length) {
  std::string str(length, '0');
  for (uint8_t i = 0; i < length; i++) {
    uint8_t symb = dist(randEngine);
    str[i] = (char) symb;
  }
  //std::cout << "STR: " << str << '\n';
  return str;
}
std::shared_ptr< Request > User::generateRequest()
{
  uint8_t messageLength = dist(randEngine) - 60;
  uint8_t receiverLength = lengthGen(randEngine);
  uint8_t senderLength = lengthGen(randEngine);
  auto req = std::make_shared<Request>();
  req->messageData = generateString(messageLength);
  req->receiverUsername = generateString(receiverLength);
  req->senderUsername = generateString(senderLength);
  req->priority = priorityGen(randEngine);
  req->time = std::chrono::steady_clock::now();
  return req;
}
