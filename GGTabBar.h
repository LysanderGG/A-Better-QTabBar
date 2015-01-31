#ifndef GGTABBAR_H
#define GGTABBAR_H

#include <QScrollArea>
#include <QTabBar>
#include <QToolButton>
#include <QVariant>

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
public:
    GGTabBar(QWidget* parent = nullptr);
    ~GGTabBar();

    inline bool isDragging() { return m_bDragging; }

protected:
    virtual void mousePressEvent    (QMouseEvent* e);
    virtual void mouseMoveEvent     (QMouseEvent* e);
    virtual void mouseReleaseEvent  (QMouseEvent* e);

private:
    bool m_bDragging;
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
    inline int count() const { return m_pTabBar->count(); }
    inline void removeTab(int index) { m_pTabBar->removeTab(index); }
    inline void setTabData(int index, const QVariant& data) { m_pTabBar->setTabData(index, data); }
    inline void setTabText(int index, const QString&  text) { m_pTabBar->setTabText(index, text); }
    inline QVariant tabData(int index) const { return m_pTabBar->tabData(index); }
    inline QString  tabText(int index) const { return m_pTabBar->tabText(index); }

signals:
    inline void currentChanged(int index);
    inline void tabCloseRequested(int index);

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
    inline void onCurrentChanged    (int index) { emit currentChanged(index); }
    inline void onTabCloseRequested (int index) { emit tabCloseRequested(index); }

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

    inline int addTab(const QString& text) { m_pMenuButton->show(); return m_pScrollableTabBar->addTab(text); }
    inline int count() const { return m_pScrollableTabBar->count(); }
    inline void removeTab(int index) { m_pScrollableTabBar->removeTab(index); if( 0 == this->count() ) { m_pMenuButton->hide(); } }
    inline void setTabData(int index, const QVariant& data) { m_pScrollableTabBar->setTabData(index, data); }
    inline void setTabText(int index, const QString&  text) { m_pScrollableTabBar->setTabText(index, text); }
    inline QVariant tabData(int index) const { return m_pScrollableTabBar->tabData(index); }
    inline QString  tabText(int index) const { return m_pScrollableTabBar->tabText(index); }

signals:
    inline void currentChanged(int index);
    inline void tabCloseRequested(int index);

public slots:
    inline void setCurrentIndex(int index) { m_pScrollableTabBar->setCurrentIndex(index); }

protected slots:
    void displayMenu();
    void onMenuAction(QAction* pAction);
    inline void onCurrentChanged    (int index) { emit currentChanged(index); }
    inline void onTabCloseRequested (int index) { emit tabCloseRequested(index); }

private:
    QToolButton*        m_pMenuButton;
    GGScrollableTabBar* m_pScrollableTabBar;
};

/* ------------------------------------------------------------------------- */

#endif /* GGTabBar_H */
