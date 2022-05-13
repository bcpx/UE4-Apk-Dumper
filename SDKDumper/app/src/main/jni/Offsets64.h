//
// Created by Ascarre on 09-05-2022.
//

#ifndef SDKGENERATOR_OFFSETS64_H
#define SDKGENERATOR_OFFSETS64_H

bool isStringDump = false, isObjectsDump = false, isSdkDump = false, isSdkWDump = false, isNew = false, isPubgM = false;

string OutPutPathDirectory("/sdcard/DumpedGames/");

namespace Offsets {

    const char* ProcessName;
    const char* ModuleName = "libUE4.so";

    enum Offsets {
        PointerSize = 0x8,

        //Global
        GWorld = 0x120,
        GNames = 0x120,
        GUObjectArray = 0x120,

        //Pointers
        GWorldPointer = 0x120,
        GNamesPointer = 0x120,

        //UWorld - PubgM
        PersistentLevel = 0x30,
        AActors = 0xB0, //0x98 for other Games
        AActorsCount = AActors + PointerSize,

        //TNameEntryArray
        NameString = 0xC,

        //FNamePoolAllocator - UE 4.23+
        GNamesToFNamePool = 0x30,
        FNamePoolCurrentBlock = 0x8,
        FNamePoolBlocks = 0x10,
        FNamePoolCurrentByteCursor = 0xC,
        FNameStride = 0x2,
        FNameEntryString = 0x2,
        FNameEntryLenBit = 6,

        //UObjectArray
        TUObjectArray = 0x10,
        TUObjectArrayNumElements = 0xC,

        //UObjectArray - UE 4.23+
        TUObjectArrayNumElements423 = 0x14,

        //UObject
        InternalIndex = 0xC,
        ClassPrivate = 0x10,
        FNameIndex = 0x18,
        OuterPrivate = 0x20,

        //FUObject - Global Pointers
        FUObjectItemPadd = 0x0,
        FUObjectItemSize = 0x18,

        //UField
        UFieldNext = 0x28,

        //FField - UE 4.23+
        FFieldClass = 0x8,
        FFieldNext = 0x20,
        FFieldName = 0x28,

        //UStruct
        UStructSuperStruct = 0x30,
        UStructChildren = 0x38,
        UStructChildProperties = 0x44,//UE 4.23+

        //UFunction
        UFunctionFunctionFlags = 0x88,
        UFunctionFunc = 0xB0,

        //UProperty
        UPropertyElementSize = 0x34,
        UPropertyPropertyFlags = 0x38,
        UPropertyOffsetInternal = 0x44,

        //Class: UBoolProperty
        UBoolPropertyFieldSize = 0x70,
        UBoolPropertyByteOffset = 0x71,
        UBoolPropertyByteMask = 0x72,
        UBoolPropertyFieldMask = 0x73,

        //UObjectProperty
        UObjectPropertyPropertyClass = 0x70,

        //UClassProperty
        UClassPropertyMetaClass = 0x78,

        //UInterfaceProperty
        UInterfacePropertyInterfaceClass = 0x78,

        //UArrayProperty
        UArrayPropertyInnerProperty = 0x70,

        //UMapProperty
        UMapPropertyKeyProp = 0x70,
        UMapPropertyValueProp = 0x78,

        //USetProperty
        USetPropertyElementProp = 0x70,

        //UStructProperty
        UStructPropertyStruct = 0x70,
    };
}

#endif //SDKGENERATOR_OFFSETS64_H
