/*
 * =============================================================================
 *       Filename:  bsonobj.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-07-01 21:06:42
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

#ifndef BSONOBJ_H
#define BSONOBJ_H

#include "c_driver/src/bson.h"

#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <utility>
#include <memory>  // std::unique_ptr
#include <tuple>

class CObjectID
{  // {{{
  public:
    CObjectID();
    CObjectID(const bson_oid_t* oid);
    CObjectID(const char* str_oid);
    const bson_oid_t* raw_oid() const;
    std::string to_string();
  private:
    bson_oid_t m_oid;
};  // }}}

class CBsonIterator
{  // {{{
  public:
    CBsonIterator(const CBsonIterator& other);
    CBsonIterator(const bson_iterator* it);
    CBsonIterator(bson_iterator* it);
    ~CBsonIterator();

    const bson_iterator* raw_data() const;
    bool more() const;
    bson_type next() const;
    bson_type type() const;
    std::string key() const;
    double value_as_double() const;
    int value_as_int() const;
    int64_t value_as_long() const;
    bson_timestamp_t value_as_timestamp() const;
    bool value_as_bool() const;
    CObjectID value_as_objectid() const;
    std::string value_as_string() const;
    CBsonIterator get_subiterator() const;

  private:
    bson_iterator* m_iterator;
};  // }}}

class CBsonObj
{  // {{{
  friend class CBsonBuilder;
  public:
    CBsonObj();
    CBsonObj(bson* b);
    CBsonObj(CBsonObj&& other);
    CBsonObj(const CBsonObj& other);
    CBsonObj(const char* str_data);
    ~CBsonObj();

//    std::string to_string(bool is_array = false, bool full = false) const;
//    void to_string(std::stringstream& ss, bool is_array = false,
//        bool full = false, int depth = 0) const;
//    int fields_count() const;
    CBsonIterator get_field(const char* field) const;
    CBsonIterator operator[] (const char* field) const {
      return get_field(field);
    }
    CBsonIterator operator[] (const std::string& field) const {
      return get_field(field.c_str());
    }
    CBsonIterator operator[] (int array_index) const {
      return get_field(std::to_string(array_index).c_str());
    }
//    bool contains_field(const std::string& field) const;
    const bson* raw_data() const;
    void print() const;
//    bool is_valid() const;
    bool is_empty() const;
    bool is_valid() const;
//    bool equal(const CBsonObj& rhs) const;
//    bool binary_equal(const CBsonObj& rhs) const;
//    int hash() const;
//    std::string md5() const;

    friend class CBsonIterator;
    typedef CBsonIterator iterator;
    CBsonIterator begin() const;

  private:
    std::unique_ptr<bson> m_bson;
    CBsonObj(CBsonObj& other) = delete;
    CBsonObj& operator= (const CBsonObj& other) = delete;
};  // }}}

class CBsonBuilder
{  // {{{
  public:
    CBsonBuilder();
    ~CBsonBuilder();
    CBsonObj obj();
    static const CBsonObj& empty_bson();
    static CBsonObj bson_exist_all_fields(
        const std::vector<std::string>& fields);
    int append_oid(const std::string& name, const CObjectID& oid = CObjectID());
    int append_int(const std::string& name, int value);
    int append_long(const std::string& name, int64_t value);
    int append_double(const std::string& name, double value);
    int append_string(const std::string& name, const std::string& value);
    int append_string(const std::string& name, const char* value);
    int append_bool(const std::string& name, bool value);
    int append_binary(const std::string& name, int type,
        const char* bin_data, int len);
    int append_null(const std::string& name);
    int append_undefined(const std::string& name);
    int append_maxkey(const std::string& name);
    int append_minkey(const std::string& name);
    int append_element(const std::string& name_or_empty,
        const CBsonIterator& it);
    int append_date(const std::string& name, int64_t milliseconds);
    int append_subobject(const std::string& name, const CBsonObj& subobject);
    int append_array(const std::string& name, const CBsonObj& items);
    // Idea come from:
    // http://stackoverflow.com/a/6894436/1032255/
    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp), void>::type
    append_array(const std::string&, const std::tuple<Tp...>& ) { }
    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == 0 && I < sizeof...(Tp), int>::type
    append_array(const std::string& name, const std::tuple<Tp...>& t)
    {
      int ret = bson_append_start_array(m_bson, name.c_str());
      if (ret != BSON_OK) return ret;
      append(std::to_string(I), std::get<I>(t));
      append_array<I + 1, Tp...>(name, t);
      return bson_append_finish_array(m_bson);
    }
    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<0 < I && I < sizeof...(Tp), void>::type
    append_array(const std::string& name, const std::tuple<Tp...>& t)
    {  // {{{
      append(std::to_string(I), std::get<I>(t));
      append_array<I + 1, Tp...>(name, t);
    }  // }}}

    CBsonBuilder& append(const std::string& name, const CObjectID& oid);
    CBsonBuilder& append(const std::string& name, int value);
    CBsonBuilder& append(const std::string& name, int64_t value);
    CBsonBuilder& append(const std::string& name, double value);
    CBsonBuilder& append(const std::string& name, const std::string& value);
    CBsonBuilder& append(const std::string& name, const char* value);
    CBsonBuilder& append(const std::string& name, bool value);
    CBsonBuilder& append(const std::string& name, const CBsonObj& subobject,
        bool is_array = false);
    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp), void>::type
    inline append(const std::string&, const std::tuple<Tp...>& ) { }
    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I < sizeof...(Tp), CBsonBuilder&>::type
    inline append(const std::string& name, const std::tuple<Tp...>& t)
    {  // {{{
      append_array(name, t);
      return *this;
    }  // }}}

  private:
    bson* m_bson;
    bool m_is_built;
    static CBsonObj m_static_empty_bson;
};  // }}}

std::ostream& operator<< (std::ostream& os, const CBsonIterator& it);
std::ostream& operator<< (std::ostream& os, const CBsonObj& obj);

#endif  // BSONOBJ_H

// vim: fdm=marker

