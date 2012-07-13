#include "panel.h"


Panel::Panel(Shape shape, QWidget *parent) :
    QTabBar(parent)
{
    setShape (shape);
    setTabsClosable(true);
    setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
    setMovable(true);
//    setExpanding(false);
//    setDrawBase(false);
}
