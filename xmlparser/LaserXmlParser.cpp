#include "LaserXmlParser.h"

#include <assert.h>

#include "../dev_cfg/PlcCfg.h"

bool LaserXmlParser::AddLayerNodes(int num) {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/PlcParam");
  if (!parent_node) {
    return false;
  }
  for (int i = 0; i < num; i++) {
    pugi::xml_node node = parent_node.append_child("Layer");
    AddLayerContents(node);
  }
  return true;
}

bool LaserXmlParser::AddLayerContents(pugi::xml_node layer_node) {
  AddProcessCfgNode(layer_node);
  AddCraftDataNodes(CRAFT_LEVELS, layer_node);
  return true;
}

bool LaserXmlParser::AddProcessCfgNode(pugi::xml_node parent_node) {
  if (!parent_node) {
    return false;
  }
  pugi::xml_node node = parent_node.append_child("ProcessCfg");
  AddProcessCfgAttributes(node);
  return true;
}

bool LaserXmlParser::AddProcessCfgAttributes(pugi::xml_node processcfg_node) {
  processcfg_node.append_attribute("no_lift") = "false";
  processcfg_node.append_attribute("pre_pierce") = "false";
  processcfg_node.append_attribute("striping") = "false";
  processcfg_node.append_attribute("cooling") = "false";
  processcfg_node.append_attribute("cutting") = 0;
  processcfg_node.append_attribute("no_follow") = "false";
  processcfg_node.append_attribute("keep_air") = "false";
  processcfg_node.append_attribute("skip") = "false";
  processcfg_node.append_attribute("craft_level") = 0;
  return true;
}

bool LaserXmlParser::AddCraftDataNodes(int num, pugi::xml_node parent_node) {
  if (!parent_node) {
    return false;
  }
  for (int i = 0; i < num; i++) {
    pugi::xml_node node = parent_node.append_child("CraftData");
    AddCraftDataAttributes(node);
  }
  return true;
}

bool LaserXmlParser::AddCraftDataAttributes(pugi::xml_node craftdata_node) {
  craftdata_node.append_attribute("enable_incr") = "false";
  craftdata_node.append_attribute("incr_time") = 300;
  craftdata_node.append_attribute("jet_height") = 10;
  craftdata_node.append_attribute("gas") = 0;
  craftdata_node.append_attribute("pressure") = 10;
  craftdata_node.append_attribute("power") = 1;
  craftdata_node.append_attribute("ratio") = 1;
  craftdata_node.append_attribute("frequency") = 50;
  craftdata_node.append_attribute("focus") = 3;
  craftdata_node.append_attribute("stay") = 3000;
  craftdata_node.append_attribute("enable_blow") = "false";
  craftdata_node.append_attribute("blow_time") = 3000;
  return true;
}

void LaserXmlParser::LayerNodeComplement(pugi::xml_node layer_node) {
  pugi::xml_node processcfg_node = layer_node.child("ProcessCfg");
  if (processcfg_node) {
    ProcessCfgAttributesComplement(processcfg_node);
  } else {
    AddProcessCfgNode(layer_node);
  }
  pugi::xml_node craftdata_node = layer_node.child("CraftData");
  if (!craftdata_node) {
    AddCraftDataNodes(CRAFT_LEVELS, layer_node);
  } else {
    int n = 0;
    for (pugi::xml_node node = craftdata_node; node;
        node = node.next_sibling("CraftData")) {
      
      CraftDataAttributesComplement(node);
      n++;
    }
    AddCraftDataNodes(CRAFT_LEVELS-n, layer_node);
  }
}

void LaserXmlParser::LayerNodesComplement(\
    pugi::xpath_node_set layer_nodes) {

  for (unsigned int i = 0; i < layer_nodes.size(); i++) {
    LayerNodeComplement(layer_nodes[i].node());
  }
}

void LaserXmlParser::CraftDataAttributesComplement(\
    pugi::xpath_node_set craftdata_nodes) {

  for (unsigned int i = 0; i < craftdata_nodes.size(); i++) {
    CraftDataAttributesComplement(craftdata_nodes[i].node());
  }
}

