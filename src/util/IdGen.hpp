#include <iostream>
#include <random>
#include <sstream>
#include <iomanip>
#include <array>

#ifndef UUID_HPP
#define UUID_HPP
/**
  * UUID gen code
  * Source: RFC 4122 spec.
  */

class UUIDGen {
  public:
    using UUID = std::string;
    // Static function to generate a UUID
    static std::string generateUUID() {
      // Seed random number generator
      static std::random_device rd;
      static std::mt19937 gen(rd()); // Standard random number generator
      static std::uniform_int_distribution<int> dist(0, 255); // Generate bytes in [0, 255]

      // Generate 16 random bytes for the UUID
      std::array<uint8_t, 16> uuid{};
      for (auto& byte : uuid) {
        byte = static_cast<uint8_t>(dist(gen));
      }

      // Set variant to 10xxxxxx (RFC 4122 variant)
      uuid[8] = (uuid[8] & 0xBF) | 0x80;

      // Set version to 0100 (Version 4 UUID)
      uuid[6] = (uuid[6] & 0x4F) | 0x40;

      // Format the UUID as a string
      std::ostringstream oss;
      for (size_t i = 0; i < uuid.size(); ++i) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(uuid[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) {
          oss << '-'; // Add hyphens at the correct positions
        }
      }
      return oss.str();
    }
};

#endif //UUID_HPP