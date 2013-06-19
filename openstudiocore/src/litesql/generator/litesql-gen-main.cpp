#include "litesql.hpp"
#include "generator.hpp"
#include "litesql-gen-cpp.hpp"
#include "litesql-gen-graphviz.hpp"
#include "litesql-gen-ruby-activerecord.hpp"
#include "logger.hpp"
#include "objectmodel.hpp"

using namespace std;
using namespace litesql;

const char* help = 
"Usage: litesql-gen [options] <my-database.xml>\n\n"
"Options:\n"
" -t, --target=TARGET                generate code for TARGET (default: c++)\n"
" -v, --verbose                      verbosely report code generation\n"
" --help                             print help\n"

" -t, --target=TARGET                generate code for TARGET (default: c++)\n"
" --output-dir=/path/to/src          output all files to directory \n"
" --output-sources=/path/to/src      output sources to directory \n"
" --output-include=/path/to/include  output includes to directory\n"
" --refresh                          refresh code of target\n"
" --overwrite                        overwrite code on generation\n"
"\n"
"Supported targets:\n"
"  'c++'        C++ target (.cpp,.hpp)\n"
"  'ruby-activerecord'          ruby target (.rb)\n"
//"  'objc'          Objective C (.m,.h)\n"
//"  'c'          C target (.c,.h)\n"
//"  'haskell'    Haskell target (.hs)\n"
//"  'sql'        SQL schema of database (.sql)\n"
//"  'php'        PHP target (.php)\n"
//"  'python'     Python target (.py)\n"
"  'graphviz'   Graphviz file (.dot)\n"
"\n\n"
;

struct options_t {
  string output_dir;
  string output_sources;
  string output_includes;
  bool refresh;
  bool printHelp;
  vector<string> targets;
};

options_t options = {"","","",true,false};

int parseArgs(int argc, char **argv) 
{
  if(argc==1)
    return -1;

  for (int i = 1; i < argc; i++) {
    string arg = argv[i];
    if (arg == "-v" || arg == "--verbose") {
      Logger::verbose(true);
      continue;
    } else if (arg == "-t" || arg == "--target") {
      if (i+1 >= argc) {
        Logger::error("Error: missing target");
        return -1;
      }    
      options.targets.push_back(argv[i+1]);
      i++;
      continue;
    } else if (litesql::startsWith(arg, "--target=")) {
      litesql::Split lang(arg, "=");
      options.targets.push_back(lang[1]);
      continue;
    }  else if (litesql::startsWith(arg, "--output-dir")) {
      litesql::Split lang(arg, "=");
      options.output_dir=lang[1];
      continue;
    } else if (litesql::startsWith(arg, "--output-sources")) {
      litesql::Split lang(arg, "=");
      options.output_sources=lang[1];
      continue;
    }  else if (litesql::startsWith(arg, "--output-include")) {
      litesql::Split lang(arg, "=");
      options.output_includes=lang[1];
      continue;
    }
    else if (arg == "--help") {
      options.printHelp = true;
      continue;
    } else if (i < argc - 1) {
      Logger::error("Error: invalid argument "+ arg);
      return -1;
    }
  }
  return 0;
}


int generateCode(ObjectModel& model)
{
    CompositeGenerator generator;
    
    generator.setOutputDirectory(options.output_dir);
    
    for (vector<string>::const_iterator target= options.targets.begin(); target!=options.targets.end();target++)
    {

      if (*target == "c++") 
      {
        CppGenerator* pCppGen = new CppGenerator();
        pCppGen->setOutputSourcesDirectory(options.output_sources);
        pCppGen->setOutputIncludesDirectory(options.output_includes);

        generator.add(pCppGen);
      }    
      else if (*target == "graphviz") 
      {
        generator.add(new GraphvizGenerator());
      }
      else if (*target == "ruby-activerecord") 
      {
        generator.add(new RubyActiveRecordGenerator());
      }
      else 
      {
        throw litesql::Except("unsupported target: " + *target);
      }
    }

    return generator.generateCode(&model)? 0 : 1 ;
}

int main(int argc, char **argv) { 

  int rc = parseArgs(argc,argv);
  if (rc!=0)
  {
    Logger::error(help);
    return -1;
  }

  if (options.printHelp) {
    cout << help << endl;
  }

  ObjectModel model;
  if (!model.loadFromFile(argv[argc-1]))
  {
    string msg = "could not load file '" + string(argv[argc-1]) + "'";
    Logger::error(msg);
    return -1;
  }
  else
  {
    try {
      return generateCode(model); 
    }
    catch (Except e) {
      Logger::error(e);
      cerr << e << endl;
      return -1;
    }
  }
}