#define AddIfNotExist(NAME, VALUE) do { \
  attr = node.attribute(NAME); \
  if (!attr) { \
    node.append_attribute(NAME) = VALUE; \
  } \
} while (0)

void LaserXmlParser::ProcessCfgAttributesComplement(\
    pugi::xml_node node) {

  pugi::xml_attribute attr = pugi::xml_attribute();
  AddIfNotExist("no_lift", "false");
  AddIfNotExist("pre_pierce", "false");
  AddIfNotExist("striping", "false");
  AddIfNotExist("cooling", "false");
  AddIfNotExist("cutting", 0);
  AddIfNotExist("no_follow", "false");
  AddIfNotExist("keep_air", "false");
  AddIfNotExist("skip", "false");
  AddIfNotExist("craft_level", 0);
}

void LaserXmlParser::CraftDataAttributesComplement(\
    pugi::xml_node node) {

  pugi::xml_attribute attr = pugi::xml_attribute();
  AddIfNotExist("enable_incr", "false");
  AddIfNotExist("incr_time", 300);
  AddIfNotExist("jet_height", 10);
  AddIfNotExist("gas", 0);
  AddIfNotExist("pressure", 10);
  AddIfNotExist("power", 1);
  AddIfNotExist("ratio", 1);
  AddIfNotExist("frequency", 50);
  AddIfNotExist("focus", 3);
  AddIfNotExist("stay", 3000);
  AddIfNotExist("enable_blow", "false");
  AddIfNotExist("blow_time", 3000);
}

pugi::xpath_node_set LaserXmlParser::LayerNodes() {
  return SelectNodes("LaserParam/PlcParam/Layer");
}

#define AddCmdNode(CmdId, CmdArgs) do { \
  pugi::xml_node cmd_node = node.append_child("PlcCmd"); \
  cmd_node.append_attribute("CmdId") = CmdId; \
  cmd_node.append_attribute("CmdArgs") = CmdArgs; \
} while (0)

void LaserXmlParser::SetCuttingProcedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("Cutting");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("Cutting");

  AddCmdNode(LASER_POWER_CUTTING, "");
  AddCmdNode(GAS_PRESSURE_CUTTING, ""); 
  AddCmdNode(GAS_OPEN_CUTTING, ""); 
  AddCmdNode(LHC_FOLLOW_CUTTING, ""); 
  AddCmdNode(FOCUS_POSITION_CUTTING, ""); 
  AddCmdNode(LASER_PULSEFREQ_CUTTING, ""); 
  AddCmdNode(LASER_DUTYRATIO_CUTTING, ""); 
  AddCmdNode(LASER_TYPE_CUTTING, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_CUTTING, ""); 
}

