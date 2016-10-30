#include "debugimage/opnecvdebugimage.h"

//***************** Singelton *****************
openCVDebugImage*           openCVDebugImage::p_instatnceThis = 0;
openCVDebugImageDestroyer   openCVDebugImage::m_oDestroyer;

openCVDebugImageDestroyer::~openCVDebugImageDestroyer()
{
    delete p_instance;
}

void openCVDebugImageDestroyer::initialize(openCVDebugImage *p)
{
    p_instance = p;
}

//***************** OpenCV Version *****************
openCVDebugImage::openCVDebugImage()
{

}

openCVDebugImage::~openCVDebugImage()
{

}

openCVDebugImage *openCVDebugImage::getInstance()
{
    if(!p_instatnceThis)
    {
        p_instatnceThis = new openCVDebugImage();
        m_oDestroyer.initialize( p_instatnceThis );
    }
    return p_instatnceThis;
}

int openCVDebugImage::waitKey(int milliseconds)
{
    // Waiting for pressing buttons
    return cv::waitKey(milliseconds);
}

void openCVDebugImage::setParentWindow(QMainWindow *parent)
{

}

void openCVDebugImage::destroyAllwindows()
{
    // Destroy all the windows
    cv::destroyAllWindows();

    // Clear the queue
    openWindows.clear();
}

void openCVDebugImage::addNewImageInstance(const char *windowName, const cv::Mat &image, bool autoSize)
{
    // Add new instance of the image object
    displayQueue.push_back(DisplayImageObject());

    // Install parameters of the new object
    displayQueue.back().autoSize    = autoSize;
    displayQueue.back().img         = image.clone();
    displayQueue.back().name        = windowName;
}

void openCVDebugImage::showImage(const char *windowName, const cv::Mat &image, bool autoSize)
{
    std::cout << "openCVDebugImage::showImage().."  << std::endl;

    // Make new window if necessary
    //  If parameter is not set
    if(!autoSize)
    {
        if( openWindows.find(windowName) == openWindows.end() )
        {
            cv::namedWindow ( windowName, cv::WINDOW_NORMAL     );
            cv::resizeWindow( windowName, image.cols, image.rows);

            openWindows.insert(windowName);
        }
    }

    // Show image
    cv::imshow(windowName, image);
}

void openCVDebugImage::loopIteration()
{
    std::cout << "openCVDebugImage::loopIteration().."  << std::endl;

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

