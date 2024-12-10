#ifndef DATEGEN_HPP
#define DATEGEN_HPP

// STD
#include <chrono>
#include <iomanip>
#include <sstream>


/**
 * @brief Retrieves the current date and time in a formatted string.
 *
 * This function uses the system clock to obtain the current date and time,
 * formats it as "YYYY-MM-DD HH:MM:SS", and returns the formatted string.
 *
 * @return A string containing the current date and time in the format "YYYY-MM-DD HH:MM:SS".
 */
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
