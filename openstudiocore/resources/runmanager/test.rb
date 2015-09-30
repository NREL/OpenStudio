infile = File.new("in.file", "r")
outfile = File.new("out.file", "w")

outfile.puts(infile.gets.to_i + 1);

