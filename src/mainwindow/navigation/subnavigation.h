#ifndef SUBNAVIGATION_H
#define SUBNAVIGATION_H

#include <QWidget>

class SubNavigation : public QWidget
{
   Q_OBJECT
public:
  SubNavigation(QWidget *parent = nullptr);
  ~SubNavigation();
};

#endif // SUBNAVIGATION_H