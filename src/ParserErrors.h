//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_PARSERERRORS_H
#define VCC23_PARSERERRORS_H

#include <string>
#include <stdexcept>
#include <sstream>

namespace vcc23
{
  class UnexpectedEndOfTokenStreamError : public std::runtime_error
  {
  private:
    static std::string getError(unsigned long attemptedOffset, unsigned long endOfStream)
    {
      std::ostringstream ss;
      ss << "Unexpected end of token stream at " << attemptedOffset << " of " << endOfStream << std::endl;
      return ss.str();
    }
  
  public:
    UnexpectedEndOfTokenStreamError(unsigned long attemptedOffset, unsigned long endOfStream)
      : std::runtime_error(getError(attemptedOffset, endOfStream))
    {
    }
  };
  
}


#endif //VCC23_PARSERERRORS_H
