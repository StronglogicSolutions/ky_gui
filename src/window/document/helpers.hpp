#include <QTextDocument>
#include <QTableWidget>

enum class RowType
{
  HEADER = 0x00,
  REPEAT = 0x01
};

QBrush GetBrushForType(const RowType& type);
bool   IsImage(const QString& s);
void   CloseTable(QTextDocument* doc_ptr);
bool   FindInTable(QTableWidget* t, const QString& s);
