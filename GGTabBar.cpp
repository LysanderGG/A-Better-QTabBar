#include "GGTabBar.h"

#include <QGlobal.h>
#include <QHBoxLayout>
#include <QMenu>
#include <QMouseEvent>
#include <QScrollBar>
#include <QTimer>

/* ------------------------------------------------------------------------- */

GGTabBar::GGTabBar(QWidget* parent)
    : QTabBar(parent)
    , m_pTabNameEdit(nullptr)
    , m_iEditedTabIndex(-1)
    , m_bDragging(false)
    , m_bRenameOnDoubleClick(true)
{
    this->setTabsClosable(true);
}

GGTabBar::~GGTabBar()
{
}

void 
GGTabBar::mousePressEvent(QMouseEvent * e)
{
    m_bDragging = true;
    QTabBar::mousePressEvent(e);
}

void 
GGTabBar::mouseMoveEvent(QMouseEvent * e)
{
    e->ignore(); //The event must go to the GGTabBar parent.
    QTabBar::mouseMoveEvent(e);
}

void 
GGTabBar::mouseReleaseEvent(QMouseEvent* e)
{
    m_bDragging = false;
    
    if( Qt::MiddleButton == e->button() ) {
        this->tabCloseRequested(this->tabAt(e->pos()));
    }

    e->ignore(); //The event must go to the GGTabBar parent.
    QTabBar::mouseReleaseEvent(e);
}

void 
GGTabBar::mouseDoubleClickEvent(QMouseEvent* e)
{
    m_bDragging = false;

    int iTabIndex = tabAt(e->pos());
    emit tabDoubleClicked(iTabIndex);

    if( m_bRenameOnDoubleClick ) {
        startRename(iTabIndex);
    }
}

void
GGTabBar::startRename(int iTabIndex)
{
    static const int c_iVerticalMargin   = 3;
    static const int c_iHorizontalMargin = 6;

    if (m_iEditedTabIndex != -1) {
        return;
    }
    m_iEditedTabIndex = iTabIndex;
    QRect currentTabRect = tabRect(iTabIndex);
    m_pTabNameEdit = new QLineEdit(this);
    m_pTabNameEdit->show();
    m_pTabNameEdit->move(currentTabRect.left() + c_iHorizontalMargin, currentTabRect.top() + c_iVerticalMargin);
    m_pTabNameEdit->resize(currentTabRect.width() - 2 * c_iHorizontalMargin, currentTabRect.height() - 2 * c_iVerticalMargin);
    m_pTabNameEdit->setText(tabText(iTabIndex));
    m_pTabNameEdit->selectAll();
    m_pTabNameEdit->setFocus();
    connect(m_pTabNameEdit, SIGNAL(editingFinished()), this, SLOT(finishRename()));
}

