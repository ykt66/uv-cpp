﻿/*
   Copyright © 2017-2020, orcaer@yeah.net  All rights reserved.

   Author: orcaer@yeah.net

   Last modified: 2020-3-30

   Description: https://github.com/wlgq2/uv-cpp
*/

#include "../include/http/Request.h"

using namespace uv;
using namespace uv::http;

Request::Request()
    :Request(HttpVersion::Http1_1,Methon::Get)
{
}

Request::Request(HttpVersion version, Methon methon)
    :version_(version),
    methon_(methon)
{
}

void Request::swapBody(std::string& str)
{
    body_.swap(str);
}

void Request::swapBody(std::string&& str)
{
    swapBody(str);
}

std::string& Request::getBody()
{
    return body_;
}

void Request::appendHead(std::string& key, std::string& value)
{
    heads_[key] = value;
}

void Request::appendHead(std::string&& key, std::string&& value)
{
    heads_[key] = value;
}

std::string Request::getHead(std::string& key)
{
    auto it = heads_.find(key);
    if (it == heads_.end())
    {
        return "";
    }
    return it->second;
}

std::string Request::getHead(std::string&& key)
{
    return getHead(key);
}

void Request::appendUrlParam(std::string& key, std::string& value)
{
    urlParms_[key] = value;
}

void Request::appendUrlParam(std::string&& key, std::string&& value)
{
    urlParms_[key] = value;
}

std::string Request::getUrlParam(std::string& key)
{
    auto it = urlParms_.find(key);
    if (it != urlParms_.end())
    {
        return it->second;
    }
    return "";
}

std::string Request::getUrlParam(std::string&& key)
{
    return getUrlParam(key);
}

void Request::setVersion(HttpVersion version)
{
    version_ = version;
}

void Request::setMethon(Methon methon)
{
    methon_ = methon;
}

void Request::setPath(std::string&& path)
{
    setPath(path);
}

void Request::setPath(std::string& path)
{
    path_ = path;
}

std::string& Request::getPath()
{
    return path_;
}

HttpVersion Request::getVersion()
{
    return version_;
}

Methon Request::getMethon()
{
    return methon_;
}

int Request::pack(std::string& data)
{
    data.resize(1024);
    data.clear();
    data += MethonToStr(methon_);
    data += " ";
    packPathParam(data);
    data += " ";
    data += HttpVersionToStr(version_);
    data += Crlf;
    for (auto it = heads_.begin();it != heads_.end();it++)
    {
        data += it->first;
        data += ": ";
        data += it->second;
        data += Crlf;
    }
    data += Crlf;
    data += body_;
    return 0;
}

int uv::http::Request::pack(uv::PacketBuffer* pack)
{
    return 0;
}

int Request::unpack(std::string& data)
{
    auto bodyPos = data.find("\r\t\r\t");
    if (bodyPos == data.npos)
    {
        //解析失败，未找到消息正文
        return -1;
    }

    return 0;
}

std::string Request::MethonToStr(Methon methon)
{
    switch (methon)
    {
    case Get :
        return "GET";
    case Post :
        return "POST";
    case Head :
        return "HEAD";
    case Put:
        return "PUT";
    case Delete:
        return "DELETE";
    case Connect:
        return "CONNECT";
    case Options:
        return "OPTIONS";
    case Trace:
        return "TRACE";
    case Patch:
        return "PATCH";
    default:
        return "";
    }
}

void uv::http::Request::packPathParam(std::string& path)
{
    if (path_.empty() || path_[0] != '/')
    {
        path += "/";
    }
    path += path_;
    if (urlParms_.size() > 0)
    {
        path += "?";
        for (auto& ele : urlParms_)
        {
            path += ele.first;
            path += "=";
            path += ele.second;
            path += "&";
        }
        path.pop_back();
    }

}