#ifndef GGTABBAR_H
#define GGTABBAR_H

#include <QScrollArea>
#include <QTabBar>
#include <QToolButton>
#include <QVariant>
#include <QLineEdit>

/* ------------------------------------------------------------------------- */

#define GG_TABBAR_MAX_SCROLL_SPEED 15

/* ------------------------------------------------------------------------- */

/**
 * Basic Tab Bar whose tabs are closed when clicking with the middle button.
 * Its default tabsClosable value is true.
 *
 * It also provides a isDragging() method to know if a tab is currently being dragged.
 */
class GGTabBar : public QTabBar
{
    Q_OBJECT
public:
    GGTabBar(QWidget* parent = nullptr);
    ~GGTabBar();
    const static int iVerticalMargin;
    const static int iHorizontalMargin;

    inline bool isDragging() { return m_bDragging; }

signals:
    void tabDoubleClicked(int);

protected:
    virtual void mousePressEvent    (QMouseEvent* e);
    virtual void mouseMoveEvent     (QMouseEvent* e);
    virtual void mouseReleaseEvent  (QMouseEvent* e);
    virtual void mouseDoubleClickEvent  (QMouseEvent* e);

private:
    bool m_bDragging;
    int m_iEditedTabIndex;
    QLineEdit * m_pTabNameEdit;
    void startRename(int);

private slots:
    void finishRename();
};

/**
 * Scrollable Tab Bar is a "scrollable" GGTabBar.
 * Its tabs are movable and when moving a tab outside of the TabBar, the tabBar is scrolled.
 *
 * The height of the GGScrollableTabBar is automatically adjusted to its content.
 * Therefore, it will not be displayed when there is no tab.
 *
 * When focusing a tab, the GGScrollableTabBar will automatically scroll to make it visible.
 *
 * The methods of GGTabBar are directly accessible through GGScrollableTabBar.
 */
class GGScrollableTabBar : public QScrollArea
{
    Q_OBJECT

public:
    GGScrollableTabBar(QWidget* parent = nullptr);
    ~GGScrollableTabBar();

    bool blockSignals(bool block); //Also blocks the GGTabBar signals.
    
    inline int addTab(const QString& text) { return m_pTabBar->addTab(text); }
    inline int addTab(const QIcon &icon, const QString &text) { return m_pTabBar->addTab(icon, text); }
    inline bool autoHide() const { return m_pTabBar->autoHide(); }
    inline int currentIndex() const { return m_pTabBar->currentIndex(); }
    inline int count() const { return m_pTabBar->count(); }
    inline bool documentMode() const { return m_pTabBar->documentMode(); }
    inline Qt::TextElideMode elideMode() const { return m_pTabBar->elideMode(); }
    inline bool expanding() const { return m_pTabBar->expanding(); }
    inline QSize iconSize() const { return m_pTabBar->iconSize(); }
    inline int insertTab(int index, const QString &text) { return m_pTabBar->insertTab(index, text); }
    inline int insertTab(int index, const QIcon&icon, const QString &text) { return m_pTabBar->insertTab(index, icon, text); }
    inline bool isTabEnabled(int index) const { return m_pTabBar->isTabEnabled(index); }
    inline void moveTab(int from, int to) { m_pTabBar->moveTab(from, to); }
    inline void removeTab(int index) { m_pTabBar->removeTab(index); }
    inline void setAutoHide(bool hide) { m_pTabBar->setAutoHide(hide); }
    inline void setDocumentMode(bool set) { m_pTabBar->setDocumentMode(set); }
    inline void setElideMode(Qt::TextElideMode elideMode) { m_pTabBar->setElideMode(elideMode); }
    inline void setExpanding(bool enabled) { m_pTabBar->setExpanding(enabled); }
    inline void setIconSize(const QSize &size) { m_pTabBar->setIconSize(size); }
    inline void setTabButton(int index, QTabBar::ButtonPosition position, QWidget *widget) { m_pTabBar->setTabButton(index, position, widget); }
    inline void setTabsClosable(bool closable) { m_pTabBar->setTabsClosable(closable); }
    inline void setTabData(int index, const QVariant& data) { m_pTabBar->setTabData(index, data); }
    inline void setTabEnabled(int index, bool bEnabled) { m_pTabBar->setTabEnabled(index, bEnabled); }
    inline void setTabIcon(int index, const QIcon &icon) { m_pTabBar->setTabIcon(index, icon); }
    inline void setTabText(int index, const QString&  text) { m_pTabBar->setTabText(index, text); }
    inline void setTabTextColor(int index, const QColor &color) { m_pTabBar->setTabTextColor(index, color); }
    inline int tabAt(const QPoint &pos) const;
    inline QWidget* tabButton(int index, QTabBar::ButtonPosition position) const { return m_pTabBar->tabButton(index, position); }
    inline bool tabsClosable() const { return m_pTabBar->tabsClosable(); }
    inline QVariant tabData(int index) const { return m_pTabBar->tabData(index); }
    inline QIcon tabIcon(int index) const { return m_pTabBar->tabIcon(index); }
    inline QRect tabRect(int index) const { return m_pTabBar->tabRect(index); }
    inline QString tabText(int index) const { return m_pTabBar->tabText(index); }
    inline QColor tabTextColor(int index) const { return m_pTabBar->tabTextColor(index); }

#ifndef QT_NO_TOOLTIP
    inline void setTabToolTip(int index, const QString &tip) { m_pTabBar->setTabToolTip(index, tip); }
    inline QString tabToolTip(int index) const { return m_pTabBar->tabToolTip(index); }
#endif

#ifndef QT_NO_WHATSTHIS
    inline void setTabWhatsThis(int index, const QString &text) { m_pTabBar->setTabWhatsThis(index, text); }
    inline QString tabWhatsThis(int index) const { return m_pTabBar->tabWhatsThis(index); }
#endif

signals:
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void tabMoved(int from, int to);
    void tabBarClicked(int index);
    void tabBarDoubleClicked(int index);

public slots:
    inline void setCurrentIndex(int index) { m_pTabBar->setCurrentIndex(index); }

protected:
    virtual bool eventFilter(QObject* o, QEvent* e);
    virtual void mouseMoveEvent     (QMouseEvent* e);
    virtual void mouseReleaseEvent  (QMouseEvent* e);

protected slots:
    inline void makeCurrentVisible() { this->makeVisible(m_pTabBar->currentIndex()); }
    void makeVisible(int index);
    void adjustHeight();
    inline void onCurrentChanged    (int index) { makeVisible(index); emit currentChanged(index); }
    inline void onTabCloseRequested (int index) { emit tabCloseRequested(index); }
    inline void onTabMoved          (int from, int to) { emit tabMoved(from, to); }
    inline void onTabBarClicked     (int index) { emit tabBarClicked(index); }
    inline void onTabBarDoubleClicked(int index) { emit tabBarDoubleClicked(index); }

private:
    void scroll(int dx);

private:
    GGTabBar*       m_pTabBar;
};

/**
 * GGTabBarWidget handles a GGScrollableTabBar and a button that displays a Menu
 * containing direct links to all the tabs when clicked.
 * The actions in the Menu are sorted by alphabetical order.
 *
 * By default, the button is not displayed when there is no tab.
 *
 * The methods of GGScrollableTabBar are directly accessible through GGTabBarWidget.
 */
class GGTabBarWidget : public QWidget
{
    Q_OBJECT

public:
    GGTabBarWidget(QWidget* parent = nullptr);
    ~GGTabBarWidget();

    bool blockSignals(bool block); //Also blocks the GGScrollableTabBar signals.

    inline int addTab(const QString& text)                    { m_pMenuButton->show(); return m_pScrollableTabBar->addTab(text); }
    inline int addTab(const QIcon &icon, const QString &text) { m_pMenuButton->show(); return m_pScrollableTabBar->addTab(icon, text); }
    inline void removeTab(int index) { m_pScrollableTabBar->removeTab(index); if( 0 == this->count() ) { m_pMenuButton->hide(); } }

