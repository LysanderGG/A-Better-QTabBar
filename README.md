# A Better QTabBar

I had to work with Qt's [QTabBar](http://doc.qt.io/qt-5/qtabbar.html) and found that a few features were missing.
So here is a reimplementation of it to provide these functionalities.

Added features:

* Tabs closable with the mouse middle button.
* Scrollable tabs to reorder them.
* Menu to list up the tabs instead of the default ugly arrow buttons.



## Classes details

Note: The methods of GGTabBar are directly accessible from GGScrollableTabBar and GGTabBarWidget.

### GGTabBar
Tabs are closable with the mouse middle button.

### GGScrollableTabBar
It is a "scrollable" GGTabBar.
Its tabs are movable and when you move a tab outside of the TabBar, it is scrolled.
When focusing a tab, the GGScrollableTabBar will automatically scroll to make it visible.

### GGTabBarWidget
It handles a GGScrollableTabBar and a Menu containing a list of direct links to all the tabs.
The actions in the Menu are sorted by alphabetical order.
