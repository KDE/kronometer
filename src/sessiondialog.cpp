/*
    Copyright (C) 2015 by Elvis Angelaccio <elvis.angelaccio@kdemail.net>

    This file is part of Kronometer.

    Kronometer is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Kronometer is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Kronometer.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "sessiondialog.h"
#include "sessionmodel.h"

#include <KLocalizedString>
#include <KMessageBox>

#include <QDialogButtonBox>
#include <QHeaderView>
#include <QKeyEvent>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QVBoxLayout>

SessionDialog::SessionDialog(QWidget *parent, const QString& title) : QDialog(parent, Qt::Dialog)
{
    m_sessionModel = new SessionModel(this);

    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_sessionModel);

    m_sessionView = new QTableView(this);
    m_sessionView->setModel(m_proxyModel);
    m_sessionView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_sessionView->setGridStyle(Qt::DotLine);
    m_sessionView->verticalHeader()->hide();
    m_sessionView->resizeColumnsToContents();
    m_sessionView->horizontalHeader()->setStretchLastSection(true);
    m_sessionView->setSortingEnabled(true);
    m_sessionView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    // TODO: the user may want to select/remove more than one session
    m_sessionView->setSelectionMode(QAbstractItemView::SingleSelection);

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    vlayout->addWidget(m_sessionView);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    vlayout->addWidget(buttonBox);

    connect(m_sessionView, &QTableView::doubleClicked, this, &SessionDialog::slotDoubleClicked);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SessionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    setLayout(vlayout);
    setWindowTitle(title);
}

Session SessionDialog::selectedSession() const
{
    return m_selectedSession;
}

void SessionDialog::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        this->accept();
        break;
    case Qt::Key_Delete:
        removeDialog();
        break;
    default:
        break;
    }
}

void SessionDialog::accept()
{
    m_selectedSession = m_sessionModel->at(selectedIndex().row());
    QDialog::accept();
}

void SessionDialog::slotDoubleClicked(const QModelIndex& index)
{
    if (m_sessionModel->isEditable(index))
        return;

    this->accept();
}

QModelIndex SessionDialog::selectedIndex()
{
    QModelIndex sortedIndex = m_sessionView->selectionModel()->currentIndex();

    return m_proxyModel->mapToSource(sortedIndex);
}

void SessionDialog::removeDialog()
{
    KMessageBox::ButtonCode buttonCode = KMessageBox::warningContinueCancel(
                                        this,
                                        i18n("Do you want to remove the selected session?"),
                                        i18n("Confirm deletion"),
                                        KStandardGuiItem::cont(),
                                        KStandardGuiItem::cancel(),
                                        "delete-session");

    if (buttonCode != KMessageBox::Continue)
        return;

    m_sessionModel->removeRow(selectedIndex().row());
}

