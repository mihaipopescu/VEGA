#include <iostream>
#include <windows.h>
#include "image.h"
#include "../math/vector3d.h"



namespace vega
{
    namespace data
    {
        image::image()
            : i_model()
            , myWidth(0)
            , myHeight(0)
        {
        }

        bool image::create( const char *szFileName )
        {
            // a handle for the current OpenGL Device Contexts
            const HDC gldc = wglGetCurrentDC();
            HDC mdc = CreateCompatibleDC(gldc);

            std::cout << "Loading image file [" << szFileName << "]... ";

            // Loads the image.
            HBITMAP _DIBHandle = (HBITMAP)LoadImage(0, szFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);	

            if(!_DIBHandle)
            {
                std::cerr << "Failed ! Reason: Unable to load image." << std::endl;
                DeleteDC(mdc);
                return false;
            }

            BITMAPINFO _biInfo;
            ZeroMemory(&_biInfo, sizeof(BITMAPINFO));
            _biInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

            // send NULL to bits to get the bitmap info
            GetDIBits(mdc, _DIBHandle, 0, 0, NULL, (BITMAPINFO*)&_biInfo, DIB_RGB_COLORS);

            std::vector<uint8> pData(_biInfo.bmiHeader.biSizeImage);
            myHeight = _biInfo.bmiHeader.biHeight;
            myWidth = _biInfo.bmiHeader.biWidth;

            // read the bitmap data
            // NOTE: We use this method to access the bitmap bits in order to modify them
            // applying different filters or other image processing algorithms in real time 
            // such as blur effect (denoising) or other convolutions.
            GetDIBits(mdc, _DIBHandle, 0, myHeight, static_cast<LPVOID>(pData.data()), (BITMAPINFO*)&_biInfo, DIB_RGB_COLORS);

            // release old data and allocate new one
            myRGBData.reserve(3 * myWidth * myHeight);
            myRGBData.assign(3 * myWidth * myHeight, 0);

            // convert from BGR to RGB
            if(_biInfo.bmiHeader.biBitCount == 24)
            {
                size_t size = myWidth * myHeight;

                for(std::size_t i=0; i<size*3; i+=3)
                {
                    myRGBData[i] = pData[i+2];
                    myRGBData[i+1] = pData[i+1];
                    myRGBData[i+2] = pData[i];
                }
            }
            else
            {
                myHeight = 0;
                myWidth = 0;
                std::cerr << "Failed ! Reason: Invalid image bitcout." << std::endl;
                DeleteObject(_DIBHandle);
                DeleteDC(mdc);
                return false;
            }

            DeleteObject(_DIBHandle);
            DeleteDC(mdc);

            std::cout << "Width = " << myWidth << ", Height = " << myHeight << std::endl;
            return true;
        }
    }
}