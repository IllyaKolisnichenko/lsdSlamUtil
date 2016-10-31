#ifndef QTDEBUGIMAGE_H
#define QTDEBUGIMAGE_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

#include "debugimage.h"

class QtDebugImage;
class QMainWindow;

class QtDebugImageDestroyer
{
private:
    QtDebugImage* p_instance;
public:
    ~QtDebugImageDestroyer();
    void initialize( QtDebugImage* p );
};

class QtDebugImage : public QThread, public DebugImage
{
    Q_OBJECT
public:
    // Set parent widget
    virtual void setParentWindow(QMainWindow* parent = 0);

    // Add image
    virtual void displayImage(  const   char*       windowName,
                                const   cv::Mat&    image,
                                        bool        autoSize = 0 );

    virtual void closeAllWindows();

    // Wait some key
    virtual int waitKey(  int milliseconds );

protected:
    // Constructor
    QtDebugImage();
    QtDebugImage( const QtDebugImage& );
    QtDebugImage& operator=( QtDebugImage& );
    // Destructor
    virtual ~QtDebugImage();

    virtual void showImage( const   char*       windowName,
                            const   cv::Mat&    image,
                                    bool        autoSize    );

    virtual void destroyAllwindows();

    // Get instance of pointer
    static QtDebugImage* getInstance();

    // Class for Singelton Tempate
    friend class QtDebugImageDestroyer;
    friend class DebugImage;

protected:
    virtual void run();

private:
    // Singelton instace pointer
    static QtDebugImage*            p_instatnceThis;
    // Destroyer exemple
    static QtDebugImageDestroyer    m_oDestroyer;

private:
    // Pointer to parent widget (MainWindow)
    QMainWindow* parentWidget;

protected:
    // Provide data thread safety
    QMutex          displayMutex;
    // Conditional variable (probably it generates the signal)
    QWaitCondition  displaySignal;
};

#endif // QTDEBUGIMAGE_H

