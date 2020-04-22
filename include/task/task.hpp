#ifndef __TASK_HPP__
#define __TASK_HPP__

namespace Task {

class Task {
 public:
  virtual bool isReady() = 0;
};
}  // namespace Task

#endif  // __TASK_HPP__
