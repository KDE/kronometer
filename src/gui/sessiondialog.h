/*
    SPDX-FileCopyrightText: 2015 Elvis Angelaccio <elvis.angelaccio@kde.org>

    This file is part of Kronometer.

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef SESSIONDIALOG_H
#define SESSIONDIALOG_H

#include "ui_sessiondialog.h"
#include "session.h"

#include <QDialog>

class SessionModel;

class KMessageWidget;

class QDialogButtonBox;
class QSortFilterProxyModel;
class QTableView;

/**
 * @brief Dialog for sessions interaction.
 */
class SessionDialog : public QDialog, public Ui::SessionDialog
{
    Q_OBJECT

public:

    explicit SessionDialog(SessionModel *sessionModel, QWidget *parent = nullptr);

    Session selectedSession() const;

public slots:

    virtual void accept() override;

protected:

    virtual void keyPressEvent(QKeyEvent *event) override;

private slots:

    /**
     * Call accept() if the selection's column is not editable.
     * @param index The selected index.
     */
    void slotDoubleClicked(const QModelIndex& index);

    /**
     * Disable the OK button if no session is selected.
     */
    void slotSelectionChanged();

    /**
     * Enable the OK button after the first session is added.
     */
    void slotSessionAdded();

    /**
     * Disable the OK button if no session is available.
     */
    void slotEmptyModel();

private:

    SessionModel *m_sessionModel;
    QSortFilterProxyModel *m_proxyModel;
    Session m_selectedSession;

    /**
     * @return The actual selected index in the view.
     */
    QModelIndex selectedIndex();

    /**
     * Ask confirm to the user before removing a session.
     */
    void removeDialog();

    Q_DISABLE_COPY(SessionDialog)
};

#endif
