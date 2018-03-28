#ifndef __YMSocketData_H__
#define __YMSocketData_H__

#include <map>
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "msgpack.hpp"

#else
#include "msgpack.hpp"
#endif

#include "YMvalue.h"
#include "YMreader.h"
using namespace std;
class YMSocketData : public CSJson::Value
{
public:
    YMSocketData() {};


    YMSocketData(const std::string &value)
    {
        CSJson::Reader reader;		
        reader.parse(value, *this);
    };

    void parse(char buffer[], unsigned int iSize)
    {
        CSJson::Reader reader;
        if (iSize > 0)
        {
            msgpack::unpacked unpackBuff;
            msgpack::unpack(&unpackBuff, buffer, iSize);
            std::string jsonStr = unpackMsgData(&unpackBuff.get());
            //CCLog(jsonStr.c_str());
            reader.parse(jsonStr, *this);
        }
        else
        {
			reader.parse("{}", *this);
        }
        //CCLog(jsonStr.c_str());
    };


    void serializer(char *buffer, int *iSize)
    {
        msgpack::sbuffer packBuffer;
        msgpack::packer<msgpack::sbuffer> pk(&packBuffer);
        packMsgData(&pk, *this);
        *iSize = packBuffer.size();
        memcpy(buffer, packBuffer.data(), packBuffer.size());
    }

    std::string getJsonString()
    {
        return _getString(*this);
    }

private:
    std::string _getString(CSJson::Value root)
    {
        std::string str = "";
        char buf[100];
        unsigned int size = root.size();
        switch (root.type())
        {
        case CSJson::nullValue:
            str += "{}";
            break;
        case CSJson::intValue:
            sprintf(buf, "%ld", root.asLargestInt());
            str += buf;
            break;
        case CSJson::uintValue:
            sprintf(buf, "%lu", root.asLargestUInt());
            str += buf;
            break;
        case CSJson::realValue:
            sprintf(buf, "%f", root.asFloat());
            str += buf;
            break;
        case CSJson::stringValue:
            str += "\"" + root.asString() + "\"";
            break;
        case CSJson::booleanValue:
            if (root.asBool())
            {
                str += "true";
            }
            else
            {
                str += "false";
            }
            break;
        case CSJson::arrayValue:
            str += "[";
            for (unsigned int i = 0; i < size; i++)
            {
                str += _getString(root[i]);
                if (i < size - 1)
                {
                    str += ", ";
                }
            }
            str += "]";
            break;
        case CSJson::objectValue:
            str += "{";
            for (unsigned int i = 0; i < size; i++)
            {
                std::string key = root.getMemberNames()[i];
                CSJson::Value val = root[key];
                str += key + ":";
                str += _getString(val);
                if (i < size - 1)
                {
                    str += ", ";
                }
            }
            str += "}";
            break;
        default:
            break;
        }
        return str;
    }


    void packMsgData(msgpack::packer<msgpack::sbuffer> *pk, CSJson::Value root)
    {
        unsigned int size = root.size();
        string utf8Txt = "";
        switch (root.type())
        {
        case CSJson::nullValue:
			pk->pack_map(0);
            break;
        case CSJson::intValue:
            pk->pack_int(root.asInt());
            break;
        case CSJson::uintValue:
            pk->pack_unsigned_int(root.asUInt());
            break;
        case CSJson::realValue:
            pk->pack_float(root.asFloat());
            break;
        case CSJson::stringValue:
            utf8Txt = root.asString().c_str();
            if (utf8Txt.size() > 0 && root.asString().size() > 0)
            {
                pk->pack(utf8Txt);
            }
            else
            {
                pk->pack(root.asString());
            }
            break;
        case CSJson::booleanValue:
            if (root.asBool())
            {
                pk->pack_true();
            }
            else
            {
                pk->pack_false();
            }
            //pk->pack_int(root.asInt());
            break;
        case CSJson::arrayValue:
            pk->pack_array(size);
            for (unsigned int i = 0; i < size; i++)
            {
                packMsgData(pk, root[i]);
            }
            break;
        case CSJson::objectValue:
            pk->pack_map(size);
            for (unsigned int i = 0; i < size; i++)
            {
                std::string key = root.getMemberNames()[i];
                CSJson::Value val = root[key];
                pk->pack(key);
                packMsgData(pk, val);
            }
            break;
        default:
            break;
        }
    }

