#ifndef __INSTAGRAM_TASK_HPP__
#define __INSTAGRAM_TASK_HPP__

#include <include/task/task.hpp>
#include <type_traits>

namespace Scheduler {
namespace Args {
const QString HEADER_TYPE = "header";
const QString FILE_TYPE = "files";
const QString DESCRIPTION_TYPE = "description";
const QString HASHTAG_TYPE = "hashtags";
const QString PROMOTE_TYPE = "promote_share";
const QString LINK_BIO_TYPE = "link_in_bio";
const QString REQUESTED_BY_TYPE = "requested_by";
const QString REQUESTED_BY_PHRASE = "requested_by_phrase";
}  // namespace Args

static constexpr const char* INSTAGRAM_NAME = "Instagram";

namespace TaskCode {
  static constexpr int IGTASKBYTE = 0xFF;
}
}  // namespace Scheduler

/**
 * @brief The InstagramTask class
 *
 * Class to organize and transport data necessary to perform an Instagram Task.
 */
class InstagramTask : public Scheduler::Task {
 public:
  /**
   * @constructor
   */
  InstagramTask();

  /**
   * Overrides @abstract Task::defineTaskArguments
   *
   * Useful for avoiding repetitive input of data.
   */
  virtual void defineTaskArguments() override;

  /**
   * Overrides @abstract Task::getTaskArguments
   *
   * Use this method to take over ownership of the task's arguments.
   */
  virtual const Scheduler::TaskArguments&& getTaskArguments() override;

  /**
   * Overrides @abstract Task::getTaskArgument
   *
   * Easy access to an argument.
   */
  virtual Scheduler::TaskArgument&& getTaskArgument(QString name) override;

  /**
   * Overrides @abstract Task::getTaskArgumentValue
   *
   * Easy access to an argument's value.
   */
  virtual const Scheduler::TypeVariant getTaskArgumentValue(QString name) override;

  /**
   * Overrides @abstract Task::getArgumentValues
   *
   * Easy access to all of the arguments that can be represented as a string.
   */
  virtual Scheduler::ArgumentValues getArgumentValues() override;

  /**
   * Overrides @abstract Task::getArgumentNames
   *
   * Provides the names of the arguments.
   */
  virtual QVector<QString> getArgumentNames() override;

  /**
   * Overrides @abstract Task::getFiles
   *
   * Easy access to the task's files.
   */
  virtual const QVector<Scheduler::KFileData> getFiles() override;

  /**
   * Overrides @abstract Task::getType
   *
   * Informs the caller of the task's type.
   */
  virtual Scheduler::TaskType getType() override;

  /**
   * Overrides @abstract Task::getTaskCode
   *
   * Returns the task's byte code.
   */
  virtual uint32_t getTaskCode() override;

  /**
   * Overrides @abstract Task::setArgument
   *
   * Set the value of an existing argument.
   */
  virtual void setArgument(QString name, Scheduler::TypeVariant arg) override;

  /**
   * Overrides @abstract Task::addArgument
   *
   * Add an additional value to a file container argument.
   */
  virtual void addArgument(QString name, Scheduler::KFileData file) override;

  /**
   * Overrides @abstract Task::addArgument
   *
   * Add an additional value to a string container argument.
   */
  virtual void addArgument(QString name, QString string) override;

  /**
   * Overrides @abstract Task::removeArgument
   *
   * Remove a value from a container argument.
   */
  virtual void removeArgument(QString name, Scheduler::TypeVariant arg) override;

  /**
   * Overrides @abstract Task::hasFiles
   *
   * Tells the caller if the task has files.
   */
  virtual bool hasFiles() override;

  /**
   * Overrides @abstract Task::isReady
   *
   * Informs the caller if the prerequisite arguments have values.
   */
  virtual bool isReady() override;

  /**
   * Overrides @abstract Task::clear
   *
   * Clears the values of all arguments, without removing the arguments.
   */
  virtual void clear() override;

  /**
   * Overrides @abstract Task::setDefaultValues
   *
   * Set default values for the arguments.
   */
  virtual void setDefaultValues() override;

  /**
   * Overrides @abstract Task::~Task
   * @destructor
   */
  virtual ~InstagramTask() override;

 private:
  /**
   * @brief m_arguments
   *
   * A container storing the task's arguments
   */
  Scheduler::TaskArguments m_arguments;
};

#endif  // __INSTAGRAM_TASK_HPP
