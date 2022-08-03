#pragma once

#include <string>
#include <vector>
#include <QString>
#include <QVector>
#include <QString>

namespace Event {
static const constexpr char* TASK_SCHEDULED = "Task Scheduled";
static const constexpr char* MESSAGE_RECEIVED = "Message Received";
}  // ns Event

namespace Platform {
static const uint8_t NAME_INDEX    = 0x00;
static const uint8_t UUID_INDEX    = 0x01;
static const uint8_t USER_INDEX    = 0x02;
static const uint8_t TIME_INDEX    = 0x03;
static const uint8_t CONTENT_INDEX = 0x04;
static const uint8_t URLS_INDEX    = 0x05;
static const uint8_t REPOST_INDEX  = 0x06;
static const uint8_t METHOD_INDEX  = 0x07;
static const uint8_t ARGS_INDEX    = 0x08;
static const uint8_t CMD_INDEX     = 0x09;
static const uint8_t STATUS_INDEX  = 0x0A;
static const uint8_t ARGSIZE       = 0x0B;

enum class PostStatus
{
  pending = 0x00,
  success = 0x01,
  failure = 0x02
};

static PostStatus from_string(const QString& s)
{
  if (s == "0")
    return PostStatus::pending;
  if (s == "1")
    return PostStatus::success;
  return PostStatus::failure;
}

struct Post
{
  QString name;
  QString uuid;
  QString user;
  QString time;
  QString content;
  QString urls;
  QString repost;
  QString method;
  QString args;
  QString cmd;
  QString status;

  bool    active{false};
  QVector<QString> payload() const
  {
    return QVector<QString>{
      name, uuid, user, time, content, urls, repost, method, args, cmd, status
    };
  }
};

} // ns Platform
