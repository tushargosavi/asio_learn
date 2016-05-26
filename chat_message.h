#ifndef CHAT_MESSAGE
#define CHAT_MESSAGE

#include <vector>
#include <iostream>
#include <ostream>

class chat_message {
public :
  
  enum { header_len = 4 };
  enum { max_data_len = 512 };
  
  chat_message() :
    body_len_(0) {
  }
  
  const char *data() const {
    return data_;
  }
  
  char *data() {
    return data_;
  }
  
  std::size_t length() {
    return header_len + body_len_;
  }
  
  const char *body() const {
    return data_ + header_len;
  }
  
  char *body() {
    return data_ + header_len;
  }
  
  std::size_t body_len() const {
    return body_len_;
  }
  
  void body_len(std::size_t new_len) {
    if (new_len > max_data_len) {
      new_len = max_data_len;
    }
    body_len_ = new_len;
  }
  
  bool decode_header() {
    char header[header_len + 1] = "";
    std::strncat(header, data_, header_len);
    size_t size = std::atoi(header);
    if (size > max_data_len) {
      body_len_ = 0;
      return false;
    }
    body_len_ = size;
    return true;
  }
  
  bool encode_header() {
    char header[header_len+1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_len_));
    std::memcpy(data_, header, header_len);
  }
      
private:
  char data_[header_len + max_data_len];
  std::size_t body_len_;
};

#endif
