#ifndef MODEL_NANO_OBSERVER_HPP
#define MODEL_NANO_OBSERVER_HPP

#include "nano_function.hpp"

namespace Nano {

class Observer
{

  template <typename T>
  friend class Signal;

  struct DelegateKeyObserver
  {
    DelegateKey delegate;
    Observer* observer;
  };
  struct SSNode
  {
    DelegateKeyObserver data;
    SSNode* next;
  }* head = nullptr;

  //-----------------------------------------------------------PRIVATE METHODS

  void insert(DelegateKey const& key, Observer* obs) {
    head = new SSNode{{key, obs}, head};
  }

  // cppcheck-suppress constParameterPointer
  void remove(DelegateKey const& key, Observer* obs) {
    SSNode* node = head;
    SSNode* prev = nullptr;
    // Only delete the first occurrence
    // cppcheck-suppress nullPointerRedundantCheck
    for (; node; prev = node, node = node->next) {
      if (node->data.delegate == key && node->data.observer == obs) {
        if (prev) {
          prev->next = node->next;
        } else {
          head = head->next;
        }
        delete node;
        break;
      }
    }
  }

  void removeAll() {
    for (auto node = head; node;) {
      auto temp = node;
      // If this is us we only need to delete
      if (this != node->data.observer) {
        // Remove this slot from this listening Observer
        node->data.observer->remove(node->data.delegate, this);
      }
      node = node->next;
      delete temp;
    }
    head = nullptr;
  }

  bool ss_isEmpty() const {
    return head == nullptr;
  }

  template <typename Delegate, typename... Uref>
  void onEach(Uref&&... args) {
    for (auto node = head, next = head; node; node = next) {
      next = node->next;
      // Perfect forward and emit
      Delegate(node->data.delegate)(std::forward<Uref>(args)...);
    }
  }

  template <typename Delegate, typename Accumulate, typename... Uref>
  void onEach_Accumulate(Accumulate&& accumulate, Uref&&... args) {
    for (auto node = head, next = head; node; node = next) {
      next = node->next;
      // Perfect forward, emit, and accumulate the return value
      accumulate(Delegate(node->data.delegate)(std::forward<Uref>(args)...));
    }
  }

  //-----------------------------------------------------------------PROTECTED

 protected:
  ~Observer() {
    removeAll();
  }

  //--------------------------------------------------------------------PUBLIC

 public:
  Observer() = default;
  Observer(const Observer& other) = delete;       // non construction-copyable
  Observer& operator=(const Observer&) = delete;  // non copyable
};

}  // namespace Nano

#endif  // MODEL_NANO_OBSERVER_HPP
