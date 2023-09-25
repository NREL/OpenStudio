# Inserts copyright at beginning of each ruby/

require 'pathname'
require 'rubygems'
require 'fileutils'

include FileUtils


ROOT_DIR = File.absolute_path(File.join(File.dirname(__FILE__), "../../"))
LICENSE_PATH = File.join(ROOT_DIR, 'LICENSE.md')


license_lines = File.readlines(LICENSE_PATH)
# Now with short version
license_lines = [
  "OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.\n",
  "See also https://openstudio.net/license\n"
]


copyright = "/***********************************************************************************************************************\n"
ruby_copyright = "########################################################################################################################\n"
license_lines.each do |line|
  if line.strip.empty?
    copyright +=  "*" + line
    ruby_copyright += "#" + line

  else
    copyright += "*  " + line
    ruby_copyright += "#  " + line
  end
end
copyright += "***********************************************************************************************************************/\n\n"
ruby_copyright += "########################################################################################################################\n\n"

# first do c++

# exceptions are files that are not part of OpenStudio
exceptions = [
  File.join(ROOT_DIR, "ci"),
  File.join(ROOT_DIR, "developer"),
  File.join(ROOT_DIR, "src/nano/"),
  File.join(ROOT_DIR, "src/polypartition/"),
  "mainpage.hpp",
  "empty.hpp"
]

# glob for hpp and cpp
files = Dir.glob(File.join(ROOT_DIR, "**/*.[ch]pp"))
files += Dir.glob(File.join(ROOT_DIR, "**/*.[ch]xx.in"))

# reject exceptions
files.reject! do |p|
  result = false
  exceptions.each do |e|
    if p.include?(e)
      result = true
      puts p
      break
    end
  end
  result
end

# loop over all files
files.each do |p|

  # start with copyright
  text = copyright

  # read file
  File.open(p, "r") do |file|
    # read until end of current copyright
    while (line = file.gets)
      if not /^\s?[\/\*]/.match(line)
        if not line.chomp.empty?
          text += line
        end
        break
      end
    end

    # now keep rest of file
    while (line = file.gets)
      text += line
    end
  end

  # write file
  File.open(p, "w") do |file|
    file << text
  end

end

# now do ruby

# exceptions are files that are not part of OpenStudio
exceptions = []

# glob for rb and py
files = Dir.glob(File.join(ROOT_DIR, "ruby/**/*.rb"))
files += Dir.glob(File.join(ROOT_DIR, "python/**/*.py"))

# reject exceptions
files.reject! do |p|
  result = false
  exceptions.each do |e|
    if p.include?(e)
      result = true
      break
    end
  end
  result
end

# loop over all files
files.each do |p|

  # start with copyright
  text = ruby_copyright

  # read file
  File.open(p, "r") do |file|
    # read until end of current copyright
    while (line = file.gets)
      if not /^#/.match(line)
        if not line.chomp.empty?
          text += line
        end
        break
      end
    end

    # now keep rest of file
    while (line = file.gets)
      text += line
    end
  end

  # write file
  File.open(p, "w") do |file|
    file << text
  end

end
