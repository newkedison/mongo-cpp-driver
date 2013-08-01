/*
 * =============================================================================
 *       Filename:  client.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-07-06 12:00:01
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

#include "client.h"
#include "bsonobj.h"

////////////////////////////////////////////////////////////////////
// CMongoCursor
////////////////////////////////////////////////////////////////////
CMongoCursor::CMongoCursor(CMongoClient& client, const std::string& ns)
  :m_cursor(mongo_cursor_alloc()), m_query(), m_fields()
{  // {{{
  mongo_cursor_init(m_cursor, client.m_mongo, ns.c_str());
}  // }}}

CMongoCursor::CMongoCursor(mongo_cursor* cursor)
  :m_cursor(cursor), m_query(), m_fields()
{  // {{{
}  // }}}

CMongoCursor::CMongoCursor(CMongoCursor&& other)
  :m_cursor(std::move(other.m_cursor)),
   m_query(std::move(other.m_query)),
   m_fields(std::move(other.m_fields))
{
}

CMongoCursor::~CMongoCursor()
{
  if (m_cursor)
  {
    mongo_cursor_destroy(m_cursor);
  }
}

CBsonObj CMongoCursor::get_object() const
{  // {{{
  return CBsonObj(mongo_cursor_data(m_cursor));
}  // }}}

bool CMongoCursor::next()
{  // {{{
  return mongo_cursor_next(m_cursor) == MONGO_OK;
}  // }}}

void CMongoCursor::set_query(const CBsonObj& query)
{  // {{{
  m_query.reset(new CBsonObj(query));
  mongo_cursor_set_query(m_cursor, m_query->raw_data());
}  // }}}

void CMongoCursor::set_fields(const CBsonObj& fields)
{  // {{{
  m_fields.reset(new CBsonObj(fields));
  mongo_cursor_set_fields(m_cursor, m_fields->raw_data());
}  // }}}

void CMongoCursor::set_limit(int limit)
{  // {{{
  mongo_cursor_set_limit(m_cursor, limit);
}  // }}}

void CMongoCursor::set_skip(int skip)
{  // {{{
  mongo_cursor_set_skip(m_cursor, skip);
}  // }}}

void CMongoCursor::set_options(int options)
{  // {{{
  mongo_cursor_set_options(m_cursor, options);
}  // }}}

////////////////////////////////////////////////////////////////////
// CMongoClient
////////////////////////////////////////////////////////////////////
CMongoClient::CMongoClient()
{  // {{{
  mongo_init_sockets();
  m_mongo = mongo_alloc();
  mongo_init(m_mongo);
}  // }}}

CMongoClient::~CMongoClient()
{  // {{{
  mongo_destroy(m_mongo);
  mongo_dealloc(m_mongo);
}  // }}}

mongo_error_t CMongoClient::last_error() const
{  // {{{
  return m_mongo->err;
}  // }}}

int CMongoClient::connect(const std::string& host, int port)
{  // {{{
  return mongo_client(m_mongo, host.c_str(), port);
}  // }}}

bool CMongoClient::has_namespace(const std::string& ns) const
{  // {{{
  return mongo_validate_ns(m_mongo, ns.c_str()) == MONGO_OK;
}  // }}}

int CMongoClient::set_op_timeout(int milliseconds)
{  // {{{
  return mongo_set_op_timeout(m_mongo, milliseconds);
}  // }}}

int CMongoClient::check_connection()
{  // {{{
  return mongo_check_connection(m_mongo);
}  // }}}

int CMongoClient::reconnect()
{  // {{{
  return mongo_reconnect(m_mongo);
}  // }}}

void CMongoClient::disconnect()
{  // {{{
  return mongo_disconnect(m_mongo);
}  // }}}

int CMongoClient::insert(const std::string& ns, const CBsonObj& data)
{  // {{{
  return mongo_insert(m_mongo, ns.c_str(), data.raw_data(), NULL);
}  // }}}

int CMongoClient::insert(const std::string& ns,
    std::vector<const CBsonObj*> data,
    int flags /* = MONGO_CONTINUE_ON_ERROR */)
{  // {{{
  const bson** arr = new const bson*[data.size()];
  for (size_t i = 0; i < data.size(); ++i)
  {
    arr[i] = data[i]->raw_data();
  }
  return mongo_insert_batch(m_mongo, ns.c_str(), arr, data.size(), NULL, flags);
}  // }}}

int CMongoClient::update(const std::string& ns, const CBsonObj& query,
    const CBsonObj& op, int flags)
{  // {{{
  return mongo_update(m_mongo, ns.c_str(), query.raw_data(), 
      op.raw_data(), flags, NULL);
}  // }}}

int CMongoClient::upsert(const std::string& ns, const CBsonObj& query,
    const CBsonObj& op)
{  // {{{
  return mongo_update(m_mongo, ns.c_str(), query.raw_data(), 
      op.raw_data(), MONGO_UPDATE_UPSERT, NULL);
}  // }}}

int CMongoClient::remove(const std::string& ns, const CBsonObj& query)
{  // {{{
  return mongo_remove(m_mongo, ns.c_str(), query.raw_data(), NULL);
}  // }}}

CMongoCursor CMongoClient::find(const std::string& ns, const CBsonObj& query,
    const std::vector<std::string> fields /* = std::vector<std::string>() */,
    int limit /* = 0 */, int skip /* = 0 */, int options /* = 0 */)
{  // {{{
  CBsonBuilder fields_builder;
  for (auto f: fields)
  {
    fields_builder.append(f.c_str(), 1);
  }
  return CMongoCursor(mongo_find(m_mongo, ns.c_str(), query.raw_data(),
        fields_builder.obj().raw_data(), limit, skip, options));
}  // }}}

CMongoCursor CMongoClient::find(const std::string& ns, const CBsonObj& query,
    const CBsonObj& orderby,
    const std::vector<std::string> fields /* = std::vector<std::string>() */,
    int limit /* = 0 */, int skip /* = 0 */, int options /* = 0 */)
{  // {{{
  CBsonBuilder fields_builder;
  for (auto f: fields)
  {
    fields_builder.append(f.c_str(), 1);
  }
  return CMongoCursor(mongo_find(m_mongo, ns.c_str(),
        BSON("$query", query, "$orderby", orderby).raw_data(),
        fields_builder.obj().raw_data(), limit, skip, options));
}  // }}}

CBsonObj CMongoClient::find_one(const std::string& ns, const CBsonObj& query,
    const std::vector<std::string> fields /* = std::vector<std::string>() */)
{  // {{{
  CBsonBuilder fields_builder;
  for (auto f: fields)
  {
    fields_builder.append(f.c_str(), 1);
  }
  bson* result = bson_alloc();
  bson_init_empty(result);
  mongo_find_one(m_mongo, ns.c_str(), query.raw_data(),
        fields_builder.obj().raw_data(), result);
  return CBsonObj(result);
}  // }}}

// vim: fdm=marker

