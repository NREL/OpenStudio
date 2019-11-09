#include "utilities/idd/IddFile.hpp"
#include "utilities/idf/IdfFile.hpp"
#include "utilities/idf/Workspace.hpp"
#include "utilities/core/FilesystemHelpers.hpp"


#include "utilities/core/PathHelpers.hpp"

#include <iostream>



int main()
{
  std::cout << "Model File Extension: `" << openstudio::modelFileExtension() << "'\n";
}
