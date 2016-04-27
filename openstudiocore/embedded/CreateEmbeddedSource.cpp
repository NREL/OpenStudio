
#include <string>
#include <fstream>
#include <iostream>

int main(int argc, char *argv[])
{
  if( argc != 5 ) {
    return 1;
  }

  auto infile = argv[1];
  auto outfile = argv[2];
  auto filenum = argv[3];
  auto embeddedname = argv[4];

  std::fstream instream(infile, std::fstream::in | std::fstream::binary);
  std::fstream outstream(outfile, std::fstream::out | std::fstream::trunc);

  if( outstream.is_open() ) {
    outstream << "static const uint8_t embedded_file_" << filenum << "[] = {";
    unsigned length = 0;
    while( 1 ) {
      auto chunk = instream.get();
      if( ! instream.eof() ) {
        if( length != 0 ) {
          outstream << ",";
        }
        outstream << "0x" << std::hex << chunk;
        ++length;
      } else {
        break;
      }
    }
    outstream << "};";

    outstream << "\n";
    outstream << "static const char *embedded_file_name_" << filenum << " = \"" << embeddedname << "\";";
    outstream << "\n";
    outstream << "static const size_t embedded_file_len_" << filenum << " = " << std::dec << length << ";";
    outstream << std::endl;

    instream.close();
    outstream.close();
  }

  return 0;
}

