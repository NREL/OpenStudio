# Then modifies SWIG wrappers so they can be parsed by rdoc
# and calls rdoc.
#
# Inputs: 
#   ARGV[0] - path to binary directory containing config file
#   ARGV[1] - path to swig generated file
#   ARGV[2] - name of the module to be generated

require ARGV[0] + 'config'
require 'pathname'
require 'fileutils'

include FileUtils

# check that called from command line directly
if not ($0 == __FILE__)
  puts "#{__FILE__} called from external script"
  exit
end

# make sure parent doc folder is in place
rdocify_dir = "#{$OpenStudio_RDocifyPath}"
Dir.mkdir(rdocify_dir) if not File.exists?(rdocify_dir)

wrapper_file = ARGV[1]
name = ARGV[2]
output_dir = "#{$OpenStudio_RDocifyPath}#{name}/"

Dir.mkdir(output_dir) if not File.exists?(output_dir)

# empty output dir
Dir.glob("#{output_dir}*").each {|d| rm_rf(d)}

# get string of wrapper file to modify
str = nil
File.open(Pathname.new(wrapper_file), 'r') do |file|
  str = file.read
end

puts "Pre-processing wrapper file #{wrapper_file}."

# delete header
str = str.split("\/\* -------- TYPES TABLE \(END\) -------- \*\/")
str = str[1]

# replace SWIGINTERN with static
str.gsub!("SWIGINTERN VALUE","static VALUE")

# put static VALUE on same line as function name
str.gsub!(/static VALUE\n/,"static VALUE ")

# process by /* chunks
strArray = str.split("\/\*")
orphanDoc = []
rbDefineBlockInd = strArray.length
strArray.each_index {|i|    

  # process last part of file, which includes rb_define calls
  if strArray[i].index(/SWIGEXPORT void Init_/) != nil then 
    rbDefineBlockInd = i

    # simplify rb_define_class_under statement for derived classes
    strArray[i].gsub!(/rb_define_class_under\(([\w]+,[ "\w]+,)[^;]+/,'rb_define_class_under(\1 rb_cObject)')

    # break out of loop
    break
  end

  # process blocks that include rdoc comments, delete all others
  if strArray[i].index(/Document-[\w]*: /) == nil then 
    strArray[i] = ''
  else
    # change derived class documentation
    # get base class and delete from first line
    inheritanceDocRegex = /Document-class: ([\w:]*) < ([\w:]*)/
    ans = strArray[i].match(inheritanceDocRegex)
    if ans != nil then
      # is derived class
      baseClassStr = ans[2]
      strArray[i].sub!(inheritanceDocRegex,'Document-class: \1')
      # add sentence about base class
      strArray[i].sub!(/Proxy of ([ \S]+)/,'Proxy of \1. Derived from ' << baseClassStr << '.')
    end

    # break documentation away from rest
    brokenBlock = strArray[i].split(/\*\//)
    if brokenBlock.size() != 2
      warn "After splitting on /*, and then */, expected an array of size 2, but got an array of size " + 
           brokenBlock.size().to_s + "."
    end

    if (brokenBlock[1].strip).empty?
      # no associated code, mark as orphan
      orphanDoc[orphanDoc.length] = i
    end

    # delete unnecessary code
    keepers = ''                                 # will fill with lines to keep
    lookingFor = ''                              # master overloaded function name
    brokenBlock[1].each_line {|ln|

      # possible keeper lines
      if (ln.match(/static VALUE/) != nil) || 
         (ln.match(/_wrap_/) != nil) || 
         (ln.match(/swig_class/) != nil) then
        ln.sub!(" {","")                         # get rid of hanging open brackets
        if ln.match(/__SWIG_\d+/) == nil         # not an overloaded function
          if (lookingFor == '') || (ln.match(lookingFor) != nil)
            keepers << ln
          end
        else                                     # is an overloaded function
          ans = ln.match(/(_wrap_[\w]+)__SWIG_\d+/)  # look for master function
          lookingFor = ans[1]
        end
      end

    }
    brokenBlock[1] = keepers

    strArray[i] = brokenBlock[0]
    strArray[i] << "*/\n" << brokenBlock[1] << "\n"
    end
  }

  san = strArray.size()
  orphanDoc.each_index { |j|

    i = orphanDoc[j]
    # strArray[i] is orphan, look for matching function

    lookingFor = ''

    # getters
    if strArray[i].match(/Get value/) != nil
      lookingFor = "_get("
    end

    # setters
    if strArray[i].match(/Set new value/) != nil
      lookingFor = "_set("
    end

    # pair getters
    if strArray[i].match(/Return the first element/) != nil
      lookingFor = "_get("
    end
    if strArray[i].match(/Return the second element/) != nil
      lookingFor = "_get("
    end

    if lookingFor.empty?
      puts "Unable to determine partial function signature for orphan documentation block:"
      puts strArray[i]
      next
    end

    ans = strArray[i].match(/Document-method: (?:\w+::)+(\w+\.\w+)/)
    if ans != nil
      lookingFor = "_" + ans[1].sub(/\./,"_") + lookingFor
    else
      next
    end

    lookingFor = Regexp.escape(lookingFor)

    lnForOrphan = ''
    startInd = i+1
    endInd = rbDefineBlockInd - 1
    for k in startInd..endInd
      # break documentation away from rest
      brokenBlock = strArray[k].split(/\*\//)

      if brokenBlock.length <= 1
        next
      end

      first = true
      keepers = ''
      replace = false
      brokenBlock[1].each_line { |ln|

        if (replace == false) && (ln.match(lookingFor) != nil)
          lnForOrphan = ln
          if first
            break
          else
            # delete from strArray[k]
            replace = true    
            next
          end
        end

        if ln.match(/\w/) != nil
          keepers << ln
          first = false
        end
      }

      if replace 
        brokenBlock[1] = keepers
        strArray[k] = brokenBlock[0]
        strArray[k] << "*/\n" << brokenBlock[1] << "\n"
      end
    end

    if !lnForOrphan.empty?
      strArray[i].chomp! << lnForOrphan.strip! << "\n\n"
    else
      puts "Unable to locate function declaration for orphan code block. Was looking for: " + lookingFor + "." # + " Code block is: "
      # puts strArray[i]
    end

    if strArray.size() != san
      warn "strArray is no longer the same size after processing orphan block " + j.to_s + ", that is, strArray[" + i.to_s + "]"
    end

  }

  str = ''
  strArray.each {|subStr|
  if subStr != '' then
    str << "/*" << subStr
  end
}  

puts "Writing file #{output_dir}#{name}.cxx"

# write file
File.open("#{output_dir}#{name}.cxx","w") do |file|
  file.write(str)
end

# call rdoc
Dir.chdir(output_dir)

puts "Calling rdoc from #{output_dir}"
system($OpenStudio_RDOC)