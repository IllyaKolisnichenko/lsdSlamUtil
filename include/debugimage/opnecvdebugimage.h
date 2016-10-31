#ifndef OPNECVDEBUGIMAGE_H
#define OPNECVDEBUGIMAGE_H

#include "debugimage.h"

//#include <opencv2/highgui/highgui.hpp>
#include <boost/thread.hpp>

class QMainWindow;
class openCVDebugImage;

class openCVDebugImageDestroyer
{
private:
    openCVDebugImage* p_instance;
public:
    ~openCVDebugImageDestroyer();
    void initialize( openCVDebugImage* p );
};

class openCVDebugImage : public DebugImage
{

public:
    // Set parent widget
    virtual void setParentWindow(QMainWindow* parent = 0){}

    // Add image
    virtual void displayImage(  const   char*       windowName,
                                const   cv::Mat&    image,
                                        bool        autoSize = 0 );

    virtual void closeAllWindows();

    // Wait some key
    virtual int waitKey(  int milliseconds );

protected:
    // Constructor
    openCVDebugImage();
    openCVDebugImage( const openCVDebugImage& );
    openCVDebugImage& operator=( openCVDebugImage& );
    // Destructor
    virtual ~openCVDebugImage();

    virtual void showImage( const   char*       windowName,
                            const   cv::Mat&    image,
                                    bool        autoSize    );

    virtual void destroyAllwindows();

    // Get instance of pointer
    static openCVDebugImage* getInstance();

    friend class openCVDebugImageDestroyer;
    friend class DebugImage;

private:    
    // Singelton instace pointer
    static openCVDebugImage*            p_instatnceThis;
    // Destroyer exemple
    static openCVDebugImageDestroyer    m_oDestroyer;

protected:
    void    run();
    void    join();
    void    stop();

private:
    // Function is called in the thread loop
    void displayThreadLoop();

protected:
    // Provide data thread safety
    boost::mutex                    displayMutex;
    // Conditional variable (probably it generates the signal)
    boost::condition_variable       displaySignal;
    // Pointer to the thread
    boost::thread*                  m_pThread;
};

#endif // OPNECVDEBUGIMAGE_H