void
GGTabBar::finishRename()
{
    setTabText(m_iEditedTabIndex, m_pTabNameEdit->text());
    m_pTabNameEdit->deleteLater();
    m_iEditedTabIndex = -1;
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

GGScrollableTabBar::GGScrollableTabBar(QWidget* parent)
    : QScrollArea(parent)
    , m_pTabBar(nullptr)
{
    m_pTabBar = new GGTabBar(this);
    m_pTabBar->setMovable(true);
    m_pTabBar->setUsesScrollButtons(false);

    this->setWidget(m_pTabBar);
    this->setFrameShape(QFrame::NoFrame);
    this->setWidgetResizable(true);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    this->horizontalScrollBar()->hide();

    connect(m_pTabBar, SIGNAL(currentChanged(int)),      this, SLOT(onCurrentChanged(int)));
    connect(m_pTabBar, SIGNAL(tabCloseRequested(int)),   this, SLOT(onTabCloseRequested(int)));
    connect(m_pTabBar, SIGNAL(tabMoved(int, int)),       this, SLOT(onTabMoved(int, int)));
    connect(m_pTabBar, SIGNAL(tabBarClicked(int)),       this, SLOT(onTabBarClicked(int)));
    connect(m_pTabBar, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(onTabBarDoubleClicked(int)));
}

GGScrollableTabBar::~GGScrollableTabBar()
{
}

bool 
GGScrollableTabBar::blockSignals(bool block)
{
    m_pTabBar->blockSignals(block);
    return QScrollArea::blockSignals(block);
}

bool 
GGScrollableTabBar::eventFilter(QObject* o, QEvent* e)
{
    //This works because QScrollArea::setWidget installs an eventFilter on the widget
    if(o && o == m_pTabBar && e->type() == QEvent::Resize) {
        this->setMaximumHeight(m_pTabBar->minimumSizeHint().height());
        QTimer::singleShot(0, this, SLOT(makeCurrentVisible()));
    }
 
    return QScrollArea::eventFilter(o, e);
}

void
GGScrollableTabBar::mouseMoveEvent(QMouseEvent* e)
{
    QPoint pos = e->pos();
    int w = this->width();

    if( pos.x() > w ) {
        this->scroll(qMin((pos.x() - w) / 2, GG_TABBAR_MAX_SCROLL_SPEED));
    } else if( pos.x() < 0 ) {
        this->scroll(qMax(pos.x() / 2, -GG_TABBAR_MAX_SCROLL_SPEED));
    }
    QWidget::mouseMoveEvent(e);
}

void 
GGScrollableTabBar::mouseReleaseEvent(QMouseEvent* e)
{
    this->makeCurrentVisible();
    QScrollArea::mouseReleaseEvent(e);
}

/* If the scroll area is smaller that the tab to make visible, ensure that its left part will be at least visible. */
void 
GGScrollableTabBar::makeVisible(int index)
{
    if( m_pTabBar->isDragging() ) { return; }

    const QRect& r = m_pTabBar->tabRect(index);
    this->ensureVisible(r.x() + r.width(), 0, 0, 0);
    this->ensureVisible(r.x(), 0, 0, 0);
}

void 
GGScrollableTabBar::scroll(int dx)
{
    QScrollBar* s = this->horizontalScrollBar();
    s->setValue(s->value() + dx);
}

void 
GGScrollableTabBar::adjustHeight()
{
    this->setMinimumHeight(m_pTabBar->height());
    this->setMaximumHeight(m_pTabBar->height());
}

/* ------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------- */

GGTabBarWidget::GGTabBarWidget(QWidget* parent)
    : QWidget(parent)
    , m_pMenuButton(nullptr)
    , m_pScrollableTabBar(nullptr)
{
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);

    m_pScrollableTabBar = new GGScrollableTabBar(this);
    m_pMenuButton = new QToolButton(this);
    pLayout->addWidget(m_pScrollableTabBar);
    pLayout->addWidget(m_pMenuButton);

    m_pMenuButton->setMenu(new QMenu(m_pMenuButton));
    m_pMenuButton->hide();
    m_pMenuButton->setStyleSheet("QToolButton::menu-indicator { image: none; }"); //Remove the menu-indicator as there is an image already.

    connect(m_pMenuButton,       SIGNAL(clicked()),                this, SLOT(displayMenu()));
    connect(m_pScrollableTabBar, SIGNAL(currentChanged(int)),      this, SLOT(onCurrentChanged(int)));
    connect(m_pScrollableTabBar, SIGNAL(tabCloseRequested(int)),   this, SLOT(onTabCloseRequested(int)));
    connect(m_pScrollableTabBar, SIGNAL(tabMoved(int, int)),       this, SLOT(onTabMoved(int, int)));
    connect(m_pScrollableTabBar, SIGNAL(tabBarClicked(int)),       this, SLOT(onTabBarClicked(int)));
    connect(m_pScrollableTabBar, SIGNAL(tabBarDoubleClicked(int)), this, SLOT(onTabBarDoubleClicked(int)));
}

GGTabBarWidget::~GGTabBarWidget() 
{
}

bool 
GGTabBarWidget::blockSignals(bool block)
{
    m_pScrollableTabBar->blockSignals(block);
    return QWidget::blockSignals(block);
}

static bool
_tabBarMenuActionLessThan(QAction* p1, QAction* p2)
{
    return p1->text() < p2->text();
}

void
GGTabBarWidget::displayMenu()
{
    QMenu* pMenu = m_pMenuButton->menu();
    QList<QAction*> lActions;

    pMenu->clear();

    for( int i = 0; i < m_pScrollableTabBar->count(); ++i )
    {
        QAction* pAct = new QAction(m_pScrollableTabBar->tabText(i), pMenu);
        pAct->setData(i);

        lActions.append(pAct);
    }

    qSort(lActions.begin(), lActions.end(), _tabBarMenuActionLessThan);
    pMenu->addActions(lActions);
    connect(pMenu, SIGNAL(triggered(QAction*)), this, SLOT(onMenuAction(QAction*)));

    if( !pMenu->isEmpty() ) {
        m_pMenuButton->showMenu();
    }
}

void 
GGTabBarWidget::onMenuAction(QAction* pAction)
{
    int tabIndex = pAction->data().toInt();
    m_pScrollableTabBar->setCurrentIndex(tabIndex);
}

/* ------------------------------------------------------------------------- */
