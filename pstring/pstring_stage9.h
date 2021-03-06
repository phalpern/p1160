// pstring_stage9.h                                                   -*-C++-*-
#ifndef PSTRING_STAGE9_H_INCLUDED
#define PSTRING_STAGE9_H_INCLUDED

#include <memory_resource_p1160>
#include <cstddef>
#include <cstring>
#include <string>

class pstring {
    // This class is for demonstration purposes *only*.

public:
    using allocator_type = std::pmr::polymorphic_allocator_P0339R5<>;

    pstring(const char *cstr, allocator_type allocator = {});

    pstring(const pstring& other, allocator_type allocator = {});

    pstring(pstring&& other);
    pstring(pstring&& other, allocator_type allocator);

    ~pstring();

    pstring& operator=(const pstring& rhs);

    allocator_type get_allocator() const
    {
        return m_allocator_;
    }

    std::string str() const
        // For sanity checks only.
    {
        return { m_buffer_, m_length_ };
    }

private:
    allocator_type  m_allocator_;
    size_t          m_length_;
    char           *m_buffer_;
};

inline
pstring::pstring(const char *cstr, allocator_type allocator)
: m_allocator_(allocator)
, m_length_(std::strlen(cstr))
, m_buffer_(m_allocator_.allocate_object<char>(m_length_ + 1))

{
    std::strcpy(m_buffer_, cstr);
}

inline
pstring::pstring(const pstring& other, allocator_type allocator)
: m_allocator_(allocator)
, m_length_(other.m_length_)
, m_buffer_(m_allocator_.allocate_object<char>(m_length_ + 1))
{
    std::strcpy(m_buffer_, other.m_buffer_);
}

inline
pstring::pstring(pstring&& other)
: pstring(static_cast<pstring&&>(other), other.m_allocator_)
{
}

inline
pstring::pstring(pstring&& other, allocator_type allocator)
: m_allocator_(allocator)
, m_length_(other.m_length_)
{
    if (m_allocator_ == other.m_allocator_) {
        m_buffer_       = other.m_buffer_;
        other.m_length_ = static_cast<size_t>(-1);
        other.m_buffer_ = nullptr;
    }
    else {
        m_buffer_ = 0;
        std::strcpy(m_buffer_, other.m_buffer_);
    }
}

inline
pstring::~pstring()
{
    m_allocator_.deallocate_object(m_buffer_, m_length_ ? m_length_ + 1 : 0);
}

inline
pstring& pstring::operator=(const pstring& rhs)
{
    if (this == &rhs) {
        return *this;                                                 // RETURN
    }

    char *buff = m_allocator_.allocate_object<char>(rhs.m_length_ + 1);
    m_allocator_.deallocate_object(m_buffer_, m_length_ ? m_length_ + 1 : 0);
    m_buffer_ = buff;
    std::strcpy(m_buffer_, rhs.m_buffer_);
    m_length_ = rhs.m_length_;

    return *this;
}

#endif

// ----------------------------------------------------------------------------
// Copyright 2019 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
