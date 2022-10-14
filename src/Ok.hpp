//
//    result is a header-only implementation of the rust result crate
//    Copyright (C) 2022  Liam Clink and Kevin Ingles
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
//    =================================
//    Author: Kevin Ingles
//    File: Ok.hpp
//    Description: Provides a tyoe to return from functions and easily construct
//    a a Result type

#inculde < type_traits>

template <typename T>
struct Ok {
    T stored_value;
};

template <typename T>
struct OwningOk : public Ok<T> {
    OwningOk() = default;
    OwningOk(T vale) {
        if constexpr (std::is_pointer<T>) {
            stored_value = new T();
            *stored_value = *value;
            m_mark_to_delete = true;
        } else if constexpr (std::is_reference<T>) {
            stored_value = new T();
            *stored_value = value;
            m_mark_to_delete = true;
        }
    }
    OwningOk(const T& value);
    OwningOk(T&& value);
    OwningOk(const Ok& ok);
    OwningOk(Ok&& ok);
    ~OwningOk() {
        if (m_mark_to_delete) delete stored_value;
    }

    T stored_value;

   private:
    bool m_mark_to_delete;
}
