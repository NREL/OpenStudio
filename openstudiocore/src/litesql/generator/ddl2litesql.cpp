#include "litesql.hpp"
#include "logger.hpp"
#include "objectmodel.hpp"
#include <fstream>

using namespace std;
using namespace litesql;

const char* help = 
"Usage: ddl2litesql [options] <my-database.xml>\n\n"
"Options:\n"
" -v, --verbose                      verbosely report code generation\n"
" --help                             print help\n"

" --output-dir=/path/to/src          output all files to directory \n"
"\n"
"\n\n"
;

struct options_t {
  string output_dir;
  bool printHelp;
};

options_t options = {"",false};

int parseArgs(int argc, char **argv) 
{
  if(argc<2)
    return -1;

  for (int i = 1; i < argc; i++) {
    string arg = argv[i];
    if (arg == "-v" || arg == "--verbose") {
      Logger::verbose(true);
      continue;
    } else if (litesql::startsWith(arg, "--output-dir")) {
      litesql::Split param(arg, "=");
      options.output_dir=param[1];
      continue;
    } else if (arg == "--help") {
      options.printHelp = true;
      continue;
    } else if (i < argc - 1) {
      Logger::error("Error: invalid argument "+ arg);
      return -1;
    }
  }
  return 0;
}

int main(int argc, char **argv) { 

  int rc = parseArgs(argc,argv);
  if (rc!=0)
  {
    Logger::error(help);
    return EXIT_FAILURE;
  }
  else
  {
    if (options.printHelp) {
      cout << help << endl;
    }

    ifstream is;

    is.open(argv[argc-1]);
    if (!is.is_open())
    {
      Logger::error("cant open ",argv[argc-1]);
      return EXIT_FAILURE;
    }
    else
    {
      ObjectModel model;
//    cout << argv[argc-1] << endl;

    }
  }
}
