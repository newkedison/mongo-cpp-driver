/*
 * =============================================================================
 *       Filename:  bsonobj.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-07-01 21:06:38
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

#include "bsonobj.h"
#include <cstring>
#include <sstream>

///////////////////////////////////////////////////////////////////
// Common
///////////////////////////////////////////////////////////////////
std::ostream& print(std::ostream& os, const CBsonIterator& it,
    bool print_single, int nested_level, bool is_object, int item_index)
{  // {{{
  std::stringstream ss;
  for (int i = 0; i < nested_level; ++i)
    ss << "  ";
  std::string space = ss.str();
  ss << "  ";
  std::string space2 = ss.str();
  if (print_single && it.type() == BSON_EOO)
  {
    os << "End of Object" << std::endl;
    return os;
  }
  if (item_index == 0 && !print_single)
  {
    os << (is_object ? "{" : "[");
    if (it.type() == BSON_EOO)
    {
      os << (is_object ? " }" : " ]");
      return os;
    }
    os << std::endl;
  }
  std::string s_key;
  if (it.type() != BSON_EOO)
  {
    os << space2;
    if (is_object)
      os << it.key() << ": ";
  }
  switch(it.type())
  {
    case BSON_EOO:
      os << space << (is_object ? "}": "]")
        << (nested_level > 0 ? ",": "") << std::endl;
      return os;
      break;
    case BSON_DOUBLE:
      os << it.value_as_double();
      break;
    case BSON_STRING:
      os << it.value_as_string();
      break;
    case BSON_OBJECT:
    case BSON_ARRAY:
      {
        bool is_obj = it.type() == BSON_OBJECT;
        bson_iterator* sub_it = bson_iterator_alloc();
        bson_iterator_subiterator(it.raw_data(), sub_it);
        CBsonIterator sub(sub_it);
        int index = 0;
        while (sub.more())
        {
          sub.next();
          print(os, sub, false, nested_level + 1, is_obj, index++);
        }
        return os;
      }
      break;
    case BSON_BINDATA:
      os << "BinaryData";
      break;
    case BSON_UNDEFINED:
      os << "Undefined";
      break;
    case BSON_OID:
      os << "ObjectID";
      break;
    case BSON_BOOL:
      os << (it.value_as_bool() ? "true" : "false");
      break;
    case BSON_DATE:
      os << "Date:" << it.value_as_long();
      break;
    case BSON_NULL:
      os << "NULL";
      break;
    case BSON_REGEX:
      os << "Regex:" << it.value_as_string();
      break;
    case BSON_DBREF:
      os << "DBREF(Deprecated)";
      break;
    case BSON_CODE:
      os << "Code";
      break;
    case BSON_SYMBOL:
      os << "Symbol:" << it.value_as_string();
      break;
    case BSON_CODEWSCOPE:
      os << "CodeWScope";
      break;
    case BSON_INT:
      os << it.value_as_int();
      break;
    case BSON_TIMESTAMP:
      os << "Time "  << it.value_as_timestamp().t
         << ", Inc " << it.value_as_timestamp().i;
      break;
    case BSON_LONG:
      os << it.value_as_long();
      break;
    case BSON_MAXKEY:
      os << "MaxKey";
      break;
    case BSON_MINKEY:
      os << "MinKey";
      break;
  }
  if (!print_single)
  {
    os << ",";
  }
  return os << std::endl;
}  // }}}

///////////////////////////////////////////////////////////////////
// CObjectID
///////////////////////////////////////////////////////////////////
CObjectID::CObjectID()
{  // {{{
  bson_oid_gen(&m_oid);
}  // }}}

CObjectID::CObjectID(const bson_oid_t* oid)
{  // {{{
  memcpy(&m_oid, oid, sizeof(bson_oid_t));
}  // }}}

CObjectID::CObjectID(const char* str_oid)
{  // {{{
  bson_oid_from_string(&m_oid, str_oid);
}  // }}}

const bson_oid_t* CObjectID::raw_oid() const
{  // {{{
  return &m_oid;
}  // }}}

std::string CObjectID::to_string()
{  // {{{
  char s[25];
  bson_oid_to_string(&m_oid, s);
  return std::string(s);
}  // }}}
///////////////////////////////////////////////////////////////////
// CBsonIterator
///////////////////////////////////////////////////////////////////
CBsonIterator::CBsonIterator(const CBsonIterator& other)
{  // {{{
  if (&other != this)
  {
    m_iterator = bson_iterator_alloc();
    memcpy(m_iterator, other.m_iterator, sizeof(bson_iterator));
  }
}  // }}}

CBsonIterator::CBsonIterator(const bson_iterator* it)
{  // {{{
  m_iterator = bson_iterator_alloc();
  memcpy(m_iterator, it, sizeof(bson_iterator));
}  // }}}

CBsonIterator::CBsonIterator(bson_iterator* it)
{  // {{{
  m_iterator = it;
}  // }}}

CBsonIterator::~CBsonIterator()
{  // {{{
  if (m_iterator != NULL)
  {
    bson_iterator_dealloc(m_iterator);
    m_iterator = NULL;
  }
}  // }}}

const bson_iterator* CBsonIterator::raw_data() const
{  // {{{
  return m_iterator;
}  // }}}

bool CBsonIterator::more() const
{  // {{{
  return bson_iterator_more(m_iterator);
}  // }}}

bson_type CBsonIterator::next() const 
{  // {{{
  return bson_iterator_next(m_iterator);
}  // }}}

bson_type CBsonIterator::type() const 
{  // {{{
  return bson_iterator_type(m_iterator);
}  // }}}

std::string CBsonIterator::key() const 
{  // {{{
  return std::string(bson_iterator_key(m_iterator));
}  // }}}

double CBsonIterator::value_as_double() const 
{  // {{{
  return bson_iterator_double(m_iterator);
}  // }}}

int CBsonIterator::value_as_int() const 
{  // {{{
  return bson_iterator_int(m_iterator);
}  // }}}

int64_t CBsonIterator::value_as_long() const 
{  // {{{
  return bson_iterator_long(m_iterator);
}  // }}}

bson_timestamp_t CBsonIterator::value_as_timestamp() const 
{  // {{{
  return bson_iterator_timestamp(m_iterator);
}  // }}}

bool CBsonIterator::value_as_bool() const 
{  // {{{
  return bson_iterator_bool(m_iterator);
}  // }}}

CObjectID CBsonIterator::value_as_objectid() const 
{  // {{{
  return CObjectID(bson_iterator_oid(m_iterator));
}  // }}}

std::string CBsonIterator::value_as_string() const 
{  // {{{
  return std::string(bson_iterator_string(m_iterator));
}  // }}}

CBsonIterator CBsonIterator::get_subiterator() const 
{  // {{{
  bson_iterator* it = bson_iterator_alloc();
  bson_iterator_subiterator(m_iterator, it);
  return CBsonIterator(it);
}  // }}}


///////////////////////////////////////////////////////////////////
// CBsonObj
///////////////////////////////////////////////////////////////////
CBsonObj CBsonBuilder::m_static_empty_bson;

CBsonObj::CBsonObj()
  :m_bson(bson_alloc()), m_fields_count(0)
{  // {{{
  bson_init_empty(m_bson.get());
}  // }}}

CBsonObj::CBsonObj(const char* str_data)
  :m_bson(bson_alloc()), m_fields_count(-1)
{  // {{{
  bson_init_finished_data_with_copy(m_bson.get(), str_data);
}  // }}}

CBsonObj::CBsonObj(bson* b)
  :m_bson(b), m_fields_count(-1)
{  // {{{
}  // }}}

CBsonObj::CBsonObj(CBsonObj&& other)
  :m_bson(std::move(other.m_bson)), m_fields_count(other.m_fields_count)
{  // {{{
}  // }}}

CBsonObj::CBsonObj(const CBsonObj& other)
  :m_bson(bson_alloc()), m_fields_count(other.m_fields_count)
{  // {{{
  bson_copy(m_bson.get(), other.m_bson.get());
}  // }}}

CBsonObj::~CBsonObj()
{  // {{{
  if (m_bson)
  {
    bson* b = m_bson.release();
    bson_destroy(b);
    bson_dealloc(b);
  }
}  // }}}

const bson* CBsonObj::raw_data() const
{  // {{{
  return m_bson.get();
}  // }}}

void CBsonObj::print() const
{  // {{{
  std::cout << *this;
}  // }}}

CBsonIterator CBsonObj::get_field(const char* field) const
{  // {{{
  bson_iterator* it = bson_iterator_alloc();
  bson_find(it, m_bson.get(), field);
  return CBsonIterator(it);
}  // }}}

bool CBsonObj::is_empty() const
{  // {{{
  if (!is_valid())
    return true;
  CBsonIterator it = begin();
  return !it.more();
}  // }}}

bool CBsonObj::is_valid() const
{  // {{{
  bson* b = m_bson.get();
  return b->data != NULL && b->finished != 0;
}  // }}}

CBsonIterator CBsonObj::begin() const
{  // {{{
  bson_iterator* it = bson_iterator_alloc();
  bson_iterator_init(it, m_bson.get());
  return CBsonIterator(it);
}  // }}}

int CBsonObj::fields_count() const
{  // {{{
  if (m_fields_count >= 0)
    return m_fields_count;
  m_fields_count = 0;
  auto it = begin();
  while(it.more())
    ++m_fields_count;
  --m_fields_count;  // exclude the EOO field
  return m_fields_count;
}  // }}}

///////////////////////////////////////////////////////////////////
// CBsonBuilder
///////////////////////////////////////////////////////////////////
CBsonBuilder::CBsonBuilder()
  :m_is_built(false)
{  // {{{
  m_bson = bson_alloc();
  bson_init(m_bson);
}  // }}}

CBsonBuilder::~CBsonBuilder()
{  // {{{
  if (!m_is_built)
  {
    bson_destroy(m_bson);
    bson_dealloc(m_bson);
  }
}  // }}}

CBsonObj CBsonBuilder::obj()
{  // {{{
  bson_finish(m_bson);
  m_is_built = true;
  return CBsonObj(m_bson);
}  // }}}

const CBsonObj& CBsonBuilder::empty_bson()
{  // {{{
  return m_static_empty_bson;
}  // }}}

CBsonObj CBsonBuilder::bson_exist_all_fields(
    const std::vector<std::string>& fields)
{  // {{{
  if (fields.size() <= 0)
  {
    return CBsonObj(empty_bson());
  }
  CBsonBuilder builder;
  for (auto f: fields)
  {
    builder.append(f, CBsonBuilder().append("$exists", true).obj());
  }
  return builder.obj();
}  // }}}

int CBsonBuilder::append_oid(const std::string& name,
    const CObjectID& oid /* = CObjectID() */)
{  // {{{
  return bson_append_oid(m_bson, name.c_str(), oid.raw_oid());
}  // }}}

