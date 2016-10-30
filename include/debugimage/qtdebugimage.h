#ifndef QTDEBUGIMAGE_H
#define QTDEBUGIMAGE_H

#include <QObject>

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

class QtDebugImage : public QObject, public debugImage
{
    Q_OBJECT
public:
    // Set parent widget
    virtual void setParentWindow(QMainWindow* parent = 0);

protected:
    // Constructor
    QtDebugImage();
    QtDebugImage( const QtDebugImage& );
    QtDebugImage& operator=( QtDebugImage& );

    // Destructor
    virtual ~QtDebugImage();

    // Class for Singelton Tempate
    friend class QtDebugImageDestroyer;

private:
    // Singelton instace pointer
    static QtDebugImage*            p_instatnceThis;
    // Destroyer exemple
    static QtDebugImageDestroyer    m_oDestroyer;

public:
    // Get instance of pointer
    static QtDebugImage* getInstance();
    virtual int waitKey(  int milliseconds );

public slots:
    // Slot for append new widget
    void appendWidgetSlot(  QString& qName, bool autoSize );

signals:
    // Need append new widget
    void needNewWidgetSignal( QString& qName, bool autoSize );

protected:
    virtual void addNewImageInstance( const char *windowName, const cv::Mat &image, bool autoSize);

    virtual void showImage(const char* windowName, const cv::Mat& image, bool autoSize);

    virtual void loopIteration();

    virtual void destroyAllwindows();

private:
    // Indigested list of all opened windows described as std::string
    std::unordered_set<std::string> openWindows;
    // Array of images
    std::vector<DisplayImageObject> displayQueue;

    // Temp struct for new widget
    DisplayImageObject m_pNewWidget;
};

#endif // QTDEBUGIMAGE_H

