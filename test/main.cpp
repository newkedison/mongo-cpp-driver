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

#include "../client.h"
#include <iostream>

void test_bson_basic()
{
  CBsonObj o;
  std::cout << o.is_empty() << std::endl;
  CBsonBuilder builder;
  builder.append("int_value", 10);
  builder.append("double_value", 11);
  CBsonObj b1 = builder.obj();
  std::cout << b1.is_empty() << std::endl;
  CBsonIterator it = b1["int_value"];
  std::cout << it.value_as_int() << std::endl;
  std::cout << b1["double_value"].value_as_double() << std::endl;
  std::cout << b1;
  std::cout << CBsonBuilder::empty_bson().is_empty() << std::endl;
  std::cout << CBsonBuilder::empty_bson();
  std::cout << CBsonBuilder().append("a", 1).append("b", 2.3).append("c", "c")
    .append("d", true).append("e", CBsonBuilder().append("e1", 5).obj()).obj();
  std::cout << CBsonBuilder().append("sub1",
      CBsonBuilder().append("sub11",
        CBsonBuilder().append("item", 10).obj()).obj()).obj();
  CBsonBuilder()
    .append("arr", CBsonBuilder().append("a", "a").append("b", "b").obj(), true)
    .obj().print();
  CBsonBuilder().append("arr", std::make_tuple(5, "str", 1.23, 'a'))
    .obj().print();
  CBsonBuilder().append("arr", std::make_tuple(std::make_tuple(1, 2), 3))
    .obj().print();
}

void test_bson_macro()
{
  BSON(("a", 1) ("b", BSON("c", 5)) ("d", BSON_ARRAY("str", 1, 1.2)))
    .print();
}

void test_client_insert()
{
  CMongoClient client;
  client.connect("localhost", 27017);
  client.insert("test1.abc", CBsonBuilder().append("a", 1).append("b", 2)
      .append("c", 3).obj());
  client.insert("test1.abc",
      CBsonBuilder().append("a",
        CBsonBuilder().append("aa",
          CBsonBuilder().append("aaa", "hello").obj())
        .obj())
      .obj());
  client.insert("test1.abc", CBsonBuilder().append("a", "test array")
    .append("arr", CBsonBuilder().append("a", "a").append("b", "b").obj(), true)
    .obj());
  client.insert("test1.abc", CBsonBuilder().append("a", "nested array")
      .append("arr", CBsonBuilder()
        .append("subarr", CBsonBuilder()
          .append("subobj", CBsonBuilder()
            .append("a", 1).append("b", 2).obj())
          .append("subobj2", CBsonBuilder()
            .append("c", 3).append("d", 4).obj())
          .obj(), true)
        .append("subarr2", CBsonBuilder()
          .append("aa", "aa").append("bb", 1.23).obj(), true)
        .obj(), true).obj());
  client.insert("test1.abc", CBsonBuilder().append("a", "nested array2")
      .append("arr", std::make_tuple(
          std::make_tuple(
            CBsonBuilder().append("a", 1).append("b", 2).obj(),
            CBsonBuilder().append("c", 3).append("d", 4).obj()),
          std::make_tuple("aa", 1.23))).obj());
  client.insert("test1.abc", BSON("a", "nested array3",
      "arr", BSON_ARRAY(
          BSON_ARRAY(BSON("a", 1, "b", 2), BSON("c", 3, "d", 4)),
          BSON_ARRAY("aa", 1.23)
      )));
}

void test_client_update()
{
  CMongoClient c;
  c.connect("127.0.0.1", 27017);
  c.update("test1.abc", CBsonBuilder().append("a", 1).obj(),
      CBsonBuilder().append("$set",
        CBsonBuilder().append("d", 4).obj()).obj(), 0);
  c.upsert("test1.abc", CBsonBuilder().append("a", 2).obj(),
      CBsonBuilder().append("$inc",
        CBsonBuilder().append("a", 5).obj())
      .append("$set", 
        CBsonBuilder().append("b", 10).obj()).obj());
}

void test_client_remove()
{
  CMongoClient c;
  c.connect("localhost", 27017);
  c.remove("test1.abc", CBsonBuilder().append("a", 
        CBsonBuilder().append("$exists", true).obj()).obj());
}

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

void test_client_sort()
{
  CMongoClient c;
  c.connect("localhost", 27017);
  c.insert("test1.abc", BSON(("a", 1)("b", 5)("order", true)));
  c.insert("test1.abc", BSON(("a", 2)("b", 1)("order", true)));
  c.insert("test1.abc", BSON(("a", 3)("b", 0)("order", true)));
  c.insert("test1.abc", BSON(("a", 4)("b", 2)("order", true)));
  c.insert("test1.abc", BSON(("a", 5)("b", 7)("order", true)));
  c.insert("test1.abc", BSON(("a", 6)("b", 3)("order", true)));
  std::cout << "-----------------Unsort:-----------------" << std::endl;
  CMongoCursor cur1(c.find("test1.abc", BSON("order", true)));
  while (cur1.next())
    std::cout << cur1.get_object();
  std::cout << "-----------------Sort 1:-----------------" << std::endl;
  CMongoCursor cur2(c.find("test1.abc", BSON("order", true),
        BSON("b", 1)));
  while (cur2.next())
    std::cout << cur2.get_object();
  std::cout << "-----------------Sort 2:-----------------" << std::endl;
  CMongoCursor cur3(c.find("test1.abc", BSON("order", true),
        BSON("b", -1)));
  while (cur3.next())
    std::cout << cur3.get_object();
}

void test_client_regex()
{
  CMongoClient c;
  c.connect("localhost", 27017);
  c.insert("test1.abc", BSON("a", "abaabaaab"));
  c.insert("test1.abc", BSON("a", "aaaaaaab"));
  c.insert("test1.abc", BSON("a", "ababababababa"));
  CBsonBuilder bb;
  bb.append_regex("a", "Aab", REGEX_FLAG_CASE_INSENSITIVE);
  CMongoCursor cur(c.find("test1.abc", bb.obj()));
  while (cur.next())
    std::cout << cur.get_object();
}

void test_client_code()
{
  CMongoClient c;
  c.connect("localhost", 27017);
  c.insert("test1.abc", BSON("a", 1, "b", 2));
  c.insert("test1.abc", BSON("a", 2, "b", 1));
  CBsonBuilder b;
  b.append_code("$where",
      "function() { return this.b > this.a && this.a % 2 == 1; }");
  CMongoCursor cur(c.find("test1.abc", b.obj()));
  while (cur.next())
    std::cout << cur.get_object();
}

void test_bson()
{
  std::cout << "###### test bson basic" << std::endl;
  test_bson_basic();
  std::cout << "###### test bson macro" << std::endl;
  test_bson_macro();
}

void test_client()
{
  std::cout << "###### test client remove" << std::endl;
  test_client_remove();
  std::cout << "###### test client insert" << std::endl;
  test_client_insert();
  std::cout << "###### test client update" << std::endl;
  test_client_update();
  std::cout << "###### test client find" << std::endl;
  test_client_find();
  std::cout << "###### test client sort" << std::endl;
  test_client_sort();
  std::cout << "###### test client regex" << std::endl;
  test_client_regex();
  std::cout << "###### test client code" << std::endl;
  test_client_code();
}

int main()
{
  std::cout << "#### test bson" << std::endl;
  test_bson();
  std::cout << "#### test client" << std::endl;
  test_client();
  return 0;
}