int CBsonBuilder::append_int(const std::string& name, int value)
{  // {{{
  return bson_append_int(m_bson, name.c_str(), value);
}  // }}}

int CBsonBuilder::append_long(const std::string& name, int64_t value)
{  // {{{
  return bson_append_long(m_bson, name.c_str(), value);
}  // }}}

int CBsonBuilder::append_double(const std::string& name, double value)
{  // {{{
  return bson_append_double(m_bson, name.c_str(), value);
}  // }}}

int CBsonBuilder::append_string(const std::string& name,
    const std::string& value)
{  // {{{
  return bson_append_string_n(m_bson, name.c_str(), value.c_str(),
      value.length());
}  // }}}

int CBsonBuilder::append_string(const std::string& name, const char* value)
{  // {{{
  return bson_append_string(m_bson, name.c_str(), value);
}  // }}}

int CBsonBuilder::append_bool(const std::string& name, bool value)
{  // {{{
  return bson_append_bool(m_bson, name.c_str(), value);
}  // }}}

int CBsonBuilder::append_binary(const std::string& name, int type,
    const char* bin_data, int len)
{  // {{{
  return bson_append_binary(m_bson, name.c_str(), type, bin_data, len);
}  // }}}

int CBsonBuilder::append_null(const std::string& name)
{  // {{{
  return bson_append_null(m_bson, name.c_str());
}  // }}}

