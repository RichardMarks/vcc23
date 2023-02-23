//
// Created by Richard Marks on 2/22/23.
//

#ifndef VC23_CARTREADER_H
#define VC23_CARTREADER_H

#include <string>
#include <vector>

namespace common
{
  class CartReader
  {
  private:
    bool valid;
    std::string name;
    std::vector<unsigned char> data;
    std::vector<unsigned char> code;
  
  public:
    CartReader();
    
    void readFromFile(const std::string &fileName);
    
    std::vector<unsigned char> getData() const;
    
    std::vector<unsigned char> getCode() const;
    
    std::string getName() const;
    
    bool isValid() const
    { return valid; }
  };
}

#endif //VC23_CARTREADER_H
