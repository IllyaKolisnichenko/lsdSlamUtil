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
void openCVDebugImage::displayImage(const char *windowName, const cv::Mat &image, bool autoSize)
{
    std::cout << "openCVDebugImage::displayImage() .."  << std::endl;

    // If need to run thread
    if( useImageDisplayThread )
    {
        // If thread wasn't started yet
        if(m_pThread == 0)
        {
            run();
            /// wait for thread running;
            /// !!!!
            ///
        }
        // Lock an access to the data
        boost::unique_lock<boost::mutex> lock(displayMutex);

        // Add new instance of the image object
        displayQueue.push_back(DisplayImageObject());

        // Install parameters of the new object
        displayQueue.back().autoSize    = autoSize;
        displayQueue.back().img         = image.clone();
        displayQueue.back().name        = windowName;

        // If there are thread that are waiting for being unlocked than one of them will be unlocked
        displaySignal.notify_one();
        std::cout << "displaySignal.notify_one() .."  << std::endl;
    }
    else
    {
        showImage( windowName, image, autoSize);
    }
}

openCVDebugImage::openCVDebugImage() :
    m_pThread               ( 0     )
{
    std::cout << "openCVDebugImage::openCVDebugImage(): Constructor.."  << std::endl;
}

openCVDebugImage::~openCVDebugImage()
{
    // if Thread running
    if( m_pThread )
        join();

    std::cout << "openCVDebugImage::~openCVDebugImage(): Destructor Done.."  << std::endl;

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

void openCVDebugImage::closeAllWindows()
{
    // Lock an access to the data
    boost::unique_lock<boost::mutex> lock(displayMutex);

    if( useImageDisplayThread )
    {
        // If thread is running
        if(m_pThread != 0)
        {
            // Stop the thread
            imageThreadKeepRunning = false;
            // Notify all
            displaySignal.notify_all();
            // Message "Thread finish"
            printf( "debugImage::closeAllWindows(): Waiting for image display thread to end!\n");

            // Unlock the mutex
            lock.unlock();

            // Wait thread finish
            m_pThread->join();

            // Notify that thread is finished
            printf("debugImage::closeAllWindows(): Done waiting for image display thread to end!\n");

            // Null the pointer
            m_pThread = 0;
        }
    }
    else
    {
        // Destroy all the windows
        destroyAllwindows();
    }
}

void openCVDebugImage::destroyAllwindows()
{
    // Destroy all the windows
    cv::destroyAllWindows();

    // Clear the queue
    openWindows.clear();
}

void openCVDebugImage::run()
{
    if( m_pThread == 0)
    {
        // Allow the thread of images
        imageThreadKeepRunning  = true;
        // Run function in the thread
        m_pThread      = new boost::thread(&openCVDebugImage::displayThreadLoop, this);

        std::cout << "openCVDebugImage::run(): Run thread"  << std::endl;
    }
}

void openCVDebugImage::join()
{
    if( m_pThread )
    {
        m_pThread->join();
        delete m_pThread;
        m_pThread = 0;
        std::cout << "openCVDebugImage::join(): Join thread"  << std::endl;
    }
}

void openCVDebugImage::stop()
{
    if( m_pThread )
    {
        m_pThread->interrupt();
        this->join();

        std::cout << "openCVDebugImage::stop(): Stoping thread"  << std::endl;
    }
}

void openCVDebugImage::displayThreadLoop()
{
    // Notify that thread is started
    printf("debugImage::displayThreadLoop(): Started image display thread!\n");

    // Set the mutex for the thread
    boost::unique_lock<boost::mutex> lock( displayMutex );

    // While we need threads of images
    while( imageThreadKeepRunning )
    {
        std::cout << "displaySignal.wait( lock ).. Data Waiting .."  << std::endl;

        // Stop the thread until we receive new data
//        displaySignal.wait( lock );
        displaySignal.timed_wait( lock, boost::posix_time::milliseconds(300) );
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

