//
// Created by Ascarre on 09-05-2022.
//

#ifndef SDKGENERATOR_OFFSETS_H
#define SDKGENERATOR_OFFSETS_H

bool isFindGNames = false, isFindGUObject = false, isFindGWorld = false, isStringDump = false, isObjectsDump = false, isSdkDump = false, isSdkWDump = false, isNew = false, isPubgM = false;

string OutPutPathDirectory("/sdcard/DumpedGames/");

namespace Offsets {

    const char* ProcessName;
    const char* ModuleName = "libUE4.so";

    enum Offsets {
        PointerSize = 0x4,

        //Global
        GWorld = 0x7B2A7F4,
        GNames = 0x78292D8,
        GUObjectArray = 0x79B1F20,

        //Pointers
        GWorldPointer = 0x3C,
        GNamesPointer = 0x80,

        //UWorld
        PersistentLevel = 0x20,
        AActors = 0x70,
        AActorsCount = AActors + PointerSize,

        //TNameEntryArray
        NameString = 0x8,

        //FNamePoolAllocator - UE 4.23+
        GNamesToFNamePool = 0x30,
        FNamePoolCurrentBlock = 0x4,
        FNamePoolBlocks = 0xC,
        FNamePoolCurrentByteCursor = 0x8,
        FNameStride = 0x2,
        FNameEntryString = 0x2,
        FNameEntryLenBit = 6,

        //UObjectArray
        TUObjectArray = 0x10,
        TUObjectArrayNumElements = 0x8,

        //UObjectArray - UE 4.23+
        TUObjectArrayNumElements423 = 0x10,

        //UObject
        InternalIndex = 0x8,
        ClassPrivate = 0xC,
        FNameIndex = 0x10,
        OuterPrivate = 0x18,

        //FUObject - Global Pointers
        FUObjectItemPadd = 0x0,
        FUObjectItemSize = 0x10,

        //UField
        UFieldNext = 0x1C,

        //FField - UE 4.23+
        FFieldClass = 0x4,
        FFieldNext = 0x10,
        FFieldName = 0x14,

        //UStruct
        UStructSuperStruct = 0x20,
        UStructChildren = 0x24,
        UStructChildProperties = 0x28,//UE 4.23+

        //UFunction
        UFunctionFunctionFlags = 0x58,
        UFunctionFunc = 0x74,

        //UProperty
        UPropertyElementSize = 0x24,
        UPropertyPropertyFlags = 0x28,
        UPropertyOffsetInternal = 0x34,

        //Class: UBoolProperty
        UBoolPropertyFieldSize = 0x50,
        UBoolPropertyByteOffset = 0x51,
        UBoolPropertyByteMask = 0x52,
        UBoolPropertyFieldMask = 0x53,

        //UObjectProperty
        UObjectPropertyPropertyClass = 0x50,

        //UClassProperty
        UClassPropertyMetaClass = 0x54,

        //UInterfaceProperty
        UInterfacePropertyInterfaceClass = 0x54,

        //UArrayProperty
        UArrayPropertyInnerProperty = 0x50,

        //UMapProperty
        UMapPropertyKeyProp = 0x50,
        UMapPropertyValueProp = 0x54,

        //USetProperty
        USetPropertyElementProp = 0x50,

        //UStructProperty
        UStructPropertyStruct = 0x50,
    };
}


#endif //SDKGENERATOR_OFFSETS_H
