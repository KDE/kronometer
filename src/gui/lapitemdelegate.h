/*
    SPDX-FileCopyrightText: 2022 Johnny Jazeix <jazeix@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef LAPITEMDELEGATE_H
#define LAPITEMDELEGATE_H

#include <QItemDelegate>

class MainWindow;
/**
 * @brief ItemDelegate to block the Return key press creating a new line
 * when editing a line.
 */
class LapItemDelegate : public QItemDelegate {
    Q_OBJECT

public:
    explicit LapItemDelegate(MainWindow *window);
    QWidget* createEditor(QWidget *parent, 
                          const QStyleOptionViewItem &option, 
                          const QModelIndex &index) const override;
    
    void onCloseEditor(QWidget *,
                       QAbstractItemDelegate::EndEditHint) const;

private:
    MainWindow *m_window;
};

#endif
