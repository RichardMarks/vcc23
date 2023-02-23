//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_CARTWRITER_H
#define VCC23_CARTWRITER_H

#include <string>
#include <vector>

namespace common
{
  
  class CartWriter
  {
  private:
    std::string name;
    std::vector<unsigned char> data;
    std::vector<unsigned char> code;
  
  public:
    void setName(const std::string &cartName);
    void setData(const std::vector<unsigned char>& dataSource);
    void setCode(const std::vector<unsigned char>& codeSource);
    void writeToFile(const std::string &fileName);
  };
  
}

#endif //VCC23_CARTWRITER_H
