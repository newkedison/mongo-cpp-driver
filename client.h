/*
 * =============================================================================
 *       Filename:  client.h
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-07-05 20:16:55
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

#ifndef CLIENT_H
#define CLIENT_H

#include "c_driver/src/mongo.h"
#include "bsonobj.h"
#include <string>

class CMongoClient;

class CMongoCursor
{
  public:
    CMongoCursor(CMongoClient& client, const std::string& ns);
    CMongoCursor(mongo_cursor* cursor);

    CBsonObj get_object() const;
    bool next();

    void set_query(const CBsonObj& query);
    void set_fields(const CBsonObj& fields);
    void set_limit(int limit);
    void set_skip(int skip);
    void set_options(int options);

    mongo_cursor* raw_data() const
    {
      return m_cursor;
    }

  private:
    mongo_cursor* m_cursor;
    std::unique_ptr<CBsonObj> m_query;
    std::unique_ptr<CBsonObj> m_fields;
};

class CMongoClient
{
  friend class CMongoCursor;
  public:
    CMongoClient();
    ~CMongoClient();
    mongo_error_t last_error() const;
    int connect(const std::string& host, int port);
    bool has_namespace(const std::string& ns) const;
    int set_op_timeout(int milliseconds);
    int check_connection();
    int reconnect();
    void disconnect();

    // CRUD
    int insert(const std::string& ns, const CBsonObj& data);
    int insert(const std::string& ns, std::vector<const CBsonObj*> datas,
        int flags = MONGO_CONTINUE_ON_ERROR);
    int update(const std::string& ns, const CBsonObj& query,
        const CBsonObj& op, int flags);
    int upsert(const std::string& ns, const CBsonObj& query,
        const CBsonObj& op);
    int remove(const std::string& ns, const CBsonObj& query);

    // Cursor
    CMongoCursor find(const std::string& ns, const CBsonObj& query,
        const std::vector<std::string> fields = std::vector<std::string>(),
        int limit = 0, int skip = 0, int options = 0);
    CBsonObj find_one(const std::string& ns, const CBsonObj& query,
        const std::vector<std::string> fields = std::vector<std::string>());

  private:
    mongo* m_mongo;
};

#endif  // CLIENT_H

