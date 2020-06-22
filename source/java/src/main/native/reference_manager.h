#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

namespace neuropod
{
namespace jni
{

template <typename T>
using PointerMap = std::shared_ptr<std::unordered_map<int64_t, std::vector<std::shared_ptr<T>>>>;

template <typename T>
class ReferenceManager
{
// A global reference manager, which saves smart pointers in an unordered map
// and reset the pointer when necessary. This way we can avoid double delete.
private:
    // instantiated as nullptr to save memory for unused types
    static PointerMap<T> holder_;

public:
    // Insert a shared pointer to the manager
    static void put(std::shared_ptr<T> pointer);
    // Remove a shared pointer from the manager
    static void remove(std::shared_ptr<T> pointer);
    // Remove any shared pointer with a certain reference from the manager
    static void remove(int64_t handle);
    // Check the input handle exist in the manager or not, if not throw a exception
    static void check(int64_t handle);
    // Check the input handle exist in the manager or not, if not return false, otherwise return true
    static bool contains(int64_t handle);
};

template <typename T>
PointerMap<T> ReferenceManager<T>::holder_ = nullptr;


} // namespace jni
} // namespace neuropod

#include "reference_manager_impl.h"