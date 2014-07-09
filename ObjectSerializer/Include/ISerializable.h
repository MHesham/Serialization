#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <typeinfo>
#include <stdarg.h>
#include "ITraversable.h"
#include "SVector.h"

namespace Serialization
{
    class ObjectLayout : public ITraversable
    {
    public:
        template<typename TObject, typename... Args>
        ObjectLayout(const char* pTypeName, const TObject* pRootAddress, Args... childrenAddresses) :
            m_pTypeName(pTypeName),
            m_pCName(typeid(*pRootAddress).name()),
            m_typeSize(sizeof(TObject))
        {
            AddMemberAddress(sizeof...(Args), childrenAddresses);
        }

        template<typename... Args>
        ObjectLayout(const char* pTypeName, ObjectLayout parentLayout, Args... childrenAddresses) :
            m_pRootAddress(parentLayout.RootAddress()),
            m_childrenAdresses(parentLayout.ChildAddresses().begin(), parentLayout.ChildAddresses().end()),
            m_pTypeName(pTypeName),
            m_pCName(typeid(*pRootAddress).name()),
            m_typeSize(sizeof(TObject))
        {
            AddMemberAddress(sizeof...(Args), childrenAddresses);
        }

        char* RootAddress() const { return m_pRootAddress; }
        const std::vector<char*>& ChildAddresses() const { return m_childrenAdresses; }
        Iterator* GetIterator() { return new VectorIterator<char*>(&m_childrenAdresses); }
        virtual std::string TypeName()  const { return m_pTypeName; }
        virtual int TypeSize()  const { return m_typeSize; }
        std::string CName() const { return m_pCName; }

    private:
        void AddMemberAddress(size_t nAddresses, ...)
        {
            void* memberAddress;
            va_list argList;
            // The va_start macro is usually equivalent to:
            // pArgList = (void*) &p_memberAddress + sizeof (p_memberAddress) ;
            va_start (argList, nAddresses);
            ++nAddresses;
            while(--nAddresses)
            {
                memberAddress = va_arg(argList, void*);
                m_childrenAdresses.push_back(reinterpret_cast<char*>(memberAddress));
            }
            va_end (argList);
        }

        const char* m_pCName;
        const char* m_pTypeName;
        size_t m_typeSize;
        char* m_pRootAddress;
        std::vector<char*> m_childrenAdresses;
    };

    class ISerializable
    {
    public:
        virtual ~ISerializable() {}
        virtual ObjectLayout GetObjectLayout() const = 0;
    };

    extern size_t sm_lastSerializableObjID;
}

#define OBJECT_MEMBERS(C, ...) \
    public: \
    ObjectLayout GetObjectLayout() { return ObjectLayout(#C, this, __VA_ARGS__); }

#define OBJECT_MEMBERS_P(C, P, ...) \
    public: \
    ObjectLayout GetObjectLayout() { return ObjectLayout(#C, P::GetObjectLayout(), __VA_ARGS__); }

#endif // SERIALIZABLE_H
