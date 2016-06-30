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

SessionDialog::SessionDialog(QWidget *parent, const QString& title) : QDialog(parent, Qt::Dialog)
{
    setupUi(this);
    m_sessionModel = new SessionModel {this};

    m_proxyModel = new QSortFilterProxyModel {this};
    m_proxyModel->setSourceModel(m_sessionModel);

    m_sessionView->setModel(m_proxyModel);
    m_sessionView->resizeColumnsToContents();
    // TODO: the user may want to select/remove more than one session

    m_msgWidget->hide();
    m_buttonBox->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Open session"));

    connect(m_sessionView, &QTableView::doubleClicked, this, &SessionDialog::slotDoubleClicked);
    connect(m_sessionView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SessionDialog::slotSelectionChanged);
    connect(m_buttonBox, &QDialogButtonBox::accepted, this, &SessionDialog::accept);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_sessionModel, &SessionModel::rowsInserted, this, &SessionDialog::slotSessionAdded);
    connect(m_sessionModel, &SessionModel::rowsRemoved, this, &SessionDialog::slotEmptyModel);

    setWindowTitle(title);

    slotEmptyModel();
    slotSelectionChanged();
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

void SessionDialog::slotSelectionChanged()
{
    auto openButton = m_buttonBox->button(QDialogButtonBox::Ok);
    m_sessionView->selectionModel()->hasSelection() ? openButton->setEnabled(true) : openButton->setEnabled(false);
}

void SessionDialog::slotSessionAdded()
{
    auto openButton = m_buttonBox->button(QDialogButtonBox::Ok);

    if (openButton->isEnabled())
        return;

    openButton->setEnabled(true);
    openButton->setToolTip(QString());
}

void SessionDialog::slotEmptyModel()
{
    if (not m_sessionModel->isEmpty())
        return;

    const auto message = i18nc("@info", "You don't have any saved session yet.");

    auto openButton = m_buttonBox->button(QDialogButtonBox::Ok);
    openButton->setEnabled(false);
    openButton->setToolTip(message);

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

