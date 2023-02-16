#include <util/util.hpp>
#include <third_party/kiqoder/kiqoder.hpp>

struct DownloadConsole
{
using Files = QVector<FileWrap>;
Kiqoder::FileHandler handler;
int32_t              wt_count;
int32_t              rx_count;
Files                files;
bool                 wt_for_metadata;

DownloadConsole(Kiqoder::FileHandler f_handler);

FileWrap* GetFile       (const QString& id);
Files&&   GetData       ();
bool      WaitingForFile(const QString& id);
void      Write         (const QString& id, uint8_t* data, const size_t size);
void      Receive       (uint8_t* data, const size_t size);
void      Wait          (const bool wait = true);
bool      SetMetadata   (const QVector<QString>& data);
bool      is_downloading() const;
bool      Waiting       () const;
void      Reset         ();
};
