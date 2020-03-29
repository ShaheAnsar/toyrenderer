#ifndef MY_LOGGER_HPP
#define MY_LOGGER_HPP

#include <iostream>
#include <utility>

namespace logger {
  enum class pri{
		 DEB, INFO, WARN, ERR
  };
  class logger{
    std::ostream& m_stream;
  public:
    logger(std::ostream&);
    template<typename T>
    logger& operator<<(std::pair<pri, T> data){
      switch (data.first) {
      case pri::DEB:
	m_stream << "[DEBUG] ";
	break;
      case pri::INFO:
	m_stream << "[INFO] ";
	break;
      case pri::WARN:
	m_stream << "[WARNING] ";
	break;
      case pri::ERR:
	m_stream << "[ERROR] ";
	break;
      }
      m_stream << data.second << std::endl;
      return *this;
    }

    template<typename T>
    logger& operator<<(T a) {
      m_stream << "[DEBUG] " << a << std::endl;
      return *this;
    }
  };
};
#endif
