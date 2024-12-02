#ifndef SMO_MESSAGEBUFFER_HPP
#define SMO_MESSAGEBUFFER_HPP
#include <deque>
#include <string>
#include <mutex>
#include <optional>
#include <memory>

using message_time = decltype(std::chrono::steady_clock::now());
struct Request {
  std::string messageData;
  uint8_t priority;
  std::string senderUsername;
  std::string receiverUsername;
  message_time time;
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
