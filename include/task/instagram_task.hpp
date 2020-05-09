#ifndef __INSTAGRAM_TASK_HPP__
#define __INSTAGRAM_TASK_HPP__

#include <include/task/task.hpp>
#include <type_traits>

class InstagramTask : public Scheduler::Task {
 public:
  virtual bool isReady() override;
  virtual void clear() override;
  virtual const Scheduler::TaskArguments getTaskArguments() override;
  virtual void defineTaskArguments() override;
  virtual ~InstagramTask() override;
  template <typename T>
  void setArgument(QString name, T arg);

 private:
  Scheduler::TaskArguments m_arguments;
};

#endif  // __INSTAGRAM_TASK_HPP