void LaserXmlParser::SetPierce1Procedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("FirstPierce");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("FirstPierce");

  AddCmdNode(LASER_POWER_FIRST, "");
  AddCmdNode(GAS_PRESSURE_FIRST, ""); 
  AddCmdNode(GAS_OPEN_FIRST, ""); 
  AddCmdNode(LHC_FOLLOW_FIRST, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(FOCUS_POSITION_FIRST, ""); 
  AddCmdNode(LASER_PULSEFREQ_FIRST, ""); 
  AddCmdNode(LASER_DUTYRATIO_FIRST, ""); 
  AddCmdNode(LASER_TYPE_FIRST, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_FIRST, ""); 
  AddCmdNode(LHC_PROGRESSIVE_FIRST, ""); 
  AddCmdNode(LASER_OFF, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(DELAY_BLOW_FIRST, ""); 
}

void LaserXmlParser::SetPierce2Procedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("SecondPierce");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("SecondPierce");

  AddCmdNode(LASER_POWER_SECOND, "");
  AddCmdNode(GAS_PRESSURE_SECOND, ""); 
  AddCmdNode(GAS_OPEN_SECOND, ""); 
  AddCmdNode(LHC_FOLLOW_SECOND, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(FOCUS_POSITION_SECOND, ""); 
  AddCmdNode(LASER_PULSEFREQ_SECOND, ""); 
  AddCmdNode(LASER_DUTYRATIO_SECOND, ""); 
  AddCmdNode(LASER_TYPE_SECOND, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_SECOND, ""); 
  AddCmdNode(LHC_PROGRESSIVE_SECOND, ""); 
  AddCmdNode(LASER_OFF, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(DELAY_BLOW_SECOND, ""); 

  AddCmdNode(LHC_FOLLOW_FIRST, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(GAS_PRESSURE_FIRST, ""); 
  AddCmdNode(GAS_OPEN_FIRST, ""); 
  AddCmdNode(FOCUS_POSITION_FIRST, ""); 
  AddCmdNode(LASER_POWER_FIRST, "");
  AddCmdNode(LASER_PULSEFREQ_FIRST, ""); 
  AddCmdNode(LASER_DUTYRATIO_FIRST, ""); 
  AddCmdNode(LASER_TYPE_FIRST, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_FIRST, ""); 
  AddCmdNode(LHC_PROGRESSIVE_FIRST, ""); 
  AddCmdNode(LASER_OFF, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(DELAY_BLOW_FIRST, ""); 
}

void LaserXmlParser::SetPierce3Procedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("ThirdPierce");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("ThirdPierce");

  AddCmdNode(LASER_POWER_THIRD, "");
  AddCmdNode(GAS_PRESSURE_THIRD, ""); 
  AddCmdNode(GAS_OPEN_THIRD, ""); 
  AddCmdNode(LHC_FOLLOW_THIRD, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(FOCUS_POSITION_THIRD, ""); 
  AddCmdNode(LASER_PULSEFREQ_THIRD, ""); 
  AddCmdNode(LASER_DUTYRATIO_THIRD, ""); 
  AddCmdNode(LASER_TYPE_THIRD, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_THIRD, ""); 
  AddCmdNode(LHC_PROGRESSIVE_THIRD, ""); 
  AddCmdNode(LASER_OFF, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(DELAY_BLOW_THIRD, ""); 

  AddCmdNode(LHC_FOLLOW_SECOND, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(GAS_PRESSURE_SECOND, ""); 
  AddCmdNode(GAS_OPEN_SECOND, ""); 
  AddCmdNode(FOCUS_POSITION_SECOND, ""); 
  AddCmdNode(LASER_POWER_SECOND, "");
  AddCmdNode(LASER_PULSEFREQ_SECOND, ""); 
  AddCmdNode(LASER_DUTYRATIO_SECOND, ""); 
  AddCmdNode(LASER_TYPE_SECOND, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_SECOND, ""); 
  AddCmdNode(LHC_PROGRESSIVE_SECOND, ""); 
  AddCmdNode(LASER_OFF, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(DELAY_BLOW_SECOND, ""); 

  AddCmdNode(LHC_FOLLOW_FIRST, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(GAS_PRESSURE_FIRST, ""); 
  AddCmdNode(GAS_OPEN_FIRST, ""); 
  AddCmdNode(FOCUS_POSITION_FIRST, ""); 
  AddCmdNode(LASER_POWER_FIRST, "");
  AddCmdNode(LASER_PULSEFREQ_FIRST, ""); 
  AddCmdNode(LASER_DUTYRATIO_FIRST, ""); 
  AddCmdNode(LASER_TYPE_FIRST, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_FIRST, ""); 
  AddCmdNode(LHC_PROGRESSIVE_FIRST, ""); 
  AddCmdNode(LASER_OFF, ""); 
  AddCmdNode(LHC_Z_AXIS_HOLD, ""); 
  AddCmdNode(DELAY_BLOW_FIRST, ""); 
}

void LaserXmlParser::SetStripingProcedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("Striping");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("Striping");

  AddCmdNode(LASER_POWER_CUTTING, "");
  AddCmdNode(GAS_PRESSURE_CUTTING, ""); 
  AddCmdNode(GAS_OPEN_CUTTING, ""); 
  AddCmdNode(LHC_FOLLOW_CUTTING, ""); 
  AddCmdNode(FOCUS_POSITION_CUTTING, ""); 
  AddCmdNode(LASER_PULSEFREQ_CUTTING, ""); 
  AddCmdNode(LASER_DUTYRATIO_CUTTING, ""); 
  AddCmdNode(LASER_TYPE_CUTTING, ""); 
  AddCmdNode(LASER_ON, ""); 
  AddCmdNode(DELAY_STAY_CUTTING, ""); 
}

void LaserXmlParser::SetCoolingProcedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("Cooling");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("Cooling");

  AddCmdNode(GAS_PRESSURE_CUTTING, ""); 
  AddCmdNode(GAS_OPEN_CUTTING, ""); 
  AddCmdNode(LHC_FOLLOW_CUTTING, ""); 
  AddCmdNode(DELAY_STAY_CUTTING, "");
}

void LaserXmlParser::SetLaserOffProcedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("LaserOff");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("LaserOff");

  AddCmdNode(DELAY_BLOW_CUTTING, ""); 
  AddCmdNode(LASER_OFF, ""); 
  AddCmdNode(GAS_CLOSE_CUTTING, ""); 
  AddCmdNode(LHC_LIFT, "");
}

void LaserXmlParser::SetLaserOffShortProcedure() {
  pugi::xml_node parent_node = FindNodeByPath("LaserParam/Procedure");
  pugi::xml_node node = parent_node.child("LaserOffShort");
  if (node) {
    parent_node.remove_child(node);
  } 
  node = parent_node.append_child("LaserOffShort");

  AddCmdNode(DELAY_BLOW_CUTTING, ""); 
  AddCmdNode(LASER_OFF, ""); 
}

bool LaserXmlParser::GetLayerData(int layer, ProcessCfg &process,
    std::vector<CraftData> &craft_data) {

  pugi::xpath_node_set layer_nodes = SelectNodes("LaserParam/PlcParam/Layer");
  assert((unsigned int)layer < layer_nodes.size());
  pugi::xml_node process_node = layer_nodes[layer].node().child("ProcessCfg");  
  if (process_node) {
    process.no_lift = process_node.attribute("no_lift").as_bool();
    process.pre_pierce = process_node.attribute("pre_pierce").as_bool();
    process.striping = process_node.attribute("striping").as_bool();
    process.cooling = process_node.attribute("cooling").as_bool();
    process.cutting = process_node.attribute("cutting").as_int();
    process.no_follow = process_node.attribute("no_follow").as_bool();
    process.keep_air = process_node.attribute("keep_air").as_bool();
    process.skip = process_node.attribute("skip").as_bool();
    process.craft_level = process_node.attribute("craft_level").as_int();
  }
  pugi::xml_node craft_node = layer_nodes[layer].node().child("CraftData");  
  if (craft_node) {
    craft_data.clear();
    for (pugi::xml_node node = craft_node; node;
        node = node.next_sibling("CraftData")) {

      CraftData craftdata;
      craftdata.enable_incr = node.attribute("enable_incr").as_bool();
      craftdata.incr_time = node.attribute("incr_time").as_double();
      craftdata.jet_height = node.attribute("jet_height").as_double();
      craftdata.gas = node.attribute("gas").as_int();
      craftdata.pressure = node.attribute("pressure").as_double();
      craftdata.power = node.attribute("power").as_double();
      craftdata.ratio = node.attribute("ratio").as_double();
      craftdata.frequency = node.attribute("frequency").as_double();
      craftdata.focus = node.attribute("focus").as_double();
      craftdata.stay = node.attribute("stay").as_double();
      craftdata.enable_blow = node.attribute("enable_blow").as_bool();
      craftdata.blow_time = node.attribute("blow_time").as_double();
      craft_data.push_back(craftdata);
    }
  }
  assert(craft_data.size() == CRAFT_LEVELS);
  return true;
}
