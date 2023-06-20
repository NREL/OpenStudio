/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef CLI_GC_VALUE_HPP
#define CLI_GC_VALUE_HPP

namespace swig {
class GC_VALUE
{
 public:
  VALUE _obj;

  static ID hash_id;
  static ID lt_id;
  static ID gt_id;
  static ID eq_id;
  static ID le_id;
  static ID ge_id;

  static ID pos_id;
  static ID neg_id;
  static ID inv_id;

  static ID add_id;
  static ID sub_id;
  static ID mul_id;
  static ID div_id;
  static ID mod_id;

  static ID and_id;
  static ID or_id;
  static ID xor_id;

  static ID lshift_id;
  static ID rshift_id;

  struct OpArgs
  {
    VALUE src;
    ID id;
    int nargs;
    VALUE target;
  };

 public:
  GC_VALUE();

  GC_VALUE(const GC_VALUE& item);

  GC_VALUE(VALUE obj);

  ~GC_VALUE();

  GC_VALUE& operator=(const GC_VALUE& item);

  operator VALUE() const;

  VALUE inspect() const;

  VALUE to_s() const;

  static VALUE swig_rescue_swallow(VALUE);

  static VALUE swig_rescue_funcall(VALUE p);

  bool relational_equal_op(const GC_VALUE& other, const ID& op_id, bool (*op_func)(const VALUE& a, const VALUE& b)) const;

  static bool operator_eq(const VALUE& a, const VALUE& b);
  static bool operator_lt(const VALUE& a, const VALUE& b);
  static bool operator_le(const VALUE& a, const VALUE& b);
  static bool operator_gt(const VALUE& a, const VALUE& b);
  static bool operator_ge(const VALUE& a, const VALUE& b);

  bool operator==(const GC_VALUE& other) const;
  bool operator<(const GC_VALUE& other) const;
  bool operator<=(const GC_VALUE& other) const;
  bool operator>(const GC_VALUE& other) const;
  bool operator>=(const GC_VALUE& other) const;

  bool operator!=(const GC_VALUE& other) const;

  GC_VALUE unary_op(const ID& op_id) const;

  GC_VALUE operator+() const;
  GC_VALUE operator-() const;
  GC_VALUE operator~() const;

  GC_VALUE binary_op(const GC_VALUE& other, const ID& op_id) const;

  GC_VALUE operator+(const GC_VALUE& other) const;
  GC_VALUE operator-(const GC_VALUE& other) const;
  GC_VALUE operator*(const GC_VALUE& other) const;
  GC_VALUE operator/(const GC_VALUE& other) const;
  GC_VALUE operator%(const GC_VALUE& other) const;
  GC_VALUE operator&(const GC_VALUE& other) const;
  GC_VALUE operator^(const GC_VALUE& other) const;
  GC_VALUE operator|(const GC_VALUE& other) const;
  GC_VALUE operator<<(const GC_VALUE& other) const;
  GC_VALUE operator>>(const GC_VALUE& other) const;
};
}  // namespace swig

ID swig::GC_VALUE::hash_id;
ID swig::GC_VALUE::lt_id;
ID swig::GC_VALUE::gt_id;
ID swig::GC_VALUE::eq_id;
ID swig::GC_VALUE::le_id;
ID swig::GC_VALUE::ge_id;
ID swig::GC_VALUE::pos_id;
ID swig::GC_VALUE::neg_id;
ID swig::GC_VALUE::inv_id;
ID swig::GC_VALUE::add_id;
ID swig::GC_VALUE::sub_id;
ID swig::GC_VALUE::mul_id;
ID swig::GC_VALUE::div_id;
ID swig::GC_VALUE::mod_id;
ID swig::GC_VALUE::and_id;
ID swig::GC_VALUE::or_id;
ID swig::GC_VALUE::xor_id;
ID swig::GC_VALUE::lshift_id;
ID swig::GC_VALUE::rshift_id;

#endif  // CLI_GC_VALUE_HPP
