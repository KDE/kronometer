/*
    SPDX-FileCopyrightText: 2014 Elvis Angelaccio <elvis.angelaccio@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "mainwindow.h"
#include "colorsettings.h"
#include "fontsettings.h"
#include "generalsettings.h"
#include "lapitemdelegate.h"
#include "lapmodel.h"
#include "sessiondialog.h"
#include "sessionmodel.h"
#include "settings.h"
#include "stopwatch.h"
#include "timedisplay.h"

#include <KActionCollection>
#include <KConfigDialog>
#include <KHelpMenu>
#include <KLocalizedString>
#include <KMessageBox>
#include <KToggleAction>
#include <KToolBar>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenuBar>
#include <QSaveFile>
#include <QSortFilterProxyModel>
#include <QSplitter>
#include <QStatusBar>
#include <QTableView>
#include <QToolButton>

MainWindow::MainWindow(SessionModel *sessionModel, QWidget *parent, const Session& session) : KXmlGuiWindow(parent),
    m_sessionModel {sessionModel},
    m_session {session}
{
    Q_INIT_RESOURCE(kronometerui);

    m_stopwatch = new Stopwatch {this};
    m_stopwatchDisplay = new TimeDisplay {this};
    connect(m_stopwatch, &Stopwatch::time, m_stopwatchDisplay, &TimeDisplay::setTime);  // bind stopwatch to its display
    connect(m_stopwatch, &Stopwatch::running, this, &MainWindow::slotRunning);
    connect(m_stopwatch, &Stopwatch::paused, this, &MainWindow::slotPaused);
    connect(m_stopwatch, &Stopwatch::inactive, this, &MainWindow::slotInactive);

    setupCentralWidget();
    setupActions();
    setupGUI(ToolBar | Keys | Save | Create, QStringLiteral("kronometerui.rc"));

    // #351746: prevent ugly 640x480 default size (unless there is a previous size to be restored, see #361494).
    if (!isWindowSizeSaved()) {
        resize(minimumSizeHint());
    }

    loadSettings();
    statusBar()->hide();

    if (m_session.isEmpty()) {
        slotInactive();
    }
    else {
        loadSession();
    }

    // TODO: replace this with solid-power API, once it's released.
    QDBusConnection::systemBus().connect(
                QStringLiteral("org.freedesktop.login1"),
                QStringLiteral("/org/freedesktop/login1"),
                QStringLiteral("org.freedesktop.login1.Manager"),
                QStringLiteral("PrepareForSleep"),
                this, SLOT(slotPrepareForSleep(bool)));

    m_screensaverInterface = new QDBusInterface(QStringLiteral("org.freedesktop.ScreenSaver"),
                                                QStringLiteral("/ScreenSaver"),
                                                QStringLiteral("org.freedesktop.ScreenSaver"));
        
    if (m_startTimerImmediately) {
        m_stopwatch->start();        
    }
    
}

MainWindow::~MainWindow()
{
    if (m_controlMenuButton) {
        m_controlMenuButton->disconnect();
    }
}

void MainWindow::setWindowTitle(const QString& title)
{
    if (title.endsWith(QLatin1String("[*]"))) {
        KXmlGuiWindow::setWindowTitle(title);
    }
    else {
        KXmlGuiWindow::setWindowTitle(title + QLatin1String("[*]"));
    }
}

void MainWindow::enableLapShortcuts(bool enable)
{
    QList<QKeySequence> shortcuts;
    if(enable) {
        shortcuts << Qt::Key_Return << Qt::Key_Enter;
    }
    actionCollection()->setDefaultShortcuts(m_lapAction, shortcuts);
}

bool MainWindow::queryClose()
{
    if (m_stopwatch->isInactive()) {
        return true;  // exit without ask
    }

    if (m_stopwatch->isRunning()) {
        m_stopwatch->pause();
    }

    if (m_session.isEmpty()) {
        auto buttonCode = KMessageBox::warningContinueCancel(
                    this,
                    i18n("Do you want to quit and lose your unsaved times?"),
                    i18nc("@title:window", "Confirm Quit"),
                    KStandardGuiItem::quit(),
                    KStandardGuiItem::cancel(),
                    QStringLiteral("quit-and-lose-times"));

        if (buttonCode != KMessageBox::Cancel) {
            return true;
        }
        return false;
    }
    else if (m_session.isOutdated()) {
        auto buttonCode = KMessageBox::warningYesNoCancel(this, i18n("Save times to session %1?", m_session.name()));

        switch (buttonCode) {
        case KMessageBox::Yes:
            slotSaveSession();
            return true;
        case KMessageBox::No:
            return true;
        default: // cancel
            return false;
        }
    }

    return true;  // there is an open session, but times are already saved.
}

void MainWindow::slotRunning()
{
    m_session.setIsOutdated(true);
    setWindowModified(true);

    activateScreenInhibition();

    stateChanged(QStringLiteral("running"));
}

void MainWindow::slotPaused()
{
    m_startAction->setText(i18nc("@action", "Re&sume"));
    disactivateScreenInhibition();

    if (m_session.isEmpty()) {
        stateChanged(QStringLiteral("paused"));
    }
    else {
        stateChanged(QStringLiteral("pausedSession"));
    }

    // the export action can be used only if there are laps (in both the paused states).
    // so, it can't be enabled directly from kronometerui.rc
    if (!m_lapModel->isEmpty()) {
        m_exportAction->setEnabled(true);
    }
}

void MainWindow::slotInactive()
{
    m_startAction->setText(i18nc("@action", "&Start"));

    m_session = Session {};

    setWindowTitle(i18nc("untitled window", "Untitled"));
    setWindowModified(false);
    disactivateScreenInhibition();

    stateChanged(QStringLiteral("inactive"));
}

void MainWindow::slotPrepareForSleep(bool beforeSleep)
{
    if (!beforeSleep)
        return;

    qDebug() << "System is going to sleep, pausing the stopwatch.";
    m_stopwatch->pause();
}

void MainWindow::slotShowSettings()
{
    if (KConfigDialog::showDialog(QStringLiteral("settings"))) {
        return;
    }

    auto dialog = new KConfigDialog {this, QStringLiteral("settings"), KronometerConfig::self()};
    dialog->setModal(true);

    auto generalPage = dialog->addPage(new GeneralSettings {this}, i18nc("@title:tab", "General Settings"));
    generalPage->setIcon(QIcon::fromTheme(QApplication::windowIcon().name()));

    auto fontPage = dialog->addPage(new FontSettings {this}, i18nc("@title:tab", "Font Settings"));
    fontPage->setIcon(QIcon::fromTheme(QStringLiteral("preferences-desktop-font")));

    auto colorPage = dialog->addPage(new ColorSettings {this}, i18nc("@title:tab", "Color Settings"));
    colorPage->setIcon(QIcon::fromTheme(QStringLiteral("fill-color")));

    connect(dialog, &KConfigDialog::settingsChanged, this, &MainWindow::slotWriteSettings);

    dialog->show();
}

void MainWindow::slotWriteSettings()
{
    KronometerConfig::self()->save();

    foreach (auto widget, QApplication::topLevelWidgets()) {
        auto window = qobject_cast<MainWindow*>(widget);

        if (window) {
            window->loadSettings();
        }
    }
}

void MainWindow::slotUpdateLapDock()
{
    m_lapView->selectRow(m_lapModel->rowCount() - 1);  // rows indexes start from 0
}

void MainWindow::slotNewSession()
{
    auto window = new MainWindow {m_sessionModel};
    window->show();
}

void MainWindow::slotOpenSession()
{
    auto dialog = new SessionDialog {m_sessionModel, this};

    connect(dialog, &QDialog::finished, this, [this, dialog](int result) {
        if (result == QDialog::Accepted) {
            auto window = new MainWindow {m_sessionModel, nullptr, dialog->selectedSession()};
            window->show();
        }
        dialog->deleteLater();
    });

    dialog->open();
}

void MainWindow::slotSaveSession()
{
    if (!m_session.isOutdated())
        return;

    m_session.clear();    // required for laps consistency
    m_session.setTime(m_stopwatch->raw());

    for (int i = 0; i < m_lapModel->rowCount(); i++) {
        m_session.addLap(m_lapModel->data(m_lapModel->index(i, 0), static_cast<int>(LapModel::Roles::LapRole)).value<Lap>());
    }

    m_session.setIsOutdated(false);
    m_sessionModel->update(m_session);

    setWindowModified(false);
}

void MainWindow::slotSaveSessionAs()
{
    auto accepted = false;
    auto name = QInputDialog::getText(this,
                                      i18nc("@title:window", "Save Session"),
                                      i18n("You can choose a name for this session:"),
                                      QLineEdit::Normal,
                                      {},
                                      &accepted);

    if (!accepted)
        return;

    if (name.isEmpty())
        name = i18nc("untitled session", "Untitled session");

    saveSessionAs(name);
}

void MainWindow::slotExportLapsAs()
{
    auto dialog = new QFileDialog {this};
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setOption(QFileDialog::DontConfirmOverwrite, false);
    dialog->setWindowTitle(i18nc("@title:window", "Export Laps"));
    dialog->setMimeTypeFilters({ QStringLiteral("text/csv"), QStringLiteral("application/json") });

    connect(dialog, &QDialog::finished, this, [this, dialog](int result) {
        if (result == QDialog::Accepted) {
            exportLapsAs(dialog->selectedFiles().at(0), dialog->selectedMimeTypeFilter());
        }
        dialog->deleteLater();
    });

    dialog->open();
}

void MainWindow::slotCopyToClipboard()
{
    QApplication::clipboard()->setText(m_stopwatchDisplay->currentTime());
}

void MainWindow::slotToggleMenuBar()
{
    // Menubar shown for the first time.
    if (KronometerConfig::menuBarNeverShown()) {
        KronometerConfig::setMenuBarNeverShown(false);
        slotWriteSettings();
    }

    menuBar()->setVisible(!menuBar()->isVisible());
    m_toggleMenuAction->setChecked(menuBar()->isVisible());

    menuBar()->isVisible() ? deleteControlMenuButton() : createControlMenuButton();
}

void MainWindow::slotUpdateControlMenu()
{
    auto menu = qobject_cast<QMenu*>(sender());
    if (!menu)
        return;

    // All actions get cleared by QMenu::clear().
    // This includes the sub-menus because 'menu' is their parent.
    menu->clear();

    auto ac = actionCollection();

    // Add "File" actions
    auto added = addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::New))), menu) |
                 addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::Open))), menu);

    if (added)
        menu->addSeparator();

    added = addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::Save))), menu) |
            addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::SaveAs))), menu);

    if (added)
        menu->addSeparator();

    added = addActionToMenu(m_exportAction, menu);

    if (added)
        menu->addSeparator();

    // Add "Edit actions
    added = addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::Copy))), menu);

    if (added)
        menu->addSeparator();

    // Add "Settings" menu entries
    addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::KeyBindings))), menu);
    addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::ConfigureToolbars))), menu);
    addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::Preferences))), menu);

    // Add "Help" menu
    auto helpMenu = new KHelpMenu {menu};
    menu->addMenu(helpMenu->menu());

    menu->addSeparator();
    addActionToMenu(ac->action(QString::fromLatin1(KStandardAction::name(KStandardAction::ShowMenubar))), menu);
}

void MainWindow::slotToolBarUpdated()
{
    if (menuBar()->isVisible())
        return;

    createControlMenuButton();
}

void MainWindow::setupCentralWidget()
{
    auto splitter = new QSplitter {this};

    m_lapModel = new LapModel {this};
    auto proxyModel = new QSortFilterProxyModel {this};
    proxyModel->setSourceModel(m_lapModel);

    m_lapView = new QTableView {this};
    m_lapView->setModel(proxyModel);
    m_lapView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_lapView->setGridStyle(Qt::DotLine);
    m_lapView->verticalHeader()->hide();
    m_lapView->horizontalHeader()->setStretchLastSection(true);
    m_lapView->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    m_lapView->setItemDelegate(new LapItemDelegate(this));
    // Enable sorting and resize the columns to take the sorting arrow into account.
    m_lapView->setSortingEnabled(true);
    m_lapView->resizeColumnsToContents();

    splitter->setOrientation(Qt::Horizontal);
    splitter->setChildrenCollapsible(false);
    splitter->addWidget(m_stopwatchDisplay);
    splitter->addWidget(m_lapView);

    setCentralWidget(splitter);
}

void MainWindow::setupActions()
{
    m_startAction = new QAction {this};
    m_pauseAction = new QAction {this};
    m_resetAction = new QAction {this};
    m_lapAction = new QAction {this};
    m_exportAction = new QAction {this};

    m_startAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-start")));

    m_pauseAction->setText(i18nc("@action", "&Pause"));  // pauseAction/resetAction have fixed text (startAction doesn't)
    m_pauseAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-pause")));

    m_resetAction->setText(i18nc("@action", "&Reset"));
    m_resetAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-reset")));

    m_lapAction->setText(i18nc("@action", "&Lap"));
    m_lapAction->setIcon(QIcon::fromTheme(QStringLiteral("chronometer-lap")));

    m_exportAction->setText(i18nc("@action:inmenu", "&Export Laps as..."));
    m_exportAction->setIcon(QIcon::fromTheme(QStringLiteral("document-export")));

    actionCollection()->addAction(QStringLiteral("start"), m_startAction);
    actionCollection()->addAction(QStringLiteral("pause"), m_pauseAction);
    actionCollection()->addAction(QStringLiteral("reset"), m_resetAction);
    actionCollection()->addAction(QStringLiteral("lap"), m_lapAction);
    actionCollection()->addAction(QStringLiteral("export_laps"), m_exportAction);
    actionCollection()->setDefaultShortcut(m_startAction, Qt::Key_Space);
    actionCollection()->setDefaultShortcut(m_pauseAction, Qt::Key_Space);
    actionCollection()->setDefaultShortcut(m_resetAction, Qt::Key_F5);
    // Enable shortcut for lap
    enableLapShortcuts(true);

    // Pause is initially disabled.
    m_pauseAction->setEnabled(false);

    // triggers for Stopwatch "behavioral" slots
    connect(m_startAction, &QAction::triggered, m_stopwatch, &Stopwatch::start);
    connect(m_pauseAction, &QAction::triggered, m_stopwatch, &Stopwatch::pause);
    connect(m_resetAction, &QAction::triggered, m_stopwatch, &Stopwatch::reset);
    connect(m_lapAction, &QAction::triggered, m_stopwatch, &Stopwatch::storeLap);

    // triggers for LapModel slots
    connect(m_resetAction, &QAction::triggered, m_lapModel,&LapModel::clear);
    connect(m_stopwatch, &Stopwatch::lap, m_lapModel, &LapModel::addLap);

    // triggers for MainWindow "gui" slots
    connect(m_startAction, &QAction::triggered, this, &MainWindow::slotRunning);
    connect(m_pauseAction, &QAction::triggered, this, &MainWindow::slotPaused);
    connect(m_resetAction, &QAction::triggered, this, &MainWindow::slotInactive);
    connect(m_lapAction, &QAction::triggered, this, &MainWindow::slotUpdateLapDock);

    // triggers for TimeDisplay slots
    connect(m_resetAction, &QAction::triggered, m_stopwatchDisplay, &TimeDisplay::reset);

    // File menu triggers
    KStandardAction::quit(this, &QWidget::close, actionCollection());
    KStandardAction::preferences(this, &MainWindow::slotShowSettings, actionCollection());
    KStandardAction::openNew(this, &MainWindow::slotNewSession, actionCollection());
    KStandardAction::save(this, &MainWindow::slotSaveSession, actionCollection());
    KStandardAction::saveAs(this, &MainWindow::slotSaveSessionAs, actionCollection());
    KStandardAction::open(this, &MainWindow::slotOpenSession, actionCollection());
    KStandardAction::copy(this, &MainWindow::slotCopyToClipboard, actionCollection());
    connect(m_exportAction, &QAction::triggered, this, &MainWindow::slotExportLapsAs);

    m_toggleMenuAction = KStandardAction::showMenubar(nullptr, nullptr, actionCollection());
    // QueuedConnection prevents crashes when toggling the visibility
    connect(m_toggleMenuAction, &KToggleAction::triggered, this, &MainWindow::slotToggleMenuBar, Qt::QueuedConnection);
}

void MainWindow::loadSettings()
{
    auto timeFrac = KronometerConfig::showSecondFractions() ? KronometerConfig::fractionsType() : TimeFormat::NoFractions;
    auto lapFrac = KronometerConfig::showSecondFractions() ? KronometerConfig::lapFractionsType() : TimeFormat::NoFractions;

    auto timeFormat = TimeFormat {KronometerConfig::showHours(), KronometerConfig::showMinutes(), timeFrac};
    auto lapTimeFormat = TimeFormat {KronometerConfig::showLapHours(), KronometerConfig::showLapMinutes(), lapFrac};

    m_startTimerImmediately = KronometerConfig::startTimerImmediately();

    m_lapAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    m_exportAction->setVisible(KronometerConfig::isLapsRecordingEnabled());
    m_lapView->setVisible(KronometerConfig::isLapsRecordingEnabled());
    m_lapView->setColumnHidden(m_lapModel->columnForRole(LapModel::Roles::AbsoluteTimeRole), !KronometerConfig::showLapAbsoluteTimes());
    m_lapView->setColumnHidden(m_lapModel->columnForRole(LapModel::Roles::NoteRole), !KronometerConfig::showLapNotes());
    m_lapModel->setTimeFormat(lapTimeFormat);
    timeFormat.showDividers(false);
    m_stopwatchDisplay->setTimeFormat(timeFormat);
    m_stopwatchDisplay->setHoursFont(KronometerConfig::hourFont());
    m_stopwatchDisplay->setMinutesFont(KronometerConfig::minFont());
    m_stopwatchDisplay->setSecondsFont(KronometerConfig::secFont());
    m_stopwatchDisplay->setFractionsFont(KronometerConfig::fracFont());
    m_stopwatchDisplay->setBackgroundColor(KronometerConfig::backgroundColor());
    m_stopwatchDisplay->setTextColor(KronometerConfig::textColor());

    setupGranularity();

    // Always hide the menubar until the user shows it for the first time.
    if (KronometerConfig::menuBarNeverShown()) {
        menuBar()->hide();
    }

    if (menuBar()->isHidden()) {
        m_toggleMenuAction->setChecked(false);
        createControlMenuButton();
    }
}

void MainWindow::setupGranularity()
{
    if (!KronometerConfig::showSecondFractions()) {
        m_stopwatch->setGranularity(Stopwatch::Granularity::Seconds);
        return;
    }

    switch (KronometerConfig::fractionsType()) {
    case TimeFormat::UpToTenths:
        m_stopwatch->setGranularity(Stopwatch::Granularity::Tenths);
        break;
    case TimeFormat::UpToHundredths:
        m_stopwatch->setGranularity(Stopwatch::Granularity::Hundredths);
        break;
    case TimeFormat::UpToMilliseconds:
        m_stopwatch->setGranularity(Stopwatch::Granularity::Milliseconds);
        break;
    default:
        break;
    }
}

void MainWindow::saveSessionAs(const QString& name)
{
    auto newSession = Session {m_stopwatch->raw()};
    newSession.setName(name);

    for (int i = 0; i < m_lapModel->rowCount(); i++) {
        newSession.addLap(m_lapModel->data(m_lapModel->index(i, 0), static_cast<int>(LapModel::Roles::LapRole)).value<Lap>());
    }

    m_sessionModel->append(newSession);

    m_session = newSession;

    setWindowTitle(m_session.name());
    setWindowModified(false);
}

void MainWindow::loadSession()
{
    m_stopwatch->initialize(m_session.time());

    const auto laps = m_session.laps();
    for (const auto& lap : laps) {
        m_lapModel->append(lap);
    }

    slotPaused();   // enter in paused state
    setWindowTitle(m_session.name());
}

void MainWindow::exportLapsAs(const QString& name, const QString& mimeType)
{
    if (name.isEmpty()) {
        return;
    }

    auto exportName = name;

    if (mimeType == QLatin1String("application/json")) {
        if (!exportName.endsWith(QLatin1String(".json"))) {
            exportName += QLatin1String(".json");
        }

        QSaveFile exportFile {exportName};
        exportFile.open(QIODevice::WriteOnly);

        QJsonObject json;
        exportLapsAsJson(json);

        auto exportDoc = QJsonDocument {json};
        exportFile.write(exportDoc.toJson());
        exportFile.commit();
    }
    else if (mimeType == QLatin1String("text/csv")) {
        if (!exportName.endsWith(QLatin1String(".csv"))) {
            exportName += QLatin1String(".csv");
        }

        QSaveFile exportFile {exportName};
        exportFile.open(QIODevice::WriteOnly);

        QTextStream stream {&exportFile};
        exportLapsAsCsv(stream);

        exportFile.commit();
    }
}

void MainWindow::exportLapsAsJson(QJsonObject& json)
{
    auto laps = QJsonArray {};
    for (auto i = 0; i < m_lapModel->rowCount(); i++) {
        auto object = QJsonObject {};
        const auto lap = m_lapModel->data(m_lapModel->index(i, 0), static_cast<int>(LapModel::Roles::LapRole)).value<Lap>();
        lap.write(object);
        laps.append(object);
    }

    json[QStringLiteral("laps")] = laps;
}

void MainWindow::exportLapsAsCsv(QTextStream& out)
{
    out << '#' << timestampMessage() << '\r' << '\n';
    out << '#' << i18nc("@info:shell", "Lap number,Lap time,Global time,Note") << '\r' << '\n';

    for (auto i = 0; i < m_lapModel->rowCount(); i++) {
        const auto index = m_lapModel->index(i, 0);
        out << i;
        out << ',' << m_lapModel->data(index, static_cast<int>(LapModel::Roles::RelativeTimeRole)).toString();
        out << ',' << m_lapModel->data(index, static_cast<int>(LapModel::Roles::AbsoluteTimeRole)).toString();
        out << ',' << m_lapModel->data(index, static_cast<int>(LapModel::Roles::NoteRole)).toString();
        out << '\r' << '\n';
    }
}

bool MainWindow::isWindowSizeSaved() const
{
    KConfigGroup group {KSharedConfig::openConfig(), "MainWindow"};

    const auto keys = group.keyList();
    for (const auto& key : keys) {
        // Size keys contain the screen size, e.g. 'Width 1920' and 'Height 1080'.
        if (key.startsWith(QLatin1String("Height")) || key.startsWith(QLatin1String("Width"))) {
            return true;
        }
    }

    return false;
}

QString MainWindow::timestampMessage()
{
    const auto timestamp = QDateTime::currentDateTime();

    const QString timeString = QLocale::system().toString(timestamp, QLocale::LongFormat);

    return i18nc("@info:shell", "Created by Kronometer on %1", timeString);
}

void MainWindow::createControlMenuButton()
{
    if (!m_controlMenuButton.isNull()) {
        return;
    }

    m_controlMenuButton = new QToolButton {this};
    m_controlMenuButton->setIcon(QIcon::fromTheme(QStringLiteral("application-menu")));
    m_controlMenuButton->setPopupMode(QToolButton::InstantPopup);
    m_controlMenuButton->setToolButtonStyle(toolBar()->toolButtonStyle());

    auto controlMenu = new QMenu {m_controlMenuButton};
    connect(controlMenu, &QMenu::aboutToShow, this, &MainWindow::slotUpdateControlMenu);

    m_controlMenuButton->setMenu(controlMenu);

    toolBar()->addWidget(m_controlMenuButton);
    connect(toolBar(), &KToolBar::iconSizeChanged,
            m_controlMenuButton, &QToolButton::setIconSize);
    connect(toolBar(), &KToolBar::toolButtonStyleChanged,
            m_controlMenuButton, &QToolButton::setToolButtonStyle);

    // The control button may get deleted when e.g. the toolbar gets edited.
    // In this case we must add it again. The adding is done asynchronously using a QTimer.
    m_controlMenuTimer.reset(new QTimer {});
    m_controlMenuTimer->setInterval(500);
    m_controlMenuTimer->setSingleShot(true);
    connect(m_controlMenuButton, &QObject::destroyed, m_controlMenuTimer.get(), qOverload<>(&QTimer::start));
    connect(m_controlMenuTimer.get(), &QTimer::timeout, this, &MainWindow::slotToolBarUpdated);
}

void MainWindow::deleteControlMenuButton()
{
    delete m_controlMenuButton;
    m_controlMenuTimer.reset();
}

bool MainWindow::addActionToMenu(QAction *action, QMenu *menu)
{
    if (!action || !menu)
        return false;

    const auto toolBarWidget = toolBar();
    const auto widgets = action->associatedWidgets();
    for (const auto widget : widgets) {
        if (widget == toolBarWidget) {
            return false;
        }
    }

    menu->addAction(action);
    return true;
}

void MainWindow::activateScreenInhibition()
{
    if (m_screenInhibitCookie) {
        qWarning() << "Screen inhibition is already active.";
        return;
    }

    auto pendingCall = m_screensaverInterface->asyncCall(QStringLiteral("Inhibit"),
                                                         QCoreApplication::applicationName(),
                                                         i18n("A stopwatch is running."));

    auto pendingCallWatcher = new QDBusPendingCallWatcher(pendingCall, this);
    connect(pendingCallWatcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *callWatcher) {
        QDBusPendingReply<quint32> reply = *callWatcher;
        if (reply.isValid()) {
            m_screenInhibitCookie = reply.value();
            qDebug() << "Screen inhibition activated, got cookie:" << m_screenInhibitCookie;
        } else {
            qWarning() << "Could not inhibit screen locker:" << reply.error();
        }
    });
}

void MainWindow::disactivateScreenInhibition()
{
    if (!m_screenInhibitCookie) {
        return;
    }

    auto pendingCall = m_screensaverInterface->asyncCall(QStringLiteral("UnInhibit"), m_screenInhibitCookie);

    auto pendingCallWatcher = new QDBusPendingCallWatcher(pendingCall, this);
    connect(pendingCallWatcher, &QDBusPendingCallWatcher::finished, this, [=](QDBusPendingCallWatcher *callWatcher) {
        QDBusPendingReply<void> reply = *callWatcher;
        if (reply.isValid()) {
            qDebug() << "Screen inhibition disabled.";
            m_screenInhibitCookie = 0;
        } else {
            qWarning() << "Could not disable screen inhibition:" << reply.error();
        }
    });
}

