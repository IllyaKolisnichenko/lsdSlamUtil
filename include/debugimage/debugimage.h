#ifndef DEBUGIMAGE_H
#define DEBUGIMAGE_H

#include <string>
#include <unordered_set>

#include <boost/thread.hpp>

#include <opencv2/opencv.hpp>

class QMainWindow;

class debugImage
{
protected:
    // The structure that describes image
    struct DisplayImageObject
    {
        cv::Mat         img;            // OpenCV image presentation
        std::string     name;           // The name of the image
        bool            autoSize;       // Flag for autosize
    };

public:
    debugImage();
    virtual ~debugImage();

    // Set parent widget
    virtual void setParentWindow(QMainWindow* parent = 0) = 0;

    void displayImage(const char* windowName, const cv::Mat& image, bool autoSize = 0 );

    void closeAllWindows();

    void    run();
    void    join();
    void    stop();

    virtual int waitKey(  int milliseconds );

protected:
    virtual void addNewImageInstance(const char *windowName, const cv::Mat &image, bool autoSize);
    virtual void showImage(const char* windowName, const cv::Mat& image, bool autoSize);

    virtual void loopIteration();

    virtual void destroyAllwindows();

private:
    // Function is called in the thread loop
    void displayThreadLoop();

private:
    // Flag of thread usage
    bool useImageDisplayThread;
    // Changing of value causes thread stop and all windows closing
    bool imageThreadKeepRunning;

    // Provide data thread safety
    boost::mutex                    displayMutex;
    // Conditional variable (probably it generates the signal)
    boost::condition_variable       displaySignal;
    // Pointer to the thread
    boost::thread*                  m_pThread;

protected:
    // Pointer to parent widget (MainWindow)
    QMainWindow* parentWidget;
};

#endif // DEBUGIMAGE_H
