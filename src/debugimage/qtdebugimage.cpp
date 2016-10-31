#include "debugimage/qtdebugimage.h"

#include <QApplication>
#include <QMainWindow>
#include <QDockWidget>
#include <QLabel>

#include <QMutexLocker>

//***************** Singelton *****************
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

//***************** QT Version *****************
void QtDebugImage::setParentWindow(QMainWindow *parent)
{
    parentWidget = parent;
}

void QtDebugImage::displayImage(const char *windowName, const cv::Mat &image, bool autoSize)
{
    std::cout << "QtDebugImage::displayImage() .."  << std::endl;

    // If need to run thread
    if( useImageDisplayThread )
    {
        // If thread wasn't started yet
        if( !this->isRunning() )
        {
            this->start();
//            this->moveToThread( parentWidget->thread() );

//            this->moveToThread( QApplication::instance()->thread() );
            /// wait for thread running;
            /// !!!!
            ///
        }
        // Lock an access to the data
        QMutexLocker lock(&displayMutex);

        // Add new instance of the image object
        displayQueue.push_back(DisplayImageObject());

        // Install parameters of the new object
        displayQueue.back().autoSize    = autoSize;
        displayQueue.back().img         = image.clone();
        displayQueue.back().name        = windowName;

        // If there are thread that are waiting for being unlocked than one of them will be unlocked
        displaySignal.wakeOne();
        std::cout << "displaySignal.notify_one() .."  << std::endl;
    }
    else
    {
        showImage( windowName, image, autoSize);
    }

}

void QtDebugImage::closeAllWindows()
{

}

//***************** QT Version *****************
QtDebugImage::QtDebugImage()
{
    std::cout << "QtDebugImage::QtDebugImage(): Constructor.."  << std::endl;
}

QtDebugImage::~QtDebugImage()
{
    std::cout << "QtDebugImage::~QtDebugImage(): Destructor Done.."  << std::endl;
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

void QtDebugImage::run()
{
    // Notify that thread is started
    printf("QtDebugImage::run(): Started image display thread!\n");

    // Set the mutex for the thread
    QMutexLocker lock( &displayMutex );

    // While we need threads of images
    while( imageThreadKeepRunning )
    {
        std::cout << "displaySignal.wait( lock ).. Data Waiting .."  << std::endl;

        // Stop the thread until we receive new data
//        displaySignal.wait( &lock );
        displaySignal.wait( &displayMutex, 300 );
        std::cout << "displaySignal.wait( lock ).. Data received"  << std::endl;

        // If we need to pause the thread
        if(!imageThreadKeepRunning)
            // Break the loop
            break;

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

    // Delete all the windows
    destroyAllwindows();

    // Notify that thread is finished
    printf("debugImage::displayThreadLoop(): Ended image display thread!\n");
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

    widget = parentWidget->findChild<QDockWidget *>( qtWindowName, Qt::FindDirectChildrenOnly );
    // Make new window if necessary
    // If parameter is not set
//    if( !autoSize )
//    {
        // If there is no name in the list
//        if( openWindows.find( windowName ) == openWindows.end() )
        if( !widget )
        {
            // Make new Label
            pLabel = new QLabel();
            // Set label name
            pLabel->setObjectName( qtLabelName );
            pLabel->setFixedSize( w, h );
//            pLabel->moveToThread(QApplication::instance()->thread());
            pLabel->moveToThread(parentWidget->thread());

            // New Dock widget
            widget = new QDockWidget( qtWindowName );
            // Set widget name
            widget->setObjectName( qtWindowName );
            widget->moveToThread(QApplication::instance()->thread());

            // Set Widget
            widget->setWidget( pLabel );            

            // Appen dock Widget to list
            parentWidget->addDockWidget( Qt::RightDockWidgetArea, widget );

            // append to list
            openWindows.insert( windowName );
        }
//    }

//    widget = parentWidget->findChild<QDockWidget *>( qtWindowName, Qt::FindDirectChildrenOnly );
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


