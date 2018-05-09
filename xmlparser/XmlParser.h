#ifndef XMLPARSER_XMLPARSER_H_ 
#define XMLPARSER_XMLPARSER_H_ 

#include "pugixml.hpp"

#include <vector>
#include <string>

class XmlParser {
 public:
  bool ParseXml(const char* file);
  pugi::xml_node FindNodeByPath(const char *node_path);
  pugi::xpath_node_set SelectNodes(const char *query);

  pugi::xml_document doc_;

};

#endif // XMLPARSER_XMLPARSER_H_ 
