#include <iostream>
#include <fstream>
#include "user.hpp"

User::User(MessageBuffer& buffer) :
  buffer{buffer},
  randEngine(rand()),
  dist(65, 90),
  priorityGen(1, 3),
  lengthGen(0, 10),
  poisson(150)
  {};

void User::run(uint32_t num) {
  while (true) {
    auto req = generateRequest(num);
    sendMessage(req, num);
  }
}
void User::sendMessage(std::shared_ptr< Request > req, uint32_t num)
{
  if (!buffer.putRequest(req)) {
    declinedRequests++;
    statMut.lock();
    statisticByUser[num].allRequests = declinedRequests + successfulRequests;
    statisticByUser[num].failedRequests = declinedRequests;
    statMut.unlock();
  } else {
    statMut.lock();
    statisticByUser[num].allRequests = declinedRequests + successfulRequests;
    statisticByUser[num].successfulRequests = successfulRequests;
    successfulRequests++;
    statMut.unlock();
  }
  printMut.lock();
  std::cout << "USER " << num << ": put request with id: " << req->id << "\n";
  printMut.unlock();

  // statistic
  if ((declinedRequests + successfulRequests) % 100 == 0) {
    statMut.lock();
    std::ofstream out;
    out.open("users.txt");
    for (uint32_t i = 0; i < USERS; i++) {
      out << "USER " << i << " statistic: ALL: " << statisticByUser[i].allRequests << " success: "
            << statisticByUser[i].successfulRequests << " declined: " << statisticByUser[i].failedRequests << '\n';
      out << "waitDurations: ";
      for (double j : statisticByUser[i].durationsWait) {
        out << j << ' ';
      }
      out << '\n' << "Processing durations: ";
      for (double j : statisticByUser[i].durationsProcess) {
        out << j << ' ';
      }
      out << '\n';
    }
    out.close();
    statMut.unlock();
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(poisson(randEngine)));
}

std::string User::generateString(uint8_t length) {
  std::string str(length, '0');
  for (uint8_t i = 0; i < length; i++) {
    uint8_t symb = dist(randEngine);
    str[i] = (char) symb;
  }
  return str;
}
std::shared_ptr< Request > User::generateRequest(uint32_t num)
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
  printMut.lock();
  req->id = idCnt;
  req->source = num;
  idCnt++;
  printMut.unlock();
  return req;
}
