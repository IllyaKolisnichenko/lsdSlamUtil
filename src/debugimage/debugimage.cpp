#include "debugimage/debugimage.h"

debugImage::debugImage() :
    m_pThread               ( 0     ),
    useImageDisplayThread   ( true  ),
    imageThreadKeepRunning  ( true  )
{

}

debugImage::~debugImage()
{
    // if Thread running
    if( m_pThread )
        join();

    std::cout << "debugImage::~debugImage(): Destructor Done.."  << std::endl;
}

void debugImage::displayImage(const char *windowName, const cv::Mat &image, bool autoSize)
{
    std::cout << "debugImage::displayImage() .."  << std::endl;
    if( useImageDisplayThread )
    {
        // If thread wasn't started yet
        if(m_pThread == 0)
            run();        // Run the thread

        // Lock an access to the data
        boost::unique_lock<boost::mutex> lock(displayMutex);

        // Add new image instance
        addNewImageInstance( windowName, image, autoSize );

        // If there are thread that are waiting for being unlocked than one of them will be unlocked
        displaySignal.notify_one();
        std::cout << "openCVdisplaySignal.notify_one() .."  << std::endl;
    }
    else
    {
        showImage( windowName, image, autoSize);
    }
}

void debugImage::destroyAllwindows()
{
    std::cout << "debugImage::destroyAllwindows(): Destroy All Windows"  << std::endl;
}

void debugImage::closeAllWindows()
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

void debugImage::run()
{
    if( m_pThread == 0)
    {
        // Allow the thread of images
        imageThreadKeepRunning  = true;
        // Run function in the thread
        m_pThread      = new boost::thread(&debugImage::displayThreadLoop, this);
    }
}

void debugImage::join()
{
    if( m_pThread )
    {
        m_pThread->join();
        delete m_pThread;
        m_pThread = 0;
        std::cout << "debugImage::join(): Join thread"  << std::endl;
    }
}

void debugImage::stop()
{
    if( m_pThread )
    {
        m_pThread->interrupt();
        this->join();

        std::cout << "debugImage::stop(): Stoping thread"  << std::endl;
    }
}

int debugImage::waitKey(int milliseconds)
{
    std::cout << "debugImage::waitKey().."  << std::endl;
}

void debugImage::addNewImageInstance( const char *windowName, const cv::Mat &image, bool autoSize )
{
    std::cout << "debugImage::addNewImageInstance(): add New Image Instance.."  << std::endl;
}

void debugImage::showImage(const char *windowName, const cv::Mat &image, bool autoSize)
{
    std::cout << "debugImage::showImage().."  << std::endl;
}

void debugImage::loopIteration()
{
    std::cout << "debugImage::loopIteration().."  << std::endl;
}

void debugImage::displayThreadLoop()
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

        loopIteration();
    }

    // Delete all the windows
    destroyAllwindows();

    // Notify that thread is finished
    printf("debugImage::displayThreadLoop(): Ended image display thread!\n");
}

