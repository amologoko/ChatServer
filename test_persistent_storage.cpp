#include <gtest/gtest.h>
#include "PersistentStore.hpp"

TEST(PersistentStore, put) {
  PersistentStore ps("storage.json");
  ps.put("sasha", "test123");
  ps.save();
}

TEST(PersistentStore, get) {
  PersistentStore ps("storage.json");
  ASSERT_EQ(ps.get("sasha"), "test123");
}
