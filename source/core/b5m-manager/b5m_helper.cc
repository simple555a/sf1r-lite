#include "b5m_helper.h"

using namespace sf1r;


const std::vector<std::string> B5MHelper::B5M_PROPERTY_LIST = boost::assign::list_of("DOCID")("DATE")("Picture")
  ("Source")("Category")("Url")("Price")
  ("Attribute")("Title")("Content");

const std::vector<std::string> B5MHelper::B5MO_PROPERTY_LIST = boost::assign::list_of("DOCID")("DATE")("Picture")
  ("Source")("Category")("Url")("Price")
  ("Attribute")("Title")("Content")("uuid");

const std::vector<std::string> B5MHelper::B5MP_PROPERTY_LIST = boost::assign::list_of("DOCID")("DATE")("Picture")
  ("Source")("Category")("Url")("Price")
  ("Attribute")("Title")("Content")("itemcount")("manmade");

