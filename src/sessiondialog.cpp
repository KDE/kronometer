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
#include <KMessageWidget>

#include <QDialogButtonBox>
#include <QHeaderView>
#include <QKeyEvent>
#include <QPushButton>
#include <QSortFilterProxyModel>
#include <QTableView>
#include <QVBoxLayout>

SessionDialog::SessionDialog(QWidget *parent, const QString& title) : QDialog(parent, Qt::Dialog)
{
    m_sessionModel = new SessionModel {this};

    m_proxyModel = new QSortFilterProxyModel {this};
    m_proxyModel->setSourceModel(m_sessionModel);

    m_sessionView = new QTableView {this};
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

    m_msgWidget = new KMessageWidget {this};
    m_msgWidget->hide();

    auto vlayout = new QVBoxLayout {this};
    vlayout->addWidget(m_msgWidget);
    vlayout->addWidget(m_sessionView);

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    m_buttonBox->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Open session"));
    vlayout->addWidget(m_buttonBox);

    connect(m_sessionView, &QTableView::doubleClicked, this, &SessionDialog::slotDoubleClicked);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SessionDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_sessionModel, &SessionModel::rowsInserted, this, &SessionDialog::slotSessionAdded);
    connect(m_sessionModel, &SessionModel::rowsRemoved, this, &SessionDialog::slotEmptyModel);

    setLayout(vlayout);
    setWindowTitle(title);

    slotEmptyModel();
}

Session SessionDialog::selectedSession() const
{
    return m_selectedSession;
}

void SessionDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete and not m_sessionModel->isEmpty()) {
        removeDialog();
    }

    QDialog::keyPressEvent(event);
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

void SessionDialog::slotSessionAdded()
{
    if (m_buttonBox->button(QDialogButtonBox::Ok)->isEnabled())
        return;

    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    m_buttonBox->button(QDialogButtonBox::Ok)->setToolTip(QString());
}

void SessionDialog::slotEmptyModel()
{
    if (not m_sessionModel->isEmpty())
        return;

    const QString message = i18nc("@info", "You don't have any saved session yet.");

    m_buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    m_buttonBox->button(QDialogButtonBox::Ok)->setToolTip(message);

    m_msgWidget->setMessageType(KMessageWidget::Information);
    m_msgWidget->setText(message);
    m_msgWidget->animatedShow();
}

QModelIndex SessionDialog::selectedIndex()
{
    auto sortedIndex = m_sessionView->selectionModel()->currentIndex();

    return m_proxyModel->mapToSource(sortedIndex);
}

void SessionDialog::removeDialog()
{
    const auto buttonCode = KMessageBox::warningYesNo(
                this,
                i18nc("@info", "Do you want to remove the selected session?"),
                i18nc("@title:window", "Confirm deletion"),
                KStandardGuiItem::yes(),
                KStandardGuiItem::no(),
                QStringLiteral("delete-session"));

    if (buttonCode != KMessageBox::Yes)
        return;

    m_sessionModel->removeRow(selectedIndex().row());
}

