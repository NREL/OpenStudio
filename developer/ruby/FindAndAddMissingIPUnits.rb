require 'openstudio'
require 'optparse'

# define the input parameters
options = Hash.new

optparse = OptionParser.new do |opts|

  opts.on("-f", "--[no-]fix", "Add the missing ipUnits to the OpenSutdio.idd (note: LOTS of whitespace changes...)") do |fix|
    options[:fix] = fix
  end

  opts.on( '-i', '--idd-path IDD_PATH',
          String,
          "Path to the OpenStudio.idd") do |iddPath|
    options[:iddPath] = iddPath
  end

end

# parse the input parameters
optparse.parse!

puts options


iddWrapper = nil
if options[:iddPath]
  IDD_PATH = options[:iddPath]
else
  IDD_PATH = File.absolute_path(File.join(File.dirname(__FILE__), "../../resources/model/OpenStudio.idd"))
  #iddWrapper = OpenStudio::IddFileAndFactoryWrapper.new("OpenStudio".to_IddFileType)
end

if !File.exist?(IDD_PATH)
  puts "The OpenStudio.idd at '#{IDD_PATH}' does not exist. Pass it via `-i PATH`"
  puts ""
  puts optparse.help
  exit 1
end

oIddFile = OpenStudio::IddFile::load(OpenStudio::toPath(IDD_PATH))
raise "Couldn't load #{IDD_PATH}" if oIddFile.empty?
iddWrapper = oIddFile.get

def apply_water_flow_rate(iddWrapper)

  puts "Looking for missing gal/min in Water Flow Rate fields"
  include_words = ['water', 'flow', 'rate']
  exclude_words = ['ratio', 'fraction', 'curve', 'length', 'mass']


  n_missing = 0

  iddWrapper.objects.each do |iddObject|
    num_fields = iddObject.numFields
    for i in 0..num_fields-1
      f = iddObject.getField(i).get
      f_name = f.name
      props = f.properties
      siUnits = props.units
      ipUnits = props.ipUnits
      if include_words.all? { |w| f_name.downcase.include?(w) } and exclude_words.none? { |w| f_name.downcase.include?(w) }
        if siUnits.empty?
          puts "#{iddObject.name} is missing units in field '#{f_name}'"
        elsif siUnits.get != "m3/s"
          puts "#{iddObject.name} has wrong units=#{siUnits.get} in field '#{f_name}'"
        elsif ipUnits.empty?
          puts "#{iddObject.name} is missing ipUnits in field '#{f_name}'"
          props.ipUnits = OpenStudio::OptionalString.new('gal/min')
          n_missing += 1
        elsif ipUnits.get != "gal/min"
          puts "#{iddObject.name} has wrong ipUnits=#{ipUnits.get} in field '#{f_name}'"
          props.ipUnits = OpenStudio::OptionalString.new('gal/min')
          n_missing += 1
        end
      end
    end
  end

  return n_missing
end

n_missing = 0
n_missing += apply_water_flow_rate(iddWrapper)

if options[:fix]
  puts "Fixing in place at #{IDD_PATH}"
  # LOTS of whitespace changes
  iddWrapper.save(OpenStudio::toPath(IDD_PATH), true)
else
  puts "Not fixing in place"
end

if n_missing > 0
  exit 1
else
  exit 0
end
