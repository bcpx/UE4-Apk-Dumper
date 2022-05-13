//
// Created by Ascarre on 09-05-2022.
//

#ifndef SDKGENERATOR_DUMPER_H
#define SDKGENERATOR_DUMPER_H

#include <iostream>
#include <fstream>

#include "PropertyStructs.h"

void DumpBlocks(ofstream &gname, uint32 &count, uintptr_t FNamePool, uint32 blockId, uint32 blockSize) {
    uintptr_t Start = Read<uintptr_t>(FNamePool + Offsets::FNamePoolBlocks + (blockId * Offsets::PointerSize));
    uintptr_t End = Start + blockSize - Offsets::FNameEntryString;
    uint32 Block = blockId;
    uint16 Offset = 0;
    while (Start < End) {
        uintptr_t FNameEntry = Start;
        int16 FNameEntryHeader = Read<int16>(FNameEntry);
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryLenBit;
        if (StrLength) {
            bool wide = FNameEntryHeader & 1;

            ///Unicode Dumping Not Supported
            if (StrLength > 0) {
                if (StrLength < 250) {
                    string str;
                    uint32 key = (Block << 16 | Offset);
                    uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryString;

                    if (wide) {
                        str = WideStr::getString(StrPtr, StrLength);
                    } else {
                        str = ReadString2(StrPtr, StrLength);
                    }
                    gname << (wide ? "Wide" : "") << dec << "{" << StrLength << "} " << hex << "[" << key << "]: " << str << endl;
                    count++;
                }
            } else {
                StrLength = -StrLength;//Negative lengths are for Unicode Characters
            }

            //Next
            Offset += StrLength / Offsets::FNameStride;
            uint16 bytes = Offsets::FNameEntryString + StrLength * (wide ? sizeof(wchar_t) : sizeof(char));
            Start += (bytes + Offsets::FNameStride - 1u) & ~(Offsets::FNameStride - 1u);
        } else {
            break;
        }
    }
}

int32 GetObjectCount(uintptr_t ModuleBase) {
    if (isNew){
        return Read<int32>((ModuleBase + Offsets::GUObjectArray) + Offsets::TUObjectArray + Offsets::TUObjectArrayNumElements423);
    } else {
        return Read<int32>((ModuleBase + Offsets::GUObjectArray) + Offsets::TUObjectArray + Offsets::TUObjectArrayNumElements);
    }
}

uintptr_t GetUObjectFromID(uintptr_t ModuleBase, uint32 index) {
    if (isNew) {
        uintptr_t TUObjectArray = Read<uintptr_t>((ModuleBase + Offsets::GUObjectArray) + Offsets::TUObjectArray);
        uintptr_t Chunk = Read<uintptr_t>(TUObjectArray + ((index / 0x10000) * Offsets::PointerSize));
        return Read<uintptr_t>(Chunk + Offsets::FUObjectItemPadd + ((index % 0x10000) * Offsets::FUObjectItemSize));
    } else {
        uintptr_t FUObjectArray = (ModuleBase + Offsets::GUObjectArray);
        uintptr_t TUObjectArray = Read<uintptr_t>(FUObjectArray + Offsets::TUObjectArray);
        return Read<uintptr_t>(TUObjectArray + (index * Offsets::FUObjectItemSize));
    }
}

/********** Main Dumping Functions **********/
void DumpStrings(uintptr_t ModuleBase) {
    uint32 count = 0;
    ofstream gname(OutPutPathDirectory + Offsets::ProcessName + "/NamesDump.txt", ofstream::out);
    if (gname.is_open()) {
        gname << "//Dumper Made By Ascarre \n Join TlgChannel - https://t.me/ascarrehacks" << "\n" << endl;
        if (isPubgM){
            gname << "//Global Offsets and Pointers Used to Dump:" << endl;
            gname << "//GNames - 0x" << hex << Offsets::GNames << " + 0x" << hex << Offsets::GNamesPointer << "\n" << endl;
        } else {
            gname << "//Global Offsets Used to Dump:" << endl;
            gname << "//GNames - 0x" << hex << Offsets::GNames << "\n" << endl;
        }

        if (isNew){
            uintptr_t FNamePool = ((ModuleBase + Offsets::GNames) + Offsets::GNamesToFNamePool);
            uint32 BlockSize = Offsets::FNameStride * 65536;
            uint32 CurrentBlock = Read<uint32>(FNamePool + Offsets::FNamePoolCurrentBlock);
            uint32 CurrentByteCursor = Read<uint32>(FNamePool + Offsets::FNamePoolCurrentByteCursor);
            for (uint32 BlockIdx = 0; BlockIdx < CurrentBlock; ++BlockIdx) { //All Blocks Except Current
                DumpBlocks(gname, count, FNamePool, BlockIdx, BlockSize);
            }
            DumpBlocks(gname, count, FNamePool, CurrentBlock, CurrentByteCursor);//Last Block
        } else {
            for (uint32 i = 0; i < GNameLimit; i++) {
                string s = GetFNameFromID(ModuleBase, i);
                if (!s.empty()) {
                    gname << "[" << i << "]: " << s << endl;
                    count++;
                }
            }
        }
        gname.close();
    }
}

