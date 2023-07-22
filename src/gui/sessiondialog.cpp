/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sessiondialog.h"
#include "sessionmodel.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <KMessageWidget>
#include <kwidgetsaddons_version.h>

#include <QDialogButtonBox>
#include <QKeyEvent>
#include <QPushButton>
#include <QScreen>
#include <QSortFilterProxyModel>
#include <QTableView>

SessionDialog::SessionDialog(SessionModel *sessionModel, QWidget *parent) : QDialog(parent, Qt::Dialog)
{
    setupUi(this);
    m_sessionModel = sessionModel;

    m_proxyModel = new QSortFilterProxyModel {this};
    m_proxyModel->setSourceModel(m_sessionModel);

    m_sessionView->setModel(m_proxyModel);
    m_sessionView->resizeColumnsToContents();
    // TODO: the user may want to select/remove more than one session

    m_msgWidget->hide();
    m_buttonBox->button(QDialogButtonBox::Ok)->setText(i18nc("@action:button", "Open session"));

    connect(m_sessionView, &QTableView::doubleClicked, this, &SessionDialog::slotDoubleClicked);
    connect(m_sessionView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SessionDialog::slotSelectionChanged);
    connect(m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_sessionModel, &SessionModel::rowsInserted, this, &SessionDialog::slotSessionAdded);
    connect(m_sessionModel, &SessionModel::rowsRemoved, this, &SessionDialog::slotEmptyModel);

    slotEmptyModel();
    slotSelectionChanged();

    auto screens = QGuiApplication::screens();
    // Set a good default size, tested on 1920x1200 and 1366x768 screens.
    resize(screens[0]->size() / 2.5);
}

Session SessionDialog::selectedSession() const
{
    return m_selectedSession;
}

void SessionDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete && !m_sessionModel->isEmpty()) {
        removeDialog();
    }

    QDialog::keyPressEvent(event);
}

void SessionDialog::accept()
{
    m_selectedSession = m_sessionModel->data(selectedIndex(), static_cast<int>(SessionModel::Roles::SessionRole)).value<Session>();
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
    if (!m_sessionModel->isEmpty())
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
#if KWIDGETSADDONS_VERSION < QT_VERSION_CHECK(5, 100, 0)
    const auto buttonCode = KMessageBox::warningYesNo(
                this,
                i18nc("@info", "Do you want to remove the selected session?"),
                i18nc("@title:window", "Confirm deletion"),
                KStandardGuiItem::yes(),
                KStandardGuiItem::no(),
                QStringLiteral("delete-session"));

    if (buttonCode != KMessageBox::Yes)
#else
    const auto buttonCode = KMessageBox::warningTwoActions(
                this,
                i18nc("@info", "Do you want to remove the selected session?"),
                i18nc("@title:window", "Confirm deletion"),
                KStandardGuiItem::remove(),
                KStandardGuiItem::cancel(),
                QStringLiteral("delete-session"));

    if (buttonCode != KMessageBox::PrimaryAction)
#endif
        return;

    m_sessionModel->removeRow(selectedIndex().row());
}


#include "moc_sessiondialog.cpp"
