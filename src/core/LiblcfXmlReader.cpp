#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "reader_xml.h"

namespace {
bool IsSpace(char value)
{
    return value == ' ' || value == '\t' || value == '\r' || value == '\n' ||
           value == '\f' || value == '\v';
}

template <class T>
void ReadVectorTokens(std::vector<T>& output, const std::string& data)
{
    output.clear();
    std::string::size_type position = 0;
    while (position < data.size()) {
        while (position < data.size() && IsSpace(data[position]))
            ++position;
        if (position >= data.size())
            break;

        const std::string::size_type begin = position;
        while (position < data.size() && !IsSpace(data[position]))
            ++position;

        T value;
        XmlReader::Read<T>(value, data.substr(begin, position - begin));
        output.push_back(value);
    }
}
}

XmlReader::XmlReader(const std::string& input_filename) :
    filename(input_filename),
    stream(NULL),
    parser(NULL),
    nesting(0)
{
    Open();
}

XmlReader::~XmlReader()
{
    Close();
}

void XmlReader::Open()
{

}

void XmlReader::Close()
{
    stream = NULL;
    parser = NULL;
}

bool XmlReader::IsOk() const
{
    return stream != NULL && !ferror(stream) && parser != NULL;
}

void XmlReader::Error(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}

void XmlReader::Parse()
{

}

void XmlReader::SetHandler(XmlHandler* handler)
{
    if (handlers.empty())
        handlers.push_back(handler);
    else
        handlers.back() = handler;
}

void XmlReader::StartElement(const char* name, const char** atts)
{
    if (handlers.empty() || handlers.back() == NULL)
        return;
    XmlHandler* handler = handlers.back();
    handlers.push_back(handler);
    handlers.back()->StartElement(*this, name, atts);
    buffer.clear();
}

void XmlReader::CharacterData(const char* data, int length)
{
    if (data != NULL && length > 0)
        buffer.append(data, static_cast<std::string::size_type>(length));
}

void XmlReader::EndElement(const char* name)
{
    if (handlers.empty() || handlers.back() == NULL)
        return;

    XmlHandler* handler = handlers.back();
    handler->CharacterData(*this, buffer);
    handlers.pop_back();
    if (handlers.empty()) {
        delete handler;
        return;
    }
    if (handler != handlers.back())
        delete handler;
    if (handlers.back() != NULL)
        handlers.back()->EndElement(*this, name);
}

template <>
void XmlReader::Read<bool>(bool& value, const std::string& data)
{
    value = data == "T";
}

template <>
void XmlReader::Read<int>(int& value, const std::string& data)
{
    value = static_cast<int>(strtol(data.c_str(), NULL, 10));
}

template <>
void XmlReader::Read<uint8_t>(uint8_t& value, const std::string& data)
{
    value = static_cast<uint8_t>(strtol(data.c_str(), NULL, 10));
}

template <>
void XmlReader::Read<int16_t>(int16_t& value, const std::string& data)
{
    value = static_cast<int16_t>(strtol(data.c_str(), NULL, 10));
}

template <>
void XmlReader::Read<uint32_t>(uint32_t& value, const std::string& data)
{
    value = static_cast<uint32_t>(strtoul(data.c_str(), NULL, 10));
}

template <>
void XmlReader::Read<double>(double& value, const std::string& data)
{
    value = strtod(data.c_str(), NULL);
}

template <>
void XmlReader::Read<std::string>(std::string& value, const std::string& data)
{
    static const std::string prefix("\xee\x80");

    if (data.find(prefix) == std::string::npos) {
        value = data;
        return;
    }

    value.clear();
    for (std::string::size_type position = 0;;) {
        const std::string::size_type next = data.find(prefix, position);
        if (next > position)
            value.append(data, position, next - position);
        if (next == std::string::npos)
            return;
        position = next + 2;
        if (position >= data.size())
            return;
        value.append(1, static_cast<char>(data[position] - '\x80'));
        ++position;
    }
}

template <class T>
void XmlReader::ReadVector(std::vector<T>& value, const std::string& data)
{
    ReadVectorTokens<T>(value, data);
}

template <>
void XmlReader::Read<std::vector<int> >(std::vector<int>& value, const std::string& data)
{
    ReadVector<int>(value, data);
}

template <>
void XmlReader::Read<std::vector<bool> >(std::vector<bool>& value, const std::string& data)
{
    ReadVector<bool>(value, data);
}

template <>
void XmlReader::Read<std::vector<uint8_t> >(std::vector<uint8_t>& value, const std::string& data)
{
    ReadVector<uint8_t>(value, data);
}

template <>
void XmlReader::Read<std::vector<int16_t> >(std::vector<int16_t>& value, const std::string& data)
{
    ReadVector<int16_t>(value, data);
}

template <>
void XmlReader::Read<std::vector<uint32_t> >(std::vector<uint32_t>& value, const std::string& data)
{
    ReadVector<uint32_t>(value, data);
}

template <>
void XmlReader::Read<std::vector<double> >(std::vector<double>& value, const std::string& data)
{
    ReadVector<double>(value, data);
}
