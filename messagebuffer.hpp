#ifndef SMO_MESSAGEBUFFER_HPP
#define SMO_MESSAGEBUFFER_HPP
#include <deque>
#include <string>
#include <mutex>
#include <optional>
#include <memory>
#include <map>
#include <vector>
#define DEVICES 6
#define USERS 3

struct UserStatistic {
  uint32_t allRequests{};
  uint32_t successfulRequests{};
  uint32_t failedRequests{};
  std::vector<double> durationsProcess{};
  std::vector<double> durationsWait{};
};

extern std::map<uint32_t, UserStatistic> statisticByUser;
extern std::mutex statMut;

using message_time = decltype(std::chrono::steady_clock::now());
struct Request {
  std::string messageData;
  uint8_t priority;
  std::string senderUsername;
  std::string receiverUsername;
  message_time time;
  uint32_t id;
  uint32_t source;
};
class MessageBuffer
{
 public:
  std::shared_ptr< Request > getRequest();
  bool putRequest(std::shared_ptr< Request > req);
 private:
  uint32_t chooseRequestToDecline();
  std::deque<std::optional<std::shared_ptr< Request >>> buffer;
  uint32_t bufferCapacity{20};
  std::mutex bufferMutex;
  uint32_t takenPlaces {};
  uint32_t successfulRequests {};
  uint32_t declinedRequests {};
};
#endif
