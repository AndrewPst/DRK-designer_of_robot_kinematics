#ifndef SPLITTERTITLEBAR_H
#define SPLITTERTITLEBAR_H

#include <QWidget>

class QPushButton;
class QDockWidget;
class QHBoxLayout;

class  SplitterTitleBar : public  QWidget
{
    Q_OBJECT
public:
    explicit SplitterTitleBar(QWidget *parent = 0);

    QSize sizeHint() const  { return  minimumSizeHint(); }
    QSize minimumSizeHint() const;

private slots:

    void onCloseButPressed(bool);
    void onMinButPressed(bool);


private:
    QPixmap minPix, closePix;

    QPushButton *_closeBut, *_minBut, *_splitHBut, *_splitVBut, *_addTabBut;

    QDockWidget *_parent;

    QHBoxLayout *_mainL = nullptr;
};

#endif // SPLITTERTITLEBAR_H
