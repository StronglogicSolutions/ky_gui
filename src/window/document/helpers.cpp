#include <QTextCursor>
#include "helpers.hpp"

QBrush GetBrushForType(const RowType& type)
{
  return (type == RowType::REPEAT) ?
    QBrush{QColor{"grey"}} :
    QBrush{QColor{"red"}};
}

bool IsImage(const QString& s)
{
  return (s == "$FILE_TYPE");
}

void CloseTable(QTextDocument* doc_ptr)
{
  QTextCursor cursor{doc_ptr};
  cursor.movePosition(QTextCursor::End);
  cursor.insertHtml("</tbody></table>");
}

bool FindInTable(QTableWidget* t, const QString& s)
{
  auto rows = t->rowCount();
  auto cols = t->columnCount();

  for (int32_t i = 0; i < rows; i++)
    for (int32_t j = 0; j < cols; j++)
      if (t->item(i, j)->text() == s)
        return true;
  return false;
}
