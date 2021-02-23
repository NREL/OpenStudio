require 'json'
require 'optparse'

# define the input parameters
options = Hash.new

optparse = OptionParser.new do |opts|

  opts.on( '-s', '--generated-sources-directory GENERATED_SOURCES_DIRECTORY',
          String,
          "Directory when the .cs generated files are, in build/csharp_wrapper/generated_sources") do |sourceDirectory|
    options[:generatedSourcesDirectory] = sourceDirectory
  end

  opts.on( '-i', '--ignore-existing-errors', "Ignores the known errors") do |ignoreErrors|
    options[:ignoreErrors] = ignoreErrors
  end

  opts.on( '-v', '--verbose', "Print errors that have been ignored") do |verbose|
    options[:verbose] = verbose
  end
end

# parse the input parameters
optparse.parse!

if options[:generatedSourcesDirectory]
  GENERATED_SOURCE_DIRECTORY = options[:generatedSourcesDirectory]
else
  GENERATED_SOURCE_DIRECTORY = File.absolute_path(File.join(File.dirname(__FILE__), "../../build/csharp_wrapper/generated_sources"))
  puts "Trying to default the generated-sources-directory to #{GENERATED_SOURCE_DIRECTORY}"
end

if !File.exist?(GENERATED_SOURCE_DIRECTORY)
  puts "The generated-sources-directory '#{GENERATED_SOURCE_DIRECTORY}' does not exist. Pass it via `-s DIR`"
  puts ""
  puts optparse.help
  exit 1
end

# This list was put together on 2021-02-23, based on 988dd3fcb92a7eea4a879e9543d4e2132c8ffe43
known_errors = {
  "OpenStudioUtilitiesCore" => [
    "SWIGTYPE_p_openstudio__WorkspaceObject.cs",
    "SWIGTYPE_p_DateTime.cs",
    "SWIGTYPE_p_std__thread__id.cs",
    "SWIGTYPE_p_openstudio__BCLComponent.cs"
  ],
  "OpenStudioUtilitiesGeometry" => [
    "SWIGTYPE_p_std__mapT_std__string_std__string_std__lessT_std__string_t_t.cs",
    "SWIGTYPE_p_std__vectorT_size_t_t.cs"
  ],
  "OpenStudioUtilitiesIdf" =>  [
    "SWIGTYPE_p_openstudio__ProgressBar.cs"
  ],
  "OpenStudioUtilitiesSql" =>  [
    "SWIGTYPE_p_openstudio__EpwFile.cs",
    "SWIGTYPE_p_std__vectorT_SummaryData_t.cs",
  ],
  "OpenStudioMeasure" =>  [
    "SWIGTYPE_p_std__variantT_std__monostate_bool_double_int_std__string_openstudio__path_t.cs"
  ],
  "OpenStudioUtilitiesData" =>  [
    "SWIGTYPE_p_std__variantT_std__monostate_bool_double_int_unsigned_int_std__string_std__vectorT_openstudio__Attribute_t_t.cs",
  ],
  "OpenStudioUtilitiesUnits" =>  [
    "SWIGTYPE_p_openstudio__Scale.cs"
  ],
  "OpenStudioUtilitiesTime" =>  [
    "SWIGTYPE_p_time_t.cs",
    "SWIGTYPE_p_boost__posix_time__time_duration.cs",
    "SWIGTYPE_p_tm.cs",
    "SWIGTYPE_p_std__time_t.cs"
  ],
  "OpenStudioUtilitiesFileTypes" =>  [
    "SWIGTYPE_p_std__vectorT_openstudio__EpwHoliday_t.cs",
    "SWIGTYPE_p_boost__optionalT_openstudio__EpwDesignCondition_t.cs",
    "SWIGTYPE_p_boost__optionalT_Json__Value_t.cs",
    "SWIGTYPE_p_boost__optionalT_openstudio__MeasureType_t.cs"
  ],
  "OpenStudioUtilitiesBCL" =>  [
    "SWIGTYPE_p_openstudio__filesystem__path.cs"
  ]
}

subdirectory_names = Dir.entries(GENERATED_SOURCE_DIRECTORY).select {|entry| File.directory? File.join(GENERATED_SOURCE_DIRECTORY,entry) and !(entry =='.' || entry == '..') }

all_bad_swigs = {}
n_tot = 0
n_tot_including_ignored = 0

# Subprojects that aren't really concerning
ignore_subdirs = ['OpenStudioAirflow', 'OpenStudioISOModel', 'OpenStudioModelEditor']

subdirectory_names.each do |subdirectory_name|
  next if ignore_subdirs.include?(subdirectory_name)
  subdir_path = File.join(GENERATED_SOURCE_DIRECTORY, subdirectory_name)
  bad_swigs = Dir.glob(File.join(subdir_path, "*")).select{|e| /SWIGTYPE/.match(e)}.map{|e| File.basename(e)}

  n_tot_including_ignored += bad_swigs.size

  if options[:ignoreErrors]
    this_dir_ignored_errors = known_errors[subdirectory_name]
    if !this_dir_ignored_errors.nil?
      old_n = bad_swigs.size
      bad_swigs = bad_swigs - this_dir_ignored_errors
      diff_n = old_n - bad_swigs.size
      if options[:verbose] && (diff_n > 0)
        puts "Ignoring #{diff_n} known SWIG errors in #{subdirectory_name}: #{this_dir_ignored_errors - bad_swigs}"
      end
    end
  end

  n = bad_swigs.size
  n_tot += n
  if n == 0
    # puts "Ok for #{subdirectory_name}"
  else
    # puts "Not ok for #{subdirectory_name}: #{n} bad swigs found"
    all_bad_swigs[subdirectory_name] = bad_swigs
  end
end

if n_tot > 0
  puts "In total, #{n_tot} Bad swigs found"

  puts JSON.pretty_generate(all_bad_swigs)

  if n_tot_including_ignored > n_tot
    puts "\nNote: #{n_tot_including_ignored - n_tot} errors have been ignored"
  end

  exit 1
else
  puts "#{n_tot_including_ignored - n_tot} errors have been ignored, no other errors found."
  exit 0
end
