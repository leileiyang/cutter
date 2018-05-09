#include "XmlParser.h"

bool XmlParser::ParseXml(const char *file) {
  if (doc_.load_file(file)) {
    return true;
  } else {
    return false;
  }
}

pugi::xml_node XmlParser::FindNodeByPath(const char *node_path) {
  return doc_.first_element_by_path(node_path);
}

pugi::xpath_node_set XmlParser::SelectNodes(const char *query) {
  return doc_.select_nodes(query);
}
