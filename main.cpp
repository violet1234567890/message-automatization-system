#include <iostream>
#include <thread>
#include "messagebuffer.hpp"
#include "user.hpp"
#include "manager.hpp"
int main()
{
  MessageBuffer buf;
  std::array<User*, USERS> users;
  Manager<DEVICES> man(buf);
  std::array<std::thread, USERS> userThreads;
  for (int i = 0; i < USERS; i++) {
    User user(buf);
    users[i] = &user;
    userThreads[i] = user.spawnUser(i);
  }
  std::thread manThread(man.spawnManager());
  for (int i = 0; i < USERS; i++) {
    userThreads[i].join();
  }
  manThread.join();
  return 0;
}
