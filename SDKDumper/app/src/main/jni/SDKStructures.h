//
// Created by Ascarre on 09-05-2022.
//

#ifndef SDKGENERATOR_SDKSTRUCTURES_H
#define SDKGENERATOR_SDKSTRUCTURES_H

#include "Memory.h"

#if defined(__LP64__)
#include "Offsets64.h"

#else

#include "Offsets.h"

#endif

//signed
typedef signed short int int16;
typedef signed int int32;

//unsigned
typedef unsigned char uint8;
typedef unsigned short int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;
typedef unsigned short UTF16;

uint32 MAX_SIZE = 100;
uint32 GNameLimit = 170000;
uint32 classCount = 0;

struct WideStr {
    static int is_surrogate(UTF16 uc) {
        return (uc - 0xd800u) < 2048u;
    }

    static int is_high_surrogate(UTF16 uc) {
        return (uc & 0xfffffc00) == 0xd800;
    }

    static int is_low_surrogate(UTF16 uc) {
        return (uc & 0xfffffc00) == 0xdc00;
    }

    static wchar_t surrogate_to_utf32(UTF16 high, UTF16 low) {
        return (high << 10) + low - 0x35fdc00;
    }

    static wchar_t *w_str(uintptr_t str, size_t len) {
        wchar_t *output = new wchar_t[len + 1];

        UTF16 *source = ReadArray<UTF16>(str, len);

        for (int i = 0; i < len; i++) {
            const UTF16 uc = source[i];
            if (!is_surrogate(uc)) {
                output[i] = uc;
            } else {
                if (is_high_surrogate(uc) && is_low_surrogate(source[i]))
                    output[i] = surrogate_to_utf32(uc, source[i]);
                else
                    output[i] = L'?';
            }
        }

        free(source);

        output[len] = L'\0';
        return output;
    }

    static string getString(uintptr_t StrPtr, int StrLength) {
        wstring str = w_str(StrPtr, StrLength);

        string result(MAX_SIZE, '\0');

        wcstombs((char *) result.data(), str.c_str(), MAX_SIZE);

        return result;
    }
};

string GetFNameFromID(uintptr_t ModuleBase, uint32 index) {
    if (isNew){
        uint32 Block = index >> 16;
        uint16 Offset = index & 65535;

        uintptr_t FNamePool = ((ModuleBase, Offsets::GNames) + Offsets::GNamesToFNamePool);

        uintptr_t NamePoolChunk = Read<uintptr_t>(FNamePool + Offsets::FNamePoolBlocks + (Block * Offsets::PointerSize));
        uintptr_t FNameEntry = NamePoolChunk + (Offsets::FNameStride * Offset);

        int16 FNameEntryHeader = Read<int16>(FNameEntry);
        uintptr_t StrPtr = FNameEntry + Offsets::FNameEntryString;
        int StrLength = FNameEntryHeader >> Offsets::FNameEntryLenBit;

        ///Unicode Dumping Not Supported Yet
        if (StrLength > 0 && StrLength < 250) {
            bool wide = FNameEntryHeader & 1;
            if (wide) {
                return WideStr::getString(StrPtr, StrLength);
            } else {
                return ReadString2(StrPtr, StrLength);
            }
        } else {
            return "None";
        }
    } else {
        uintptr_t GNames;
        if (isPubgM){
            GNames = Read<uintptr_t>(Read<uintptr_t>(ModuleBase + Offsets::GNames) + Offsets::GNamesPointer);
        } else {
            GNames = Read<uintptr_t>(ModuleBase + Offsets::GNames);
        }
        uintptr_t FNameEntryArr = Read<uintptr_t>(GNames + ((index / 0x4000) * Offsets::PointerSize));
        uintptr_t FNameEntry = Read<uintptr_t>(FNameEntryArr + ((index % 0x4000) * Offsets::PointerSize));
        return ReadString(FNameEntry + Offsets::NameString, MAX_SIZE);
    }
}

struct UObject {
    static int32 getIndex(uintptr_t object) {
        return Read<int32>(object + Offsets::InternalIndex);
    }

    static uintptr_t getClass(uintptr_t object) {
        return Read<uintptr_t>(object + Offsets::ClassPrivate);
    }

    static uint32 getNameID(uintptr_t object) {
        return Read<uint32>(object + Offsets::FNameIndex);
    }

    static uintptr_t getOuter(uintptr_t object) {
        return Read<uintptr_t>(object + Offsets::OuterPrivate);
    }

    static string getName(uintptr_t ModuleBase, uintptr_t object) {
        return GetFNameFromID(ModuleBase, getNameID(object));
    }

    static string getClassName(uintptr_t ModuleBase, uintptr_t object) {
        return getName(ModuleBase, getClass(object));
    }

    static bool isValid(uintptr_t object) {
        return (object != 0 && getNameID(object) != 0 && getClass(object) != 0);
    }
};

struct UField {
    static uintptr_t getNext(uintptr_t field) {
        return Read<uintptr_t>(field + Offsets::UFieldNext);
    }
};

struct FField {
    static string getName(uintptr_t ModuleBase, uintptr_t fField) {
        return GetFNameFromID(ModuleBase, Read<uint32>(fField + Offsets::FFieldName));
    }

    static string getClassName(uintptr_t ModuleBase, uintptr_t fField) {
        return GetFNameFromID(ModuleBase, Read<uint32>(Read<uintptr_t>(fField + Offsets::FFieldClass)));
    }

    static uintptr_t getNext(uintptr_t fField) {
        return Read<uint32>(fField + Offsets::FFieldNext);
    }
};

struct UStruct {
    static uintptr_t getSuperClass(uintptr_t structz) {
        return Read<uintptr_t>(structz + Offsets::UStructSuperStruct);
    }

    static uintptr_t getChildren(uintptr_t structz) {
        return Read<uintptr_t>(structz + Offsets::UStructChildren);
    }

    static uintptr_t getChildProperties(uintptr_t structz) {
        return Read<uintptr_t>(structz + Offsets::UStructChildProperties);
    }

    static string getClassName(uintptr_t ModuleBase, uintptr_t clazz) {
        return UObject::getName(ModuleBase, clazz);
    }

    static string getClassPath(uintptr_t ModuleBase, uintptr_t object) {
        uintptr_t clazz = UObject::getClass(object);
        string classname = UObject::getName(ModuleBase, clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(ModuleBase, superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }

    static string getStructClassPath(uintptr_t ModuleBase, uintptr_t clazz) {
        string classname = UObject::getName(ModuleBase,clazz);

        uintptr_t superclass = getSuperClass(clazz);
        while (superclass) {
            classname += ".";
            classname += UObject::getName(ModuleBase, superclass);

            superclass = getSuperClass(superclass);
        }

        return classname;
    }
};

struct UFunction {
    static int32 getFunctionFlags(uintptr_t func) {
        return Read<int32>(func + Offsets::UFunctionFunctionFlags);
    }

    static uintptr_t getFunc(uintptr_t func) {
        return Read<uintptr_t>(func + Offsets::UFunctionFunc);
    }
};

struct UProperty {
    static int32 getElementSize(uintptr_t prop) {
        return Read<int32>(prop + Offsets::UPropertyElementSize);
    }

    static uint64 getPropertyFlags(uintptr_t prop) {
        return Read<uint64>(prop + Offsets::UPropertyPropertyFlags);
    }

    static int32 getOffset(uintptr_t prop) {
        return Read<int32>(prop + Offsets::UPropertyOffsetInternal);
    }
};

struct UBoolProperty {
    static uint8 getFieldSize(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyFieldSize);
    }

    static uint8 getByteOffset(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyByteOffset);
    }

    static uint8 getByteMask(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyByteMask);
    }

    static uint8 getFieldMask(uintptr_t prop) {
        return Read<uint8>(prop + Offsets::UBoolPropertyFieldMask);
    }
};

struct UObjectProperty {
    static uintptr_t getPropertyClass(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UObjectPropertyPropertyClass);
    }
};

struct UClassProperty {
    static uintptr_t getMetaClass(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UClassPropertyMetaClass);
    }
};

struct UInterfaceProperty {
    static uintptr_t getInterfaceClass(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UInterfacePropertyInterfaceClass);
    }
};

struct UArrayProperty {
    static uintptr_t getInner(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UArrayPropertyInnerProperty);
    }
};

struct UMapProperty {
    static uintptr_t getKeyProp(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UMapPropertyKeyProp);
    }

    static uintptr_t getValueProp(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UMapPropertyValueProp);
    }
};

struct USetProperty {
    static uintptr_t getElementProp(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::USetPropertyElementProp);
    }
};

struct UStructProperty {
    static uintptr_t getStruct(uintptr_t prop) {
        return Read<uintptr_t>(prop + Offsets::UStructPropertyStruct);
    }
};

#endif //SDKGENERATOR_SDKSTRUCTURES_H
