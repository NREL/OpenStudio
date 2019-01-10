/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef CLI_GC_VALUE_HPP
#define CLI_GC_VALUE_HPP

namespace swig {
  class GC_VALUE {
  public:
    VALUE  _obj;

    static ID hash_id;
    static ID   lt_id;
    static ID   gt_id;
    static ID   eq_id;
    static ID   le_id;
    static ID   ge_id;

    static ID  pos_id;
    static ID  neg_id;
    static ID  inv_id;

    static ID  add_id;
    static ID  sub_id;
    static ID  mul_id;
    static ID  div_id;
    static ID  mod_id;

    static ID  and_id;
    static ID   or_id;
    static ID  xor_id;

    static ID  lshift_id;
    static ID  rshift_id;

    struct OpArgs
    {
      VALUE src;
      ID    id;
      int   nargs;
      VALUE target;
    };


  public:
    GC_VALUE();

    GC_VALUE(const GC_VALUE& item);

    GC_VALUE(VALUE obj);

    ~GC_VALUE();

    GC_VALUE & operator=(const GC_VALUE& item);

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
}

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

#endif // CLI_GC_VALUE_HPP