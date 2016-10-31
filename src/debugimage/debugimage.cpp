#include "debugimage/debugimage.h"
#include "debugimage/opnecvdebugimage.h"
#include "debugimage/qtdebugimage.h"

DebugImage::DebugImage_ID
DebugImage::m_eType = DebugImage::None;

DebugImage::DebugImage() :
        useImageDisplayThread   ( true      ),
        imageThreadKeepRunning  ( true      )
//        m_eType                 ( None )
{
    openWindows.clear();
    displayQueue.clear();
}

DebugImage::~DebugImage()
{

}

void DebugImage::displayImage( const char*      windowName,
                               const cv::Mat    &image,
                               bool             autoSize    )
{
    std::cout << "debugImage::displayImage().."  << std::endl;
}

void DebugImage::setType(DebugImage_ID type)
{
    m_eType = type;
    /// TODO: Close All Windows !!!
}

void DebugImage::destroyAllwindows()
{
    std::cout << "debugImage::destroyAllwindows(): Destroy All Windows"  << std::endl;
}

int DebugImage::waitKey(int milliseconds)
{
    std::cout << "debugImage::waitKey().."  << std::endl;
}

void DebugImage::showImage( const   char*       windowName,
                            const   cv::Mat     &image,
                                    bool        autoSize    )
{
    std::cout << "debugImage::showImage().."  << std::endl;
}

DebugImage *DebugImage::getInstance()
{
    DebugImage * p;
    switch ( m_eType )
    {
        case OpenCV_ID:
            p = new openCVDebugImage();
            break;
        case Qt_ID:
            p = new QtDebugImage();
            break;

        default:
            std::cout << "DebugImage::getInstance()..ERROR: Type DID'T Set.."  << std::endl;
            assert( false);

    }
    return p;
}
