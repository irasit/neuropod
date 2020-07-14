namespace neuropod
{
namespace jni
{

template <typename T>
PointerMap<T> ReferenceManager<T>::holder_ = nullptr;

template <typename T>
std::shared_timed_mutex ReferenceManager<T>::sharedMutex_;

template <typename T>
void ReferenceManager<T>::put(std::shared_ptr<T> pointer)
{
    std::unique_lock<std::shared_timed_mutex> writerLock(sharedMutex_, std::defer_lock);
    writerLock.lock();
    if (!holder_)
    {
        holder_ = std::make_shared<std::unordered_map<int64_t, std::vector<std::shared_ptr<T>>>>();
    }
    (*holder_)[reinterpret_cast<int64_t>(pointer.get())].emplace_back(std::move(pointer));
}

template <typename T>
std::shared_ptr<T> ReferenceManager<T>::get(int64_t handle)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(sharedMutex_, std::defer_lock);
    readerLock.lock();
    check(handle);
    return (*holder_)[handle].front();
}

template <typename T>
void ReferenceManager<T>::remove(std::shared_ptr<T> pointer)
{
    remove(reinterpret_cast<int64_t>(pointer.get()));
}

template <typename T>
void ReferenceManager<T>::remove(int64_t handle)
{
    std::unique_lock<std::shared_timed_mutex> writerLock(sharedMutex_, std::defer_lock);
    writerLock.lock();
    if (!contains(handle))
    {
        return;
    }
    (*holder_)[handle].back().reset();
    (*holder_)[handle].pop_back();
    if ((*holder_)[handle].empty())
    {
        (*holder_).erase(handle);
    }
}

template <typename T>
void ReferenceManager<T>::check(int64_t handle)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(sharedMutex_, std::defer_lock);
    readerLock.lock();
    if (!contains(handle))
    {
        throw std::runtime_error("Object is not allocated!");
    }
}

template <typename T>
bool ReferenceManager<T>::contains(int64_t handle)
{
    std::shared_lock<std::shared_timed_mutex> readerLock(sharedMutex_, std::defer_lock);
    readerLock.lock();
    if (!holder_)
    {
        return false;
    }
    if ((*holder_).count(handle) == 0)
    {
        return false;
    }
    if ((*holder_)[handle].empty())
    {
        return false;
    }
    return true;
}

template <typename T>
void ReferenceManager<T>::reset()
{
    std::unique_lock<std::shared_timed_mutex> writerLock(sharedMutex_, std::defer_lock);
    writerLock.lock();
    if (!holder_)
    {
        return;
    }
    for (auto &entry : (*holder_))
    {
        for (auto &pointer : entry.second)
        {
            pointer.reset();
        }
    }
    holder_.reset();
}

} // namespace jni
} // namespace neuropod
