#pragma once

#include <string>
#include <vector>
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
static const uint8_t STATUS_INDEX  = 0x0A;

static const uint8_t ARGSIZE       = 0x0B;

struct Post
{
QString name;
QString uuid          ;
QString user        ;
QString time          ;
QString content     ;
QString urls          ;


};

} // ns Platform