    inline bool autoHide() const { return m_pScrollableTabBar->autoHide(); }
    inline int currentIndex() const { return m_pScrollableTabBar->currentIndex(); }
    inline int count() const { return m_pScrollableTabBar->count(); }
    inline bool documentMode() const { return m_pScrollableTabBar->documentMode(); }
    inline Qt::TextElideMode elideMode() const { return m_pScrollableTabBar->elideMode(); }
    inline bool expanding() const { return m_pScrollableTabBar->expanding(); }
    inline QSize iconSize() const { return m_pScrollableTabBar->iconSize(); }
    inline int insertTab(int index, const QString &text) { return m_pScrollableTabBar->insertTab(index, text); }
    inline int insertTab(int index, const QIcon&icon, const QString &text) { return m_pScrollableTabBar->insertTab(index, icon, text); }
    inline bool isTabEnabled(int index) const { return m_pScrollableTabBar->isTabEnabled(index); }
    inline void moveTab(int from, int to) { m_pScrollableTabBar->moveTab(from, to); }
    inline void setAutoHide(bool hide) { m_pScrollableTabBar->setAutoHide(hide); }
    inline void setDocumentMode(bool set) { m_pScrollableTabBar->setDocumentMode(set); }
    inline void setElideMode(Qt::TextElideMode elideMode) { m_pScrollableTabBar->setElideMode(elideMode); }
    inline void setExpanding(bool enabled) { m_pScrollableTabBar->setExpanding(enabled); }
    inline void setIconSize(const QSize &size) { m_pScrollableTabBar->setIconSize(size); }
    inline void setTabButton(int index, QTabBar::ButtonPosition position, QWidget *widget) { m_pScrollableTabBar->setTabButton(index, position, widget); }
    inline void setTabsClosable(bool closable) { m_pScrollableTabBar->setTabsClosable(closable); }
    inline void setTabData(int index, const QVariant& data) { m_pScrollableTabBar->setTabData(index, data); }
    inline void setTabEnabled(int index, bool bEnabled) { m_pScrollableTabBar->setTabEnabled(index, bEnabled); }
    inline void setTabIcon(int index, const QIcon &icon) { m_pScrollableTabBar->setTabIcon(index, icon); }
    inline void setTabText(int index, const QString&  text) { m_pScrollableTabBar->setTabText(index, text); }
    inline void setTabTextColor(int index, const QColor &color) { m_pScrollableTabBar->setTabTextColor(index, color); }
    inline int tabAt(const QPoint &pos) const;
    inline QWidget* tabButton(int index, QTabBar::ButtonPosition position) const { return m_pScrollableTabBar->tabButton(index, position); }
    inline bool tabsClosable() const { return m_pScrollableTabBar->tabsClosable(); }
    inline QVariant tabData(int index) const { return m_pScrollableTabBar->tabData(index); }
    inline QIcon tabIcon(int index) const { return m_pScrollableTabBar->tabIcon(index); }
    inline QRect tabRect(int index) const { return m_pScrollableTabBar->tabRect(index); }
    inline QString tabText(int index) const { return m_pScrollableTabBar->tabText(index); }
    inline QColor tabTextColor(int index) const { return m_pScrollableTabBar->tabTextColor(index); }

#ifndef QT_NO_TOOLTIP
    inline void setTabToolTip(int index, const QString &tip) { m_pScrollableTabBar->setTabToolTip(index, tip); }
    inline QString tabToolTip(int index) const { return m_pScrollableTabBar->tabToolTip(index); }
#endif

#ifndef QT_NO_WHATSTHIS
    inline void setTabWhatsThis(int index, const QString &text) { m_pScrollableTabBar->setTabWhatsThis(index, text); }
    inline QString tabWhatsThis(int index) const { return m_pScrollableTabBar->tabWhatsThis(index); }
#endif

signals:
    void currentChanged(int index);
    void tabCloseRequested(int index);
    void tabMoved(int from, int to);
    void tabBarClicked(int index);
    void tabBarDoubleClicked(int index);

public slots:
    inline void setCurrentIndex(int index) { m_pScrollableTabBar->setCurrentIndex(index); }

protected slots:
    void displayMenu();
    void onMenuAction(QAction* pAction);
    inline void onCurrentChanged    (int index) { emit currentChanged(index); }
    inline void onTabCloseRequested (int index) { emit tabCloseRequested(index); }
    inline void onTabMoved          (int from, int to) { emit tabMoved(from, to); }
    inline void onTabBarClicked     (int index) { emit tabBarClicked(index); }
    inline void onTabBarDoubleClicked(int index) { emit tabBarDoubleClicked(index); }

private:
    QToolButton*        m_pMenuButton;
    GGScrollableTabBar* m_pScrollableTabBar;
};

/* ------------------------------------------------------------------------- */

#endif /* GGTabBar_H */
