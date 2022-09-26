#ifndef INITRUBYENGINE_included
#define INITRUBYENGINE_included

extern "C"
{
  void Init_encdb();
  void Init_big5();
  void Init_cesu_8();
  void Init_cp949();
  void Init_emacs_mule();
  void Init_euc_jp();
  void Init_euc_kr();
  void Init_euc_tw();
  void Init_gb18030();
  void Init_gb2312();
  void Init_gbk();
  void Init_iso_8859_1();
  void Init_iso_8859_10();
  void Init_iso_8859_11();
  void Init_iso_8859_13();
  void Init_iso_8859_14();
  void Init_iso_8859_15();
  void Init_iso_8859_16();
  void Init_iso_8859_2();
  void Init_iso_8859_3();
  void Init_iso_8859_4();
  void Init_iso_8859_5();
  void Init_iso_8859_6();
  void Init_iso_8859_7();
  void Init_iso_8859_8();
  void Init_iso_8859_9();
  void Init_koi8_r();
  void Init_koi8_u();
  void Init_shift_jis();
  void Init_utf_16be();
  void Init_utf_16le();
  void Init_utf_32be();
  void Init_utf_32le();
  void Init_windows_1250();
  void Init_windows_1251();
  void Init_windows_1252();
  void Init_windows_1253();
  void Init_windows_1254();
  void Init_windows_1257();
  void Init_windows_31j();

  void Init_transdb();
  void Init_trans_big5();
  void Init_trans_cesu_8();
  void Init_trans_chinese();
  void Init_trans_ebcdic();
  void Init_trans_emoji();
  void Init_trans_emoji_iso2022_kddi();
  void Init_trans_emoji_sjis_docomo();
  void Init_trans_emoji_sjis_kddi();
  void Init_trans_emoji_sjis_softbank();
  void Init_trans_escape();
  void Init_trans_gb18030();
  void Init_trans_gbk();
  void Init_trans_iso2022();
  void Init_trans_japanese();
  void Init_trans_japanese_euc();
  void Init_trans_japanese_sjis();
  void Init_trans_korean();
  void Init_trans_single_byte();
  void Init_trans_utf8_mac();
  void Init_trans_utf_16_32();

  void Init_bigdecimal();
  void Init_bigdecimal(void);
  void Init_continuation(void);
  void Init_coverage(void);
  void Init_cparse(void);
  void Init_date_core(void);
  void Init_digest(void);
  void Init_escape(void);
  void Init_etc(void);
  void Init_fcntl(void);
  void Init_fiber(void);
  void Init_fiddle(void);
  void Init_generator(void);
  void Init_md5(void);
  void Init_monitor(void);
  void Init_nkf(void);
  void Init_nonblock(void);
  void Init_objspace(void);
  void Init_parser(void);
  void Init_pathname(void);
  void Init_psych(void);
  void Init_ripper(void);
  void Init_rmd160(void);
  void Init_sdbm(void);
  void Init_sha1(void);
  void Init_sha2(void);
  void Init_sizeof(void);
  void Init_socket(void);
  void Init_stringio(void);
  void Init_strscan(void);
  void Init_wait(void);
  void Init_zlib(void);

  void Init_openssl(void);
  void Init_ruby_description(void);

#ifndef _WIN32
  void Init_console(void);
  void Init_dbm(void);
  void Init_gdbm(void);
  void Init_pty(void);
  void Init_readline(void);
  void Init_syslog(void);
#endif

}  // extern C

#endif  // INITRUBYENGINE_included
