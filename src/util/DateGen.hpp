#ifndef DATEGEN_HPP
#define DATEGEN_HPP

#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

inline std::string getCurrentDateTime() {
  // Get the current time
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);

  // Format the time
  std::tm local_time = *std::localtime(&now_time);
  std::ostringstream oss;
  oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

#endif //DATEGEN_HPP