void DumpObjects(uintptr_t ModuleBase) {
    uint32 count = 0;
    ofstream obj(OutPutPathDirectory + Offsets::ProcessName + "/Objects.txt", ofstream::out);
    if (obj.is_open()) {

        obj << "//Dumper Made By Ascarre \n Join TlgChannel - https://t.me/ascarrehacks" << "\n" << endl;
        if (isPubgM){
            obj << "//Global Offsets and Pointers Used to Dump:" << endl;
            obj << "//GNames - 0x" << hex << Offsets::GNames << " + 0x" << hex << Offsets::GNamesPointer << endl;
            obj << "//GUObjectArray - 0x" << hex << Offsets::GUObjectArray << "\n" << endl;
        } else {
            obj << "//Global Offsets Used to Dump:" << endl;
            obj << "//GNames - 0x" << hex << Offsets::GNames  << endl;
            obj << "//GUObjectArray - 0x" << hex << Offsets::GUObjectArray << "\n" << endl;
        }

        int32 ocount = GetObjectCount(ModuleBase);
        if (ocount < 10 || ocount > 999999) {
            ocount = 300000;
        }
        for (int32 i = 0; i < ocount; i++) {
            uintptr_t UObjectArray = GetUObjectFromID(ModuleBase, i);
            if (UObject::isValid(UObjectArray)) {
                obj << setbase(16) << "[0x" << i << "]:" << endl;
                obj << "Name: " << UObject::getName(ModuleBase, UObjectArray).c_str() << endl;
                obj << "Class: " << UObject::getClassName(ModuleBase, UObjectArray).c_str() << endl;
                obj << "ObjectPtr: 0x" << setbase(16) << UObjectArray << endl;
                obj << "ClassPtr: 0x" << setbase(16) << UObject::getClass(UObjectArray) << endl;
                obj << endl;
                count++;
            }
        }
        obj.close();
    }
}

void DumpSDK(uintptr_t ModuleBase) {
    ofstream sdk(OutPutPathDirectory + Offsets::ProcessName + "/SDK.txt", ofstream::out);
    if (sdk.is_open()) {

        sdk << "//Dumper Made By Ascarre \n Join TlgChannel - https://t.me/ascarrehacks" << "\n" << endl;
        if (isPubgM){
            sdk << "//Global Offsets and Pointers Used to Dump:" << endl;
            sdk << "//GNames - 0x" << hex << Offsets::GNames << " + 0x" << hex << Offsets::GNamesPointer << endl;
            sdk << "//GUObjectArray - 0x" << hex << Offsets::GUObjectArray << "\n" << endl;
        } else {
            sdk << "//Global Offsets Used to Dump:" << endl;
            sdk << "//GNames - 0x" << hex << Offsets::GNames  << endl;
            sdk << "//GUObjectArray - 0x" << hex << Offsets::GUObjectArray << "\n" << endl;
        }

        int32 oCount = GetObjectCount(ModuleBase);
        if (oCount < 10 || oCount > 999999) {
            oCount = 300000;
        }
        for (int32 i = 0; i < oCount; i++) {
            uintptr_t UObjectArray = GetUObjectFromID(ModuleBase, i);
            if (UObject::isValid(UObjectArray)) {
                WriteStructures(ModuleBase, sdk, UObject::getClass(UObjectArray));
            }
        }
        sdk.close();
    }
}

void DumpSDKW(uintptr_t ModuleBase) {
    ofstream sdk(OutPutPathDirectory + Offsets::ProcessName + "/SDKW.txt", ofstream::out);
    if (sdk.is_open()) {

        sdk << "//Dumper Made By Ascarre \n Join TlgChannel - https://t.me/ascarrehacks" << "\n" << endl;
        if (isPubgM){
            sdk << "//Global Offsets and Pointers Used to Dump:" << endl;
            sdk << "//GWorld - 0x" << hex << Offsets::GWorld << " + 0x" << hex << Offsets::GWorldPointer << endl;
            sdk << "//GNames - 0x" << hex << Offsets::GNames << " + 0x" << hex << Offsets::GNamesPointer << "\n" << endl;
        } else {
            sdk << "//Global Offsets Used to Dump:" << endl;
            sdk << "//GWorld - 0x" << hex << Offsets::GWorld << endl;
            sdk << "//GNames - 0x" << hex << Offsets::GNames << "\n" << endl;
        }

        uintptr_t GWorld;
        if (isPubgM) {
            GWorld = Read<uintptr_t>(Read<uintptr_t>(ModuleBase + Offsets::GWorld) + Offsets::GWorldPointer);
        } else {
            GWorld = Read<uintptr_t>(ModuleBase + Offsets::GWorld);
        }
        if (UObject::isValid(GWorld)) {
            WriteStructures(ModuleBase, sdk, UObject::getClass(GWorld));

            uintptr_t PersistentLevel = Read<uintptr_t>(GWorld + Offsets::PersistentLevel);
            uintptr_t AActors = Read<uintptr_t>(PersistentLevel + Offsets::AActors);
            int AActorsCount = Read<int>(PersistentLevel + Offsets::AActorsCount);

            for (int i = 0; i < AActorsCount; i++) {
                uintptr_t Base = Read<uintptr_t>(AActors + (i * Offsets::PointerSize));
                if (UObject::isValid(Base)) {
                    WriteStructures(ModuleBase, sdk, UObject::getClass(Base));
                }
            }
        }
        sdk.close();
    }
}

#endif //SDKGENERATOR_DUMPER_H
