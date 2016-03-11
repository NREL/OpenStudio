#ifndef litesql_gen_main_hpp
#define litesql_gen_main_hpp
#include "config.h"

#include "xmlobjects.hpp"
#include <vector>
#include <string>
#include <cstdio>
void generateCode(xml::Database& db,
                  std::vector<xml::Object*>& objects,
                  std::vector<xml::Relation*>& relations);

#endif // #ifndef litesql_gen_main_hpp

