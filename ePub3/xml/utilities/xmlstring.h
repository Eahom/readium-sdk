//
//  xmlstring.h
//  ePub3
//
//  Created by Jim Dovey on 2013-09-23.
//  Copyright (c) 2012-2013 The Readium Foundation and contributors.
//  
//  The Readium SDK is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef __ePub3_xml_string_h__
#define __ePub3_xml_string_h__

#include <ePub3/xml/base.h>
#include <string>
#include <codecvt>

#if EPUB_USE(LIBXML2)
#include <libxml2/xmlstring.h>
#include <ePub3/utilities/utfstring.h>
#elif EPUB_USE(WIN_XML)
// Nothing
#else
#error No XML engine specified
#endif

EPUB3_XML_BEGIN_NAMESPACE

#if EPUB_USE(LIBXML2)
typedef ePub3::string string;
#elif EPUB_USE(WIN_XML)
class string : public std::wstring
{
public:
	using _Base = std::wstring;
	using UTF8Converter = std::wstring_convert<std::codecvt_utf8<value_type>>;

private:
	mutable std::string _utf8;

public:
	// grrr, VC doesn't support C++11 inheriting constructors yet
	//using std::wstring::basic_string;

	string() : _Base() {}
	string(const value_type* s) : _Base(s) {}
	string(const value_type* s, size_type sz) : _Base(s, sz) {}
	string(const char* s) : _Base(UTF8Converter().from_bytes(s)) {}
	string(const char* s, size_type len) : _Base(UTF8Converter().from_bytes(s, s+len)) {}
	string(const unsigned char* s) : _Base(UTF8Converter().from_bytes(reinterpret_cast<const char*>(s))) {}
	string(const unsigned char* s, size_type len) : _Base(UTF8Converter().from_bytes(reinterpret_cast<const char*>(s), reinterpret_cast<const char*>(s + len))) {}
	string(const _Base& s) : _Base(s) {}
	string(_Base&& s) : _Base(std::move(s)) {}
	string(size_type sz, value_type c) : _Base(sz, c) {}

	string(Platform::String^ pstr) : _Base(pstr->Data(), pstr->Length()) {}
	string(const Platform::StringReference& sref) : _Base(sref.Data(), sref.Length()) {}
	~string() { _Base::~_Base(); }

	::Platform::String^ win_str() const { return ref new ::Platform::String(data(), length()); }
	operator ::Platform::String^() const { return win_str(); }
	operator ::Platform::StringReference() const { return ::Platform::StringReference(data(), length()); }

	const unsigned char* utf8() const {
		_utf8 = UTF8Converter().to_bytes(*this);
		return reinterpret_cast<const unsigned char*>(_utf8.c_str());
	}

};
#endif

static const string XMLNamespace((const char*)"http://www.w3.org/XML/1998/namespace");
static const string XMLNSNamespace((const char*)"http://www.w3.org/2000/xmlns/");

EPUB3_XML_END_NAMESPACE

#endif /* __ePub3_xml_string_h__ */
