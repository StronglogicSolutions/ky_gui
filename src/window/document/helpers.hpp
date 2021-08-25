#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTableWidget>

enum class RowType
{
  HEADER = 0x00,
  REPEAT = 0x01
};

/**
 * @brief GetBrushForType
 * @param type
 * @return
 */
static QBrush GetBrushForType(const RowType& type)
{
  return (type == RowType::REPEAT) ?
    QBrush{QColor{"grey"}} :
    QBrush{QColor{"red"}};
}

/**
 * @brief IsImage
 * @param s
 * @return
 */
static bool IsImage(const QString& s)
{
  return (s == "$FILE_TYPE");
}

/**
 * @brief CloseTable
 * @param doc_ptr
 */
static void CloseTable(QTextDocument* doc_ptr)
{
  QTextCursor cursor{doc_ptr};
  cursor.movePosition(QTextCursor::End);
  cursor.insertHtml("</tbody></table>");
}

static bool FindInTable(QTableWidget* t, const QString& s)
{
  auto rows = t->rowCount();
  auto cols = t->columnCount();

  for (int32_t i = 0; i < rows; i++)
    for (int32_t j = 0; j < cols; j++)
      if (t->item(i, j)->text() == s)
        return true;
  return false;
}
