/*
 * =============================================================================
 *       Filename:  main.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  2013-07-02 22:07:24
 *         Author:  newk (newk), newkedison@gmail.com
 *        Company:  
 *          Copyright (c) 2013, newk
 * =============================================================================
 */

//#include "bsonobj.h"
#include "../client.h"
#include <iostream>

//void test_bson()
//{
//  CBsonObj o;
//  std::cout << o.is_empty() << std::endl;
//  CBsonBuilder builder;
//  builder.append("int_value", 10);
//  builder.append("double_value", 11);
//  CBsonObj b1 = builder.obj();
//  std::cout << b1.is_empty() << std::endl;
//  CBsonIterator it = b1["int_value"];
//  std::cout << it.value_as_int() << std::endl;
//  std::cout << b1["double_value"].value_as_double() << std::endl;
//  std::cout << b1;
//  std::cout << CBsonBuilder::empty_bson().is_empty() << std::endl;
//  std::cout << CBsonBuilder::empty_bson();
//  std::cout << CBsonBuilder().append("a", 1).append("b", 2.3).append("c", "c")
//    .append("d", true).append("e", CBsonBuilder().append("e1", 5).obj()).obj();
//  std::cout << CBsonBuilder().append("sub1",
//      CBsonBuilder().append("sub11",
//        CBsonBuilder().append("item", 10).obj()).obj()).obj();
//}

//void test_client_insert()
//{
//  CMongoClient client;
//  client.connect("localhost", 27017);
//  client.insert("test1.abc", CBsonBuilder().append("a", 1).append("b", 2)
//      .append("c", 3).obj());
//  client.insert("test1.abc",
//      CBsonBuilder().append("a",
//        CBsonBuilder().append("aa",
//          CBsonBuilder().append("aaa", "hello").obj())
//        .obj())
//      .obj());
//}

//void test_client_update()
//{
//  CMongoClient c;
//  c.connect("127.0.0.1", 27017);
//  c.update("test1.abc", CBsonBuilder().append("a", 1).obj(),
//      CBsonBuilder().append("$set",
//        CBsonBuilder().append("d", 4).obj()).obj(), 0);
//  c.upsert("test1.abc", CBsonBuilder().append("a", 2).obj(),
//      CBsonBuilder().append("$inc",
//        CBsonBuilder().append("a", 5).obj())
//      .append("$set", 
//        CBsonBuilder().append("b", 10).obj()).obj());
//}

//void test_client_remove()
//{
//  CMongoClient c;
//  c.connect("localhost", 27017);
//  c.remove("test1.abc", CBsonBuilder().append("a", 
//        CBsonBuilder().append("$exists", true).obj()).obj());
//}

void test_client_find()
{
  CMongoClient c;
  c.connect("localhost", 27017);
  CBsonObj b(c.find_one("test1.abc", CBsonBuilder::empty_bson()));
  std::cout << b;

  CMongoCursor cur(c.find("test1.abc", CBsonBuilder::empty_bson()));
  std::cout << cur.raw_data()->seen << std::endl;
  while (cur.next())
  {
    std::cout << cur.get_object();
  }

  CMongoCursor cur2(c.find("test1.abc", CBsonBuilder::bson_exist_all_fields(
          {"d"})));
  while (cur2.next())
  {
    std::cout << cur2.get_object();
  }
}

void test_client()
{
//  test_client_insert();
//  test_client_update();
//  test_client_remove();
  test_client_find();
}

int main()
{
  test_client();
  return 0;
}

