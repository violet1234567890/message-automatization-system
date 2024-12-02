#include <iostream>
#include <thread>
#include "messagebuffer.hpp"
#include "user.hpp"
#include "manager.hpp"
int main()
{
  MessageBuffer buf;
  User user(buf);
  Manager<5> man(buf);
  std::thread userThread(user.spawnUser());
  std::thread manThread(man.spawnManager());
  userThread.join();
  manThread.join();
  return 0;
}
