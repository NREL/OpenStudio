#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include "nano_function.hpp"

namespace Nano
{

class Observer
{

    template <typename T> friend class Signal;

    struct DelegateKeyObserver { DelegateKey delegate; Observer* observer; };
    struct Node { DelegateKeyObserver data; Node* next; } *head = nullptr;

    //-----------------------------------------------------------PRIVATE METHODS

    void insert(DelegateKey const& key, Observer* ptr)
    {
        head = new Node { { key, ptr }, head };
    }

    void insert(DelegateKey const& key)
    {
        this->insert(key, this);
    }

    void remove(DelegateKey const& key)
    {
        Node* node = head;
        Node* prev = nullptr;
        // Only delete the first occurrence
        for ( ; node; prev = node, node = node->next)
        {
            if (node->data.delegate == key)
            {
                if (prev)
                {
                    prev->next = node->next;
                }
                else
                {
                    head = head->next;
                }
                delete node;
                break;
            }
        }
    }

    void removeAll()
    {
        for (auto node = head; node;)
        {
            auto temp = node;
            // If this is us we only need to delete
            if (this != node->data.observer)
            {
                // Remove this slot from this listening Observer
                node->data.observer->remove(node->data.delegate);
            }
            node = node->next;
            delete temp;
        }
        head = nullptr;
    }

    bool isEmpty() const
    {
        return head == nullptr;
    }

    template <typename Delegate, typename... Uref>
    void onEach(Uref&&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;
            // Perfect forward and emit
            Delegate(node->data.delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void onEach_Accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        for (auto node = head, next = head; node; node = next)
        {
            next = node->next;
            // Perfect forward, emit, and accumulate the return value
            accumulate(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
        }
    }

    //-----------------------------------------------------------------PROTECTED

    protected:

    ~Observer()
    {
        removeAll();
    }

    //--------------------------------------------------------------------PUBLIC

    public:

    Observer() = default;
    Observer(const Observer& other) = delete; // non construction-copyable
    Observer& operator=(const Observer&) = delete; // non copyable

};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_OBSERVER_HPP
