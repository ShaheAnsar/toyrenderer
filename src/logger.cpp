#include <logger.hpp>

using namespace logger;



logger::logger::logger(std::ostream& stream) : m_stream(stream){
  m_stream << "[INFO] Enabled Logger" << std::endl;
}