int CBsonBuilder::append_undefined(const std::string& name)
{  // {{{
  return bson_append_undefined(m_bson, name.c_str());
}  // }}}

int CBsonBuilder::append_maxkey(const std::string& name)
{  // {{{
  return bson_append_maxkey(m_bson, name.c_str());
}  // }}}

int CBsonBuilder::append_minkey(const std::string& name)
{  // {{{
  return bson_append_minkey(m_bson, name.c_str());
}  // }}}

int CBsonBuilder::append_element(const std::string& name_or_empty,
    const CBsonIterator& it)
{  // {{{
  return bson_append_element(m_bson,
      name_or_empty.empty() ? NULL : name_or_empty.c_str(),
      it.raw_data());
}  // }}}

int CBsonBuilder::append_date(const std::string& name, int64_t milliseconds)
{  // {{{
  return bson_append_date(m_bson, name.c_str(), milliseconds);
}  // }}}

int CBsonBuilder::append_subobject(const std::string& name,
    const CBsonObj& subobject)
{  // {{{
  return bson_append_bson(m_bson, name.c_str(), subobject.m_bson.get());
}  // }}}

int CBsonBuilder::append_array(const std::string& name,
    const CBsonObj& items)
{  // {{{
  int ret = bson_append_start_array(m_bson, name.c_str());
  if (ret != BSON_OK) return ret;
  bson_iterator* it = bson_iterator_alloc();
  bson_iterator_init(it, items.raw_data());
  int index = 0;
  while (bson_iterator_more(it))
  {
    if (bson_iterator_next(it) != BSON_EOO)
    {
      ret = bson_append_element(m_bson, std::to_string(index).c_str(), it);
      if (ret != BSON_OK) return ret;
    }
  }
  bson_iterator_dealloc(it);
  return bson_append_finish_array(m_bson);
}  // }}}