    std::string unpackMsgData(msgpack::object *obj)
    {
        std::string str = "";
        switch (obj->type)
        {
        case msgpack::type::NIL:
        case msgpack::type::BOOLEAN:
        case msgpack::type::POSITIVE_INTEGER:
        case msgpack::type::NEGATIVE_INTEGER:
        case msgpack::type::DOUBLE:
        case msgpack::type::RAW:
            str += getMsgPackValue(obj);
            break;
        case msgpack::type::ARRAY:
            str += "[";
            if (obj->via.array.size != 0)
            {
                msgpack::object* p(obj->via.array.ptr);
                if (p->type == msgpack::type::MAP || p->type == msgpack::type::ARRAY)
                {
                    str += unpackMsgData(p);
                }
                else
                {
                    str += getMsgPackValue(p);
                }
                ++p;
                for (msgpack::object * const pend(obj->via.array.ptr + obj->via.array.size); p < pend; ++p)
                {
                    str += ",";
                    if (p->type == msgpack::type::MAP || p->type == msgpack::type::ARRAY)
                    {
                        str += unpackMsgData(p);
                    }
                    else
                    {
                        str += getMsgPackValue(p);
                    }
                }
            }
            str += "]";
            break;

        case msgpack::type::MAP:
            str += "{";
            if (obj->via.map.size != 0)
            {
                msgpack::object_kv* p(obj->via.map.ptr);
                str += getMsgPackValue(&(p->key));
                str += ":";
                if (p->val.type == msgpack::type::MAP || p->val.type == msgpack::type::ARRAY)
                {
                    str += unpackMsgData(&(p->val));
                }
                else
                {
                    str += getMsgPackValue(&(p->val));
                }
                ++p;
                for (msgpack::object_kv * const pend(obj->via.map.ptr + obj->via.map.size);
                        p < pend; ++p)
                {
                    str += ", ";
                    str += getMsgPackValue(&(p->key));
                    str += ":";
                    if (p->val.type == msgpack::type::MAP || p->val.type == msgpack::type::ARRAY)
                    {
                        str += unpackMsgData(&(p->val));
                    }
                    else
                    {
                        str += getMsgPackValue(&(p->val));
                    }
                }
            }
            str += "}";
            break;
        default:
            // FIXME
            //s << "#<UNKNOWN " << (uint16_t)obj.type << ">";
            break;
        }
        return str;
    }

    std::string getMsgPackValue(msgpack::object *obj)
    {
        std::string str = "";
        char buf[16];
        switch (obj->type)
        {
        case msgpack::type::NIL:
            str = "{}";
            break;
        case msgpack::type::BOOLEAN:
            str = (obj->via.boolean ? "true" : "false");
            break;
        case msgpack::type::POSITIVE_INTEGER:
            sprintf(buf, "%lu", obj->via.u64);
            str = buf;
            break;
        case msgpack::type::NEGATIVE_INTEGER:
            sprintf(buf, "%8d", obj->via.i64);
            str = buf;
            break;
        case msgpack::type::DOUBLE:
            sprintf(buf, "%f", obj->via.dec);
            str = buf;
            break;
        case msgpack::type::RAW:
            char chars[20480];
            memset(chars, 0, 20480);
            memcpy(chars, obj->via.raw.ptr, obj->via.raw.size);
            str = "\"";
            str += chars;
            str += "\"";
            break;
        }
        return str;
    }
};

#endif
