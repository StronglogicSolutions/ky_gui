#ifndef __STATUS_DELEGATE_HPP__
#define __STATUS_DELEGATE_HPP__

#include <QStyledItemDelegate>
#include <QEvent>
#include <functional>

using post_item_fn_t = std::function<void(const QModelIndex &)>;


using status_map_t     = std::map<QString, int>;
using status_strings_t = std::map<QString, QString>;
static const status_map_t g_status_map{
  {"Scheduled", 0},
  {"Complete", 1},
  {"Error", 2},
  {"0", 0},
  {"1", 1},
  {"2", 2}
};

static const status_strings_t g_status_strings{
  {"Scheduled", "0"},
  {"Complete", "1"},
  {"Error", "2"},
  {"0", "0"},
  {"1", "1"},
  {"2", "2"}
};

static const status_strings_t g_status_names{
  {"0", "Scheduled"},
  {"1", "Complete"},
  {"2", "Error"}
};

class StatusDelegate : public QStyledItemDelegate
{

  Q_OBJECT

public:
  explicit StatusDelegate(QObject* parent, const post_item_fn_t& fn);
  ~StatusDelegate() override = default;
  QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const final;
  void setEditorData(QWidget *editor, const QModelIndex &index) const override;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex& index) const override;

private:  
  post_item_fn_t m_callback;
};

#endif // __STATUS_DELEGATE_HPP__
