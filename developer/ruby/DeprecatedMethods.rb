# This script aims to facilitate the reporting of deprecated methods
# An amount of manual work will be necessary to add relevant information in the
# resulting deprecated_methods.csv:
# * Fill the 'Replace With' with any relevant method that supersedes it
# * Add the 'Commit SHA' to the commit that deprecated the method in question

require 'fileutils'
require 'csv'

ROOT_DIR = File.absolute_path(File.join(File.dirname(__FILE__), "../../"))
DEPRECATED_CSV_PATH = File.join(File.dirname(__FILE__), 'deprecated_methods.csv')
DEPRECATED_MD_PATH = File.join(File.dirname(__FILE__), 'deprecated_methods.md')

# Parse the CMakeLists.txt for the current version
# It greps for 'project(OpenStudio VERSION X.Y.Z)
# It will raise if something goes wrong
#
# @param cmake_path [String] The path to sdk core CMakeLists.txt
# @return [String] the resulting version, eg "2.8.0".
def parse_version_from_cmakelist(cmake_path)
  version_line = nil
  File.open(cmake_path) do |f|
    version_line = f.grep(/project\(OpenStudio VERSION/)
  end

  if version_line.nil?
    raise "Couldn't find 'project(OpenStudio VERSION' in #{cmake_path}"
  elsif !((version_line.class == Array) && (version_line.size == 1))
    raise "Expected to find only one version in #{cmake_path}, found: #{version_line}"
  end
  os_version_str = version_line[0].match(/VERSION\s+(\d+\.\d+\.\d+)/).captures[0]
  return os_version_str
end

# Parse the hpp files for newly added OS_DEPRECATED.
# Checks if already present in known_deprecated in which case it won't add it
# to the return new_deprecated for the current version, but wil add it to
# old_deprecated.
#
# @param known_deprecated [Array] The Array of Hash from CSV deprecated_methods.csv
# @return [Hash] keys are 'new' and 'old', each have a value that is an
# Array of hash of newly-added / known deprecated
def parse_new_deprecated(known_deprecated, os_version_str)
  new_deprecated = []
  old_deprecated = []
  Dir.glob( File.join(ROOT_DIR, "src/**/*.hpp") ) do |p|

    class_name = File.basename(p).sub('.hpp', '')
    content = File.read(p)
    deprecated_signatures = content.scan(/OS_DEPRECATED.* (\w+)\(.*\).*;/).flatten
    next if deprecated_signatures.empty?
    deprecated_signatures.each do |method|
      found = known_deprecated.select{|h| (h['Class Name'] == class_name) && (h['Method'] == method)}
      if !found.empty?
        old_deprecated << found[0]
        next
      end

      puts "Found new OS_DEPRECATED #{class_name}::#{method}"
      row = {
        'Namespace' => p.split('src/')[1].split('/')[0],
        'Class Name' => class_name,
        'Method' => method,
        # has to be right now since it's new
        'Deprecated Since' => os_version_str,
        'Commit SHA' =>  nil,
        'Replace With' =>  nil,
        'Removed In' =>  nil,
      }
      new_deprecated << row
    end
  end

  return {'new' => new_deprecated,
          'old' => old_deprecated}
end

# Any stuff that is in know_deprecated but NOT in old_deprecated was recently
# removed, so we update the "Removed In" field if not already present
#
# @param known_deprecated [Array] The Array of Hash from CSV deprecated_methods.csv
# @param old_deprecated [Array] The Array of Hash of the ones we did find
#
# @return None:  know_deprecated is mutated with 'Removed In'
def update_removed_deprecated(known_deprecated, old_deprecated, os_version_str)
  (known_deprecated - old_deprecated).each do |h|
    next if !h['Removed In'].nil?
    h['Removed In'] = os_version_str
  end
end

# Save to CSV (and make a copy of old CSV)
def output_to_csv(new_table)
  # Create a copy of the old CSV
  FileUtils.cp('deprecated_methods.csv', 'deprecated_methods.csv.bak')

  CSV.open(DEPRECATED_CSV_PATH, "wb") do |csv|
    csv << new_table.first.keys # adds the header
    new_table.each do |hash|
      csv << hash.values
    end
  end

  puts "Saved CSV to disk to #{DEPRECATED_CSV_PATH} (backed up old to deprecated_methods.csv.bak)"

end


def add_link_to_sha(h)
  dep = h['Deprecated Since']
  sha = h['Commit SHA']
  if dep.nil?
    return ''
  elsif sha.nil?
    return dep
  else
    return "[#{dep}](https://github.com/NREL/OpenStudio/commit/#{sha})"
  end
end

# Format output as a Markdown table
def output_to_markdown(new_table)

  File.open(DEPRECATED_MD_PATH, 'w') do |f|

    headers = ["Namespace", "Class Name", "Method", "Deprecated Since", "Replace With", "Removed In"]
    f.puts headers.join('   |    ')
    f.puts headers.map{'---'}.join('  |  ')

    new_table.each do |h|
      toprint = [
        h['Namespace'], h['Class Name'], h['Method'], add_link_to_sha(h), h['Replace With'], h['Removed In']
      ]
      f.puts toprint.join('   |    ')
    end
  end
  puts "Saved Markdown Table to disk to #{DEPRECATED_MD_PATH}"

end

cmake_path = File.join(ROOT_DIR, 'CMakeLists.txt')
os_version_str = parse_version_from_cmakelist(cmake_path)


# Load Deprecated methods
raw_data = CSV.table(DEPRECATED_CSV_PATH, {:header_converters => nil})
known_deprecated = raw_data.map { |row| row.to_hash }

x = parse_new_deprecated(known_deprecated, os_version_str)
new_deprecated = x['new']
old_deprecated = x['old']
update_removed_deprecated(known_deprecated, old_deprecated, os_version_str)

new_table = known_deprecated + new_deprecated
output_to_csv(new_table)
output_to_markdown(new_table)







