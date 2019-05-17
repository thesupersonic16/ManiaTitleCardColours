// ManiaTitleCardColours.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <ManiaModLoader.h>

// Set this to FALSE if you want this mod to use hardcoded colours
#define USECONFIGFILE TRUE
#include <memory>
#include <IniFile.hpp>

extern "C"
{
    // Ini Config file which will contain all the colours.
    static std::unique_ptr<IniFile> ConfigFile;

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
        auto addr = *(BYTE**)(baseAddress + 0x00AA7634);
        // Is in Encore Mode
        bool encore = *(addr + 0x37) == 5;
        // Address to the Background Colour within the Buffer 
        addr = (BYTE*)((*(int*)addr) + 0x0374);

        // Background Colour
        if (USECONFIGFILE)
        {
            auto group = ConfigFile->getGroup("Background");
            if (encore)
                WriteColour(addr, group->getInt("EncoreModeR"), group->getInt("EncoreModeG"), group->getInt("EncoreModeB"));
            else
                WriteColour(addr, group->getInt("ManiaModeR"), group->getInt("ManiaModeG"), group->getInt("ManiaModeB"));
            addr -= 4;
        }
        else
        {
            // NOTE: 0x00RRGGBB
            if (encore)
                *(int*)addr = 0x006BAE99; // Write Encore Mode Background Colour
            else
                *(int*)addr = 0x00F0C800; // Write Mania Mode Background Colour
        }
        
        // Jump back
        addr -= 0x10;
        
        if (USECONFIGFILE)
        {
            // Write Colours from Config file
            auto group = ConfigFile->getGroup(encore ? "EncoreMode" : "ManiaMode");
            WriteColour(addr, group->getInt("C1R"), group->getInt("C1G"), group->getInt("C1B"));
            WriteColour(addr, group->getInt("C2R"), group->getInt("C2G"), group->getInt("C2B"));
            WriteColour(addr, group->getInt("C3R"), group->getInt("C3G"), group->getInt("C3B"));
            WriteColour(addr, group->getInt("C4R"), group->getInt("C4G"), group->getInt("C4B"));
            return;
        }
        else
        {

            if (encore)
            {
                // Write Encore Mode Colours
                WriteColour(addr, 55, 81, 162); // NOTE: ADDR, R, G, B
                WriteColour(addr, 199, 82, 91);
                WriteColour(addr, 66, 143, 195);
                WriteColour(addr, 223, 177, 63);
            }
            else
            {
                // Write Mania Mode Colours
                WriteColour(addr, 240, 140, 24); // NOTE: ADDR, R, G, B
                WriteColour(addr, 96, 192, 160);
                WriteColour(addr, 240, 80, 48);
                WriteColour(addr, 64, 96, 176);
            }
        }
    }

    __declspec(dllexport) void Init(const char *path)
    {

        if (USECONFIGFILE)
        {
            // Set Working Directory to the mod folder
            char buffer[MAX_PATH];
            GetCurrentDirectoryA(MAX_PATH, buffer);
            SetCurrentDirectoryA(path);
            // Open Config file
            FILE *handle = fopen("ManiaTitleCardColours.ini", "r");
            if (!handle)
            {
                MessageBoxA(nullptr, "Failed to load config file for ManiaTitleCardColours!\n\nMake sure ManiaTitleCardColours.ini exists!", "ManiaTitleCardColours", MB_ICONERROR);
                return;
            }
            ConfigFile = std::unique_ptr<IniFile>(new IniFile(handle));
            fclose(handle);
            SetCurrentDirectoryA(buffer);
        }

        // Writes our hook into the function that handles what colours to use 
        WriteJump((void*)(baseAddress + 0x00015100), SetupTitleCardColours);
    }

    __declspec(dllexport) ModInfo ManiaModInfo = { ModLoaderVer, GameVer };

}