CBsonBuilder& CBsonBuilder::append(const std::string& name, int value)
{  // {{{
  bson_append_int(m_bson, name.c_str(), value);
  return *this;
}  // }}}

CBsonBuilder& CBsonBuilder::append(const std::string& name, int64_t value)
{  // {{{
  bson_append_long(m_bson, name.c_str(), value);
  return *this;
}  // }}}

CBsonBuilder& CBsonBuilder::append(const std::string& name, double value)
{  // {{{
  bson_append_double(m_bson, name.c_str(), value);
  return *this;
}  // }}}

CBsonBuilder& CBsonBuilder::append(const std::string& name, const std::string& value)
{  // {{{
  bson_append_string_n(m_bson, name.c_str(), value.c_str(), value.length());
  return *this;
}  // }}}

CBsonBuilder& CBsonBuilder::append(const std::string& name, const char* value)
{  // {{{
  bson_append_string(m_bson, name.c_str(), value);
  return *this;
}  // }}}

CBsonBuilder& CBsonBuilder::append(const std::string& name, bool value)
{  // {{{
  bson_append_bool(m_bson, name.c_str(), value);
  return *this;
}  // }}}

CBsonBuilder& CBsonBuilder::append(const std::string& name,
    const CBsonObj& subobject, bool is_array /* = false */)
{  // {{{
  if (is_array)
  {
    append_array(name, subobject);
  }
  else
  {
    bson_append_bson(m_bson, name.c_str(), subobject.m_bson.get());
  }
  return *this;
}  // }}}

///////////////////////////////////////////////////////////////////
// General
///////////////////////////////////////////////////////////////////
std::ostream& operator<< (std::ostream& os, const CBsonIterator& it)
{  // {{{
  return print(os, it, true, 0, true, 0);
}  // }}}

std::ostream& operator<< (std::ostream& os, const CBsonObj& obj)
{  // {{{
  if (!obj.is_valid())
    return os << "Invalid Bson Object" << std::endl;
  if (obj.is_empty())
  {
    return os << "{}" << std::endl;
  }
  int index = 0;
  CBsonIterator it = obj.begin();
  while (it.more())
  {
    it.next();
    print(os, it, false, 0, true, index++);
  }
  return os;
}  // }}}


// vim: fdm=marker

