#include "./RubyEngine.hpp"
#include "./InitRubyEngine.hpp"
#include "./GC_Value.hpp"
#include <RubyInterpreter.hpp>
#include <iostream>
#include <embedded_files.hxx>

namespace openstudio {

void RubyEngine::initRubyEngine() {
  RUBY_INIT_STACK;
  ruby_init();

  swig::GC_VALUE::hash_id = rb_intern("hash");
  swig::GC_VALUE::lt_id = rb_intern("<");
  swig::GC_VALUE::gt_id = rb_intern(">");
  swig::GC_VALUE::eq_id = rb_intern("==");
  swig::GC_VALUE::le_id = rb_intern("<=");
  swig::GC_VALUE::ge_id = rb_intern(">=");

  swig::GC_VALUE::pos_id = rb_intern("+@");
  swig::GC_VALUE::neg_id = rb_intern("-@");
  swig::GC_VALUE::inv_id = rb_intern("~");

  swig::GC_VALUE::add_id = rb_intern("+");
  swig::GC_VALUE::sub_id = rb_intern("-");
  swig::GC_VALUE::mul_id = rb_intern("*");
  swig::GC_VALUE::div_id = rb_intern("/");
  swig::GC_VALUE::mod_id = rb_intern("%");

  swig::GC_VALUE::and_id = rb_intern("&");
  swig::GC_VALUE::or_id = rb_intern("|");
  swig::GC_VALUE::xor_id = rb_intern("^");

  swig::GC_VALUE::lshift_id = rb_intern("<<");
  swig::GC_VALUE::rshift_id = rb_intern(">>");

  // Need embedded_help for requiring files out of the embedded system
  auto embedded_extensions_string = embedded_files::getFileAsString(":/embedded_help.rb");

  try {
    rubyInterpreter.evalString(embedded_extensions_string);
  } catch (const std::exception& e) {
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Exception in embedded_help: " << e.what() << std::endl;  // endl will flush
    ruby_cleanup(1);
    throw;
  } catch (...) {
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Unknown Exception in embedded_help" << std::endl;  // endl will flush
    ruby_cleanup(1);
    throw;
  }

  // encodings
  // Get the symbols from: `DUMPBIN /ARCHIVEMEMBERS  <path/to/ruby/>/lib/enc/libenc.lib`
  Init_encdb();
  rb_provide("enc/encdb.so");
  Init_big5();
  rb_provide("enc/big5.so");
  Init_cesu_8();
  rb_provide("enc/cesu_8.so");
  Init_cp949();
  rb_provide("enc/cp949.so");
  Init_emacs_mule();
  rb_provide("enc/emacs_mule.so");
  Init_euc_jp();
  rb_provide("enc/euc_jp.so");
  Init_euc_kr();
  rb_provide("enc/euc_kr.so");
  Init_euc_tw();
  rb_provide("enc/euc_tw.so");
  Init_gb18030();
  rb_provide("enc/gb18030.so");
  Init_gb2312();
  rb_provide("enc/gb2312.so");
  Init_gbk();
  rb_provide("enc/gbk.so");
  Init_iso_8859_1();
  rb_provide("enc/iso_8859_1.so");
  Init_iso_8859_10();
  rb_provide("enc/iso_8859_10.so");
  Init_iso_8859_11();
  rb_provide("enc/iso_8859_11.so");
  Init_iso_8859_13();
  rb_provide("enc/iso_8859_13.so");
  Init_iso_8859_14();
  rb_provide("enc/iso_8859_14.so");
  Init_iso_8859_15();
  rb_provide("enc/iso_8859_15.so");
  Init_iso_8859_16();
  rb_provide("enc/iso_8859_16.so");
  Init_iso_8859_2();
  rb_provide("enc/iso_8859_2.so");
  Init_iso_8859_3();
  rb_provide("enc/iso_8859_3.so");
  Init_iso_8859_4();
  rb_provide("enc/iso_8859_4.so");
  Init_iso_8859_5();
  rb_provide("enc/iso_8859_5.so");
  Init_iso_8859_6();
  rb_provide("enc/iso_8859_6.so");
  Init_iso_8859_7();
  rb_provide("enc/iso_8859_7.so");
  Init_iso_8859_8();
  rb_provide("enc/iso_8859_8.so");
  Init_iso_8859_9();
  rb_provide("enc/iso_8859_9.so");
  Init_koi8_r();
  rb_provide("enc/koi8_r.so");
  Init_koi8_u();
  rb_provide("enc/koi8_u.so");
  Init_shift_jis();
  rb_provide("enc/shift_jis.so");
  Init_utf_16be();
  rb_provide("enc/utf_16be.so");
  Init_utf_16le();
  rb_provide("enc/utf_16le.so");
  Init_utf_32be();
  rb_provide("enc/utf_32be.so");
  Init_utf_32le();
  rb_provide("enc/utf_32le.so");
  Init_windows_1250();
  rb_provide("enc/windows_1250.so");
  Init_windows_1251();
  rb_provide("enc/windows_1251.so");
  Init_windows_1252();
  rb_provide("enc/windows_1252.so");
  Init_windows_1253();
  rb_provide("enc/windows_1253.so");
  Init_windows_1254();
  rb_provide("enc/windows_1254.so");
  Init_windows_1257();
  rb_provide("enc/windows_1257.so");
  Init_windows_31j();
  rb_provide("enc/windows_31j.so");

  // Get the symbols from: `DUMPBIN /ARCHIVEMEMBERS  <path/to/ruby>/lib/enc/libtrans.lib`
  Init_transdb();
  rb_provide("enc/trans/transdb.so");

  Init_trans_big5();
  rb_provide("enc/trans/big5.so");

  Init_trans_cesu_8();
  rb_provide("enc/trans/cesu_8.so");

  Init_trans_chinese();
  rb_provide("enc/trans/chinese.so");

  Init_trans_ebcdic();
  rb_provide("enc/trans/ebcdic.so");

  Init_trans_emoji();
  rb_provide("enc/trans/emoji.so");

  Init_trans_emoji_iso2022_kddi();
  rb_provide("enc/trans/emoji_iso2022_kddi.so");

  Init_trans_emoji_sjis_docomo();
  rb_provide("enc/trans/emoji_sjis_docomo.so");

  Init_trans_emoji_sjis_kddi();
  rb_provide("enc/trans/emoji_sjis_kddi.so");

  Init_trans_emoji_sjis_softbank();
  rb_provide("enc/trans/emoji_sjis_softbank.so");

  Init_trans_escape();
  rb_provide("enc/trans/escape.so");

  Init_trans_gb18030();
  rb_provide("enc/trans/gb18030.so");

  Init_trans_gbk();
  rb_provide("enc/trans/gbk.so");

  Init_trans_iso2022();
  rb_provide("enc/trans/iso2022.so");

  Init_trans_japanese();
  rb_provide("enc/trans/japanese.so");

  Init_trans_japanese_euc();
  rb_provide("enc/trans/japanese_euc.so");

  Init_trans_japanese_sjis();
  rb_provide("enc/trans/japanese_sjis.so");

  Init_trans_korean();
  rb_provide("enc/trans/korean.so");

  Init_trans_single_byte();
  rb_provide("enc/trans/single_byte.so");

  Init_trans_utf8_mac();
  rb_provide("enc/trans/utf8_mac.so");

  Init_trans_utf_16_32();
  rb_provide("enc/trans/utf_16_32.so");

  Init_bigdecimal();
  rb_provide("bigdecimal");
  rb_provide("bigdecimal.so");

  Init_continuation();
  rb_provide("continuation");
  rb_provide("continuation.so");

  Init_coverage();
  rb_provide("coverage");
  rb_provide("coverage.so");

  Init_cparse();
  rb_provide("cparse");
  rb_provide("racc/cparse");
  rb_provide("cparse.so");
  rb_provide("racc/cparse.so");

  Init_date_core();
  rb_provide("date_core");
  rb_provide("date_core.so");

  Init_digest();
  rb_provide("digest");
  rb_provide("digest.so");

  Init_escape();
  rb_provide("escape");
  rb_provide("escape.so");

  Init_etc();
  rb_provide("etc");
  rb_provide("etc.so");

  Init_fcntl();
  rb_provide("fcntl");
  rb_provide("fcntl.so");

  Init_fiber();
  rb_provide("fiber");
  rb_provide("fiber.so");

  Init_fiddle();
  rb_provide("fiddle");
  rb_provide("fiddle.so");

  // TODO: Fix this
  // #ifndef _WIN32
  Init_generator();
  rb_provide("json/ext/generator");
  rb_provide("json/ext/generator.so");
  // #endif

  Init_md5();
  rb_provide("md5");
  rb_provide("digest/md5");
  rb_provide("md5.so");
  rb_provide("digest/md5.so");

  Init_monitor();
  rb_provide("monitor");
  rb_provide("monitor.so");

  Init_nkf();
  rb_provide("nkf");
  rb_provide("nkf.so");

  Init_nonblock();
  rb_provide("nonblock");
  rb_provide("nonblock.so");
  rb_provide("io/nonblock");
  rb_provide("io/nonblock.so");

  Init_ruby_description();

  Init_objspace();
  rb_provide("objspace");
  rb_provide("objspace.so");

  // TODO: Fix this
  // #ifndef _WIN32
  Init_parser();
  rb_provide("json/ext/parser");
  rb_provide("json/ext/parser.so");
  // #endif

  Init_pathname();
  rb_provide("pathname");
  rb_provide("pathname.so");

  // TODO: Fix this
  // #ifndef _WIN32
  Init_psych();
  rb_provide("psych");
  rb_provide("psych.so");
  // #endif

  Init_ripper();
  rb_provide("ripper");
  rb_provide("ripper.so");

  Init_rmd160();
  rb_provide("rmd160");
  rb_provide("digest/rmd160");
  rb_provide("rmd160.so");
  rb_provide("digest/rmd160.so");

  Init_sdbm();
  rb_provide("sdbm");
  rb_provide("sdbm.so");

  Init_sha1();
  rb_provide("sha1");
  rb_provide("digest/sha1");
  rb_provide("sha1.so");
  rb_provide("digest/sha1.so");

  Init_sha2();
  rb_provide("sha2");
  rb_provide("digest/sha2");
  rb_provide("sha2.so");
  rb_provide("digest/sha2.so");

  Init_sizeof();
  rb_provide("sizeof");
  rb_provide("sizeof.so");

  Init_socket();
  rb_provide("socket");
  rb_provide("socket.so");

  Init_stringio();
  rb_provide("stringio");
  rb_provide("stringio.so");

  Init_strscan();
  rb_provide("strscan");
  rb_provide("strscan.so");

  Init_wait();
  rb_provide("wait");
  rb_provide("io/wait");
  rb_provide("wait.so");
  rb_provide("io/wait.so");

  Init_zlib();
  rb_provide("zlib");
  rb_provide("zlib.so");

  Init_openssl();
  rb_provide("openssl");
  rb_provide("openssl.so");

  Init_nonblock();
  rb_provide("io/nonblock");
  rb_provide("io/nonblock.so");

#ifndef _WIN32
  // Init_console on Windows fails to load openssl
  Init_console();
  rb_provide("console");
  rb_provide("console.so");

  Init_dbm();
  rb_provide("dbm");
  rb_provide("dbm.so");

  Init_gdbm();
  rb_provide("gdbm");
  rb_provide("gdbm.so");

  Init_pty();
  rb_provide("pty");
  rb_provide("pty.so");

  Init_readline();
  rb_provide("readline");
  rb_provide("readline.so");

  Init_syslog();
  rb_provide("syslog");
  rb_provide("syslog.so");
#endif
}

}  // namespace openstudio
