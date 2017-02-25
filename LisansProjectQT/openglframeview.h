#ifndef OPENGLFRAMEVIEW_H
#define OPENGLFRAMEVIEW_H

#include <QWidget>

namespace Ui {
class OpenGlFrameView;
}

class OpenGlFrameView : public QQuickItem
{
    Q_OBJECT
public:
    OpenGlFrameView(QWidget *parent);

private:
    Ui::OpenGlFrameView *ui;

signals:

public slots:
};

#endif // OPENGLFRAMEVIEW_H
