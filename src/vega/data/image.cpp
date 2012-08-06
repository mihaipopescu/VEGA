#include <iostream>
#include <windows.h>
#include "image.h"
#include "../math/vector3d.h"



namespace vega
{
    namespace data
    {
        image::image(const std::string& _FileName)
            : i_model()
            , myWidth(0)
            , myHeight(0)
            , myBMPFileName(_FileName)
        {
        }

        bool image::create()
        {
            // a handle for the current OpenGL Device Contexts
            const HDC gldc = wglGetCurrentDC();
            HDC mdc = CreateCompatibleDC(gldc);

            std::cout << "Loading image file [" << myBMPFileName.c_str() << "]... ";

            // Loads the image.
            HBITMAP _DIBHandle = (HBITMAP)LoadImage(0, myBMPFileName.c_str(), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);	

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
            myHeight = (uint16)_biInfo.bmiHeader.biHeight;
            myWidth = (uint16)_biInfo.bmiHeader.biWidth;

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
                std::cerr << "Failed ! Reason: Invalid image bit count." << std::endl;
                DeleteObject(_DIBHandle);
                DeleteDC(mdc);
                return false;
            }

            DeleteObject(_DIBHandle);
            DeleteDC(mdc);

            std::cout << "Width = " << myWidth << ", Height = " << myHeight << std::endl;
            return true;
        }

        bool image::get_pixel( uint16 x, uint16 y, math::vector3d& pixel ) const
        {
            size_t index = 3*(y*myWidth + x);
            if( index >= myRGBData.size() )
                return false;

            pixel = math::vector3d(myRGBData[index+0]/255.f, myRGBData[index+1]/255.f, myRGBData[index+2]/255.f);
            return true;
        }

        bool image::set_pixel_contrast( uint16 x, uint16 y, float fContrast )
        {
            size_t index = 3*(y*myWidth + x);
            if( index >= myRGBData.size() )
                return false;

            float r = myRGBData[index+0] * fContrast;
            float g = myRGBData[index+1] * fContrast;
            float b = myRGBData[index+2] * fContrast;

            myRGBData[index+0] = r > 255.f ? 255 : (uint8)r;
            myRGBData[index+1] = g > 255.f ? 255 : (uint8)g;
            myRGBData[index+2] = b > 255.f ? 255 : (uint8)b;

            return true;
        }

    }
}