// ManiaTitleCardColours.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <ManiaModLoader.h>

extern "C"
{

    void WriteColour(BYTE*& address, BYTE red, BYTE green, BYTE blue)
    {
        // Writes all three colours into the buffer in BGR order
        *(int*)address = red << 16 | green << 8 | blue;
        // Offsets the Address for the next set
        address += 4;
    }

    void SetupTitleCardColours()
    {
        // TitleCard Pointer
        auto addr = *(BYTE**)0x00E9C5CC;
        // Is in Encore Mode
        bool encore = *(addr + 0x37) == 5;
        // Address to the Background Colour within the Buffer 
        addr = (BYTE*)((*(int*)addr) + 0x0374);

        // Background Colour
        if (encore)
            *(int*)addr = 0x006BAE99; // Write Encore Mode Background Colour
        else
            *(int*)addr = 0x00F0C800; // Write Mania Mode Background Colour
        
        // Test Background Colour
        //    *(int*)addr = 0x00212086;

        // Jump back
        addr -= 0x10;
        if (encore) // Write Encore Mode Colours
        {
            // Normal Colours
            WriteColour(addr, 55 , 81 , 162); // NOTE: ADDR, R, G, B
            WriteColour(addr, 199, 82 , 91);
            WriteColour(addr, 66 , 143, 195);
            WriteColour(addr, 223, 177, 63);

            // Test Colours
            //WriteColour(addr, 246, 141, 21);
            //WriteColour(addr, 51, 48, 216);
            //WriteColour(addr, 132, 32, 34);
            //WriteColour(addr, 67, 97, 182);
        }
        else // Write Mania Mode Colours
        {
            // Normal Colours
            WriteColour(addr, 240, 140, 24); // NOTE: ADDR, R, G, B
            WriteColour(addr, 96 , 192, 160);
            WriteColour(addr, 240,  80, 48);
            WriteColour(addr, 64 , 96 , 176);

            // Test Colours
            //WriteColour(addr, 246, 141, 21);
            //WriteColour(addr, 51 , 48 , 216);
            //WriteColour(addr, 132, 32 , 34);
            //WriteColour(addr, 67 , 97 , 182);
        }
    }

    __declspec(dllexport) void Init(const char *path)
    {
        // Writes our hook into the function that handles what colours to use 
        WriteJump((void*)0x011B5A00, SetupTitleCardColours);
    }

    __declspec(dllexport) ModInfo ManiaModInfo = { ModLoaderVer, GameVer };

}


