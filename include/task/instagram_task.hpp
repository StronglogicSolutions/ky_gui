#ifndef __INSTAGRAM_TASK_HPP__
#define __INSTAGRAM_TASK_HPP__

#include <include/task/task.hpp>
#include <type_traits>

namespace Scheduler {
namespace Args {
const QString HEADER_TYPE = "header";
const QString DESCRIPTION_TYPE = "description";
const QString HASHTAG_TYPE = "hashtag";
const QString PROMOTE_TYPE = "promote/share";
const QString LINK_BIO_TYPE = "link/bio";
const QString REQUESTED_BY_TYPE = "requested by";
}  // namespace Args
}  // namespace Scheduler
class InstagramTask : public Scheduler::Task {
 public:
  InstagramTask(Scheduler::KFileData);
  InstagramTask(QVector<Scheduler::KFileData>);
  virtual void defineTaskArguments() override;
  virtual const Scheduler::TaskArguments getTaskArguments() override;
  virtual Scheduler::TypeVariant getTaskArgument(QString name) override;
  virtual Scheduler::ArgumentValues getArgumentValues() override;
  virtual const QVector<Scheduler::KFileData> getFiles() override;
  virtual Scheduler::TaskType getType() override;
  virtual void setArgument(QString name, Scheduler::TypeVariant arg) override;
  virtual bool isReady() override;
  virtual void clear() override;
  virtual void setDefaultValues() override;
  virtual ~InstagramTask() override;

 private:
  Scheduler::TaskArguments m_arguments;
  QVector<Scheduler::KFileData> files;
};

#endif  // __INSTAGRAM_TASK_HPP
