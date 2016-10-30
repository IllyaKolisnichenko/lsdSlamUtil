#include "debugimage/qtdebugimage.h"

#include <QMainWindow>
#include <QDockWidget>
#include <QLabel>

QtDebugImage*           QtDebugImage::p_instatnceThis = 0;
QtDebugImageDestroyer   QtDebugImage::m_oDestroyer;

QtDebugImageDestroyer::~QtDebugImageDestroyer()
{
    delete p_instance;
}

void QtDebugImageDestroyer::initialize(QtDebugImage *p)
{
    p_instance = p;
}

void QtDebugImage::setParentWindow(QMainWindow *parent)
{
    parentWidget = parent;
}

QtDebugImage::QtDebugImage()
{

}

QtDebugImage::~QtDebugImage()
{

}

QtDebugImage *QtDebugImage::getInstance()
{
    if(!p_instatnceThis)
    {
        p_instatnceThis = new QtDebugImage();
        m_oDestroyer.initialize( p_instatnceThis );
    }

    return p_instatnceThis;
}

int QtDebugImage::waitKey(int milliseconds)
{
    std::cout << "QtDebugImage::waitKey().."  << std::endl;
}

void QtDebugImage::appendWidgetSlot(QString &qName, bool autoSize)
{
    std::cout << "QtDebugImage::appendWidgetSlot(): Make new window "  << std::endl;

    QString qtLabelName( "ImageLabel" );

    QLabel*         pLabel;
    QDockWidget*    widget;

    int w = 320;
    int h = 240;

    // Make new Label
    pLabel = new QLabel();
    // Set label name
    pLabel->setObjectName( qtLabelName );
    pLabel->setFixedSize( w, h );

    // New Dock widget
    widget = new QDockWidget( qName );
    // Set widget name
    widget->setObjectName( qName );

    // Set Widget
    widget->setWidget( pLabel );

    // Appen dock Widget to list
    parentWidget->addDockWidget( Qt::RightDockWidgetArea, widget );
}

void QtDebugImage::destroyAllwindows()
{
    std::cout << "QtDebugImage::destroyAllwindows().."  << std::endl;

    // Destroy all the windows
    /// TODO: Destroy all QDockWidgets
    // for all widget names in openWindows
        // finde widget pointer
        // destroy widget

    // Clear the queue
    openWindows.clear();
}

void QtDebugImage::addNewImageInstance(const char *windowName, const cv::Mat &image, bool autoSize)
{
    std::cout << "QtDebugImage::addNewImageInstance().."  << std::endl;

    // Add new instance of the image object
    displayQueue.push_back( DisplayImageObject() );

    // Install parameters of the new object
    displayQueue.back().autoSize    = autoSize;
    displayQueue.back().img         = image.clone();
    displayQueue.back().name        = windowName;
}

void QtDebugImage::showImage(const char *windowName, const cv::Mat &image, bool autoSize)
{
    std::cout << "QtDebugImage::showImage().."  << std::endl;

    QString qtWindowName( windowName );
    QString qtLabelName( "ImageLabel" );

    QLabel*         pLabel;
    QDockWidget*    widget;

    int w = 320;
    int h = 240;

    // Make new window if necessary
    // If parameter is not set
//    if( !autoSize )
//    {
        // If there is no name in the list
        if( openWindows.find( windowName ) == openWindows.end() )
        {
            // Save params
//            m_pNewWidget.name       = windowName;
//            m_pNewWidget.img        = img;
//            m_pNewWidget.autoSize   = autoSize;

            // Emit signal
            emit needNewWidgetSignal( qtWindowName, autoSize );

            // append to list
            openWindows.insert( windowName );
        }
//    }

    widget = parentWidget->findChild<QDockWidget *>( qtWindowName, Qt::FindDirectChildrenOnly );
    //
    pLabel = widget->findChild<QLabel *>( qtLabelName, Qt::FindDirectChildrenOnly );

    /// Check Format before !!!!
    QImage imgIn= QImage( (uchar*)  image.data,
                                    image.cols,
                                    image.rows,
                                    image.step,
                                    QImage::Format_RGB888 );

    // set a scaled pixmap to a w x h window keeping its aspect ratio
    pLabel->setPixmap( QPixmap::fromImage(imgIn).scaled( w, h, Qt::IgnoreAspectRatio ));

//    pLabel->setPixmap( QPixmap::fromImage(imgIn).scaled( image.cols, image.rows, Qt::KeepAspectRatio ));

    // Show image
//    cv::imshow(windowName, image);
}

void QtDebugImage::loopIteration()
{
    std::cout << "QtDebugImage::loopIteration().."  << std::endl;

    // If the queue is not empty
    while(displayQueue.size() > 0)
    {
        showImage( displayQueue.back().name.c_str(),
                   displayQueue.back().img,
                   displayQueue.back().autoSize         );

        // Delete the strcture from the queue
        displayQueue.pop_back();
    }
}

