#ifndef _H_BASE_64
#define _H_BASE_64
#include <string>
using namespace std;

std::string base64_encode(unsigned char const* , unsigned int len);  
std::string base64_decode(std::string const& s); 

#endif 
