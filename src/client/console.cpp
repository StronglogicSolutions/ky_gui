#include <include/client/console.hpp>

DownloadConsole::DownloadConsole(kiqoder::FileHandler f_handler)
: handler(f_handler)
{
  Reset();
}
//-------------------------------------------------------
FileWrap*
DownloadConsole::GetFile (const QString& id)
{
  for (auto it = files.begin(); it != files.end(); it++)
    if (it->id == id)
      return it;
  return nullptr;
};
//-------------------------------------------------------
bool
DownloadConsole::WaitingForFile(const QString& id)
{
  const FileWrap* file = GetFile(id);
  return (file != nullptr && file->buffer == nullptr);
}
//-------------------------------------------------------
bool
DownloadConsole::is_downloading() const
{
  return wt_count;
}
//-------------------------------------------------------
void
DownloadConsole::Write(const QString& id, uint8_t* data, const size_t size)
{
  FileWrap* file = GetFile(id);
  if (file != nullptr && file->HasID())
    file->buffer = QByteArray(reinterpret_cast<char*>(data), size);
  else
    KLOG("Failed to write incoming data");
}
//-------------------------------------------------------
void
DownloadConsole::Receive(uint8_t* data, const size_t size)
{
  handler.processPacket(data, size);
}
//-------------------------------------------------------
DownloadConsole::Files&&
DownloadConsole::GetData()
{
  return std::move(files);
}
//-------------------------------------------------------
void
DownloadConsole::Wait(const bool wait)
{
  wt_for_metadata = wait;
}
//-------------------------------------------------------
bool
DownloadConsole::SetMetadata(const QVector<QString>& data)
{
  static const int32_t FILE_ID_INDEX{1};
  const  auto&         id = data[FILE_ID_INDEX];
  if (WaitingForFile(id))
  {
    Wait(false);
    handler.setID(id.toUInt());
    KLOG("Set metadata");
    return true;
  }
  KLOG("Failed to set metadata");
  return false;
}
//-------------------------------------------------------
bool
DownloadConsole::Waiting() const
{
  return wt_for_metadata;
}
//-------------------------------------------------------
void
DownloadConsole::Reset()
{
  wt_count        = 0;
  rx_count        = 0;
  wt_for_metadata = false;
  handler.reset();
}
