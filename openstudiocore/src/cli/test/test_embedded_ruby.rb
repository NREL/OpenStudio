require 'minitest/autorun'

# test that all our static linked extensions work correctly
# these tests should all pass when run by ruby or openstudio CLI
class EmbeddedRuby_Test < Minitest::Test
  
  def test_io_open_null_device
    io = nil
    begin
      io = File.open(Gem::Util::NULL_DEVICE, 'w')
      io.puts "Hi"
      assert(true)
    ensure
      io.close
    end
  end
  
  def test_encodings
    s = "some string"
    assert_equal(Encoding::UTF_8, s.encoding)
    assert_equal(Encoding::UTF_16, s.encode('utf-16').encoding)
    assert(true)
  end
  
  def test_bigdecimal
    require 'bigdecimal'
    d = BigDecimal.new("0")
    assert(true)
  end
  
  def test_continuation
    # DLM: obsolete
    #require 'continuation'
    #arr = [ "Freddie", "Herbie", "Ron", "Max", "Ringo" ]
    #callcc{|cc| $cc = cc}
    #$cc.call unless message =~ /Max/
    #assert(true)
  end
  
  def test_coverage
    require "coverage"
    Coverage.start
    Coverage.result 
    assert(true)
  end
  
  def test_cparse
    # DLM: how to test?
    assert(true)
  end
  
  def test_date_core
    require 'date'
    Date.new(2001,2,3)
    assert(true)
  end
  
  def test_etc
    require 'etc'
    Etc.getlogin
    assert(true)
  end
  
  def test_fiber
    require 'fiber'
    
    fiber = Fiber.new do
      Fiber.yield 1
      2
    end
    fiber.resume
    fiber.resume
    assert(true)
  end
  
  #def test_fiddle
  #  require 'fiddle'
  #  Fiddle::ALIGN_CHAR
  #  assert(true)
  #end
  
  def test_generator
    g = Enumerator::Generator.new(Proc.new { 1 })
    g.to_a
    assert(true)
  end
  
  def test_json
    require 'json'
    require 'json/pure'
    my_hash = JSON.parse('{"hello": "goodbye"}')
    JSON.generate(my_hash)
  end
  
  def test_md5
    require 'digest'
    require 'digest/md5'
    Digest::MD5.hexdigest 'abc'
    assert(true)
  end
  
  def test_nkf
    require 'nkf'
    NKF.nkf("-s", 'abc')
    assert(true)
  end
  
  def test_objspace
    require 'objspace'
    ObjectSpace::allocation_sourcefile("")
    assert(true)
  end
  
  def test_pathname
    require 'pathname'
    Pathname.new("/")
    assert(true)
  end
  
  def test_pysch
    # DLM: I could not get this to work in Ruby
    # says you need to install the pysch gem?
    #require 'pysch'
    #Psych.load("--- foo")
    #assert(true)
  end
  
  def test_ripper
    require 'ripper'
    Ripper.lex("def m(a) nil end")
    assert(true)
  end
  
  def test_rmd160
    require 'digest/rmd160'
    Digest::RMD160.hexdigest 'abc' 
    assert(true)
  end
  
  def test_sdbm
    require 'sdbm'

    SDBM.open 'my_database' do |db|
      db['apple'] = 'fruit'
      db['pear'] = 'fruit'
      db['carrot'] = 'vegetable'
      db['tomato'] = 'vegetable'
    end
    assert(true)
  end
  
  def test_sha1
    require 'digest/sha1'
    s = Digest::SHA1.hexdigest 'abc' 
    #puts s
    assert(true)
  end
  
  def test_sha2
    require 'digest/sha2'
    s = Digest::SHA2.hexdigest 'abc' 
    #puts s
    assert(true)
  end
  
  def test_stringio
    require 'stringio'

    io = StringIO.new
    io.puts "Hello World"
    io.string 
    assert(true)
  end
  
  def test_strscan
    require 'strscan'
    
    s = StringScanner.new('This is an example string')
    s.eos?              
    s.scan(/\w+/)  
    assert(true)
  end
  
  def test_zlib
    require 'zlib'

    data_to_compress = '112233445566'
    data_compressed = Zlib::Deflate.deflate(data_to_compress)
    #puts "Compressed: #{data_compressed}"
    uncompressed_data = Zlib::Inflate.inflate(data_compressed)
    #puts "Uncompressed: #{uncompressed_data}"
    assert(true)
  end
  
  def test_openssl
    require 'openssl'
    s = OpenSSL::PKey::RSA.new 2048
    #puts s
    assert(true)
  end
  
  def test_parallel
    require 'parallel'
    Parallel.each(1..5, :in_processes => 2) {|i| i } #puts i}
    Parallel.each(1..5, :in_threads => 2) {|i| i } #puts i}
    assert(true)
  end
  
  def test_embedded_gems
    require 'openstudio-standards'
    require 'openstudio-workflow'
    v = OpenstudioStandards::VERSION
    #puts v
    v = OpenStudio::Workflow::VERSION
    #puts v
    assert(true)
  end
  
end