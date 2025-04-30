#include "utils/hex_string.hpp"

string hexString(uint64_t value, uint64_t digits, bool ellipsis, bool withPrefix) {
  std::stringstream stream;
  if (withPrefix) {
    stream << "0x";
  }
  stream << std::setfill('0') << std::setw(digits) << std::hex << std::uppercase << value;
  std::string valueStr(stream.str());
  if (ellipsis == true) {
    uint64_t valueLength = valueStr.length();
    valueStr = valueStr.substr(0, withPrefix ? 4 : 2) + ".." + valueStr.substr(valueLength - 4, 4);
  }
  return valueStr;
}