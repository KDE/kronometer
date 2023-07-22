/*
    SPDX-FileCopyrightText: 2022 Johnny Jazeix <jazeix@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "lapitemdelegate.h"
#include "mainwindow.h"

#include <QKeySequence>
#include <QList>

LapItemDelegate::LapItemDelegate(MainWindow *window) : QItemDelegate(window),
                                                       m_window{window}
{
    connect(this, &QItemDelegate::closeEditor, this, &LapItemDelegate::onCloseEditor);
}

QWidget* LapItemDelegate::createEditor(QWidget *parent, 
                                       const QStyleOptionViewItem &option, 
                                       const QModelIndex &index) const
{
    m_window->enableLapShortcuts(false);
    return QItemDelegate::createEditor(parent, option, index);
}
void LapItemDelegate::onCloseEditor(QWidget *,
                                    QAbstractItemDelegate::EndEditHint) const
{
    m_window->enableLapShortcuts(true);
}

#include "moc_lapitemdelegate.cpp"
