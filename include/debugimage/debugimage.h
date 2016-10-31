#ifndef DEBUGIMAGE_H
#define DEBUGIMAGE_H

#include <string>
#include <unordered_set>

//#include <boost/thread.hpp>

#include <opencv2/opencv.hpp>

class QMainWindow;

class DebugImage
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
    // Debug ID
    enum DebugImage_ID {  None, OpenCV_ID, Qt_ID };

    // Set parent widget
    virtual void setParentWindow(QMainWindow* parent = 0) = 0;

    // Add image
    virtual void displayImage(  const   char*       windowName,
                                const   cv::Mat&    image,
                                        bool        autoSize = 0 );

    /// !!!! Make RESET() function
    virtual void closeAllWindows()=0;

    // Wait some key
    virtual int  waitKey(  int milliseconds );

    // Change type
    static void setType( DebugImage_ID type );
    // Get instance of pointer
    static DebugImage* getInstance();

protected:
    DebugImage();
    virtual ~DebugImage();

    // Realy show image
    virtual void showImage          ( const char*       windowName,
                                      const cv::Mat     &image,
                                            bool        autoSize = true   );

    // Destroy all windows
    virtual void destroyAllwindows();

protected:
    // Flag of thread usage
    bool useImageDisplayThread;
    // Flag for stop image thread
    bool imageThreadKeepRunning;

    // Indigested list of all opened windows described as std::string
    std::unordered_set<std::string> openWindows;
    // Array of images
    std::vector<DisplayImageObject> displayQueue;

private:
    // Type for all instances
    static DebugImage_ID   m_eType;

};

#endif // DEBUGIMAGE_H
