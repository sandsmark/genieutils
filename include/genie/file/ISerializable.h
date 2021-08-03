/*
    genieutils - A library for reading and writing data files of genie
               engine games.
    Copyright (C) 2011 - 2013  Armin Preiml
    Copyright (C) 2013 - 2021  Mikko "Tapsa" P

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "genie/Types.h"

#include <iostream>

#include <array>
#include <vector>
#include <string.h>
#include <stdint.h>
#include <cassert>
#include <memory>

namespace genie {

//------------------------------------------------------------------------------
/// Generic base class for genie file serialization
//
class ISerializable
{

public:
    virtual ~ISerializable() = default;

    /// Set position to start reading the object from stream
    inline void setInitialReadPosition(std::streampos pos)
    {
        init_read_pos_ = pos;
    }

    /// Where in the input stream this object is starting at
    inline std::streampos getInitialReadPosition() const
    {
        return init_read_pos_;
    }

    /// Read object from istream.
    ///
    /// @param istr Input stream to read from
    void readObject(std::istream &istr);

    /// Write object to stream.
    ///
    /// @param ostr Output stream to write to
    void writeObject(std::ostream &ostr);

    /// Returns size in bytes.
    virtual size_t objectSize();

    /// Serialize this object as a subobject of another one.
    ///
    /// @param root The object to serialize from.
    void serializeSubObject(ISerializable *const root);

    /// Sets game version to assume when loading, and used when saving
    virtual inline void setGameVersion(GameVersion gv)
    {
        gameVersion_ = gv;
    }

    /// Game version that will be assumed when loading and saving
    inline GameVersion getGameVersion() const
    {
        return gameVersion_;
    }

    /// Updates the game version of all objects in @param vec
    template <typename T, typename V>
    static void updateGameVersion(GameVersion gv, V &vec)
    {
        for (T &it : vec) {
            ISerializable *item = dynamic_cast<ISerializable *>(&it);
            item->setGameVersion(gv);
        }
    }

    /// Needs access to get and set stream methods for (de)compressing.
    friend class Compressor;

    /// Versions used to read/write/paste.

    /// Internal genie version in the DAT file, values between 6 to 12
    static float dat_internal_ver;

    /// Scenario file version; "1.00" to "1.21"
    static std::string scn_ver;

    /// Internal genie PLR version 1.0 to 1.30
    static float scn_plr_data_ver;

    /// Internal scenario file version
    static float scn_internal_ver;

    /// Scenario trigger version 1.0 to 1.6
    static double scn_trigger_ver;

protected:
    /// Defines the current operation when serializing
    enum Operation {
        OP_INVALID = -1,
        OP_READ = 0,
        OP_WRITE = 1,
        OP_CALC_SIZE = 2
    };

    /// Updates the gv of all objects with the gv of this object.
    //
    template <typename T>
    void updateGameVersion(std::vector<T> &vec)
    {
        updateGameVersion<T>(getGameVersion(), vec);
    }
    template <typename T, size_t N>
    void updateGameVersion(std::array<T, N> &vec)
    {
        updateGameVersion<T>(getGameVersion(), vec);
    }

    //----------------------------------------------------------------------------
    /// Set operation to process
    ///
    /// @param op operation
    inline void setOperation(Operation op)
    {
        operation_ = op;
    }

    //----------------------------------------------------------------------------
    /// Get current operation
    ///
    /// @return operation
    inline Operation getOperation() const
    {
        return operation_;
    }

    //----------------------------------------------------------------------------
    /// Check if given operation is active.
    ///
    /// @param op operation to check
    inline bool isOperation(Operation op) const
    {
        assert(operation_ != Operation::OP_INVALID);

        if (operation_ == Operation::OP_READ) {
            if (istr_->eof()) {
                throw std::ios_base::failure("Premature end of input");
            }

            if (!istr_->good()) {
                throw std::ios_base::failure("Invalid input stream");
            }
        } else if (operation_ == Operation::OP_WRITE && !ostr_->good()) {
            if (!ostr_->good()) {
                throw std::runtime_error("Invalid output stream");
            }
        }

        return (op == operation_);
    }

    /// Sets the stream the data should be read from
    inline void setIStream(std::istream &istr)
    {
        istr_ = &istr;
    }

    /// Returns the current stream data is read from
    inline std::istream *getIStream()
    {
        return istr_;
    }

    /// Sets the stream data should be saved to
    inline void setOStream(std::ostream &ostr)
    {
        ostr_ = &ostr;
    }

    /// Returns the stream data is saved to
    inline std::ostream *getOStream()
    {
        return ostr_;
    }

    //----------------------------------------------------------------------------
    /// @return position of the istreams get pointer.
    //
    std::streampos tellg() const;

    //----------------------------------------------------------------------------
    /// Custom strnlen for mingw32.
    ///
    /// @param str char array to get length from.
    /// @param max_size max array length
    /// @return string length
    //
    static size_t strnlen(const char *str, size_t max_size);

    //----------------------------------------------------------------------------
    /// Derived classes need to implement this method. It will automatically be
    /// called on reading or writing an Object.
    //
    virtual void serializeObject() = 0;

    //----------------------------------------------------------------------------
    /// Reads a string from istr. Returns empty string if len parameter is 0.
    /// The string will be cut at the first \0.
    ///
    /// @param len Number of characters to read.
    //
    std::string readString(size_t len);

    //----------------------------------------------------------------------------
    /// Writes a string to ostr.
    ///
    /// @param str string to write
    /// @param len number of chars to write.
    //
    void writeString(const std::string &str, size_t len);

    //----------------------------------------------------------------------------
    /// Generic read method for basic data types.
    ///
    /// @return read data
    //
    template <typename T,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    T read()
    {
        if (istr_->eof()) {
            throw std::ios_base::failure("Premature end of input");
        }

        if (!istr_->good()) {
            throw std::ios_base::failure("Invalid input stream");
        }


        T ret = {};
        istr_->read(reinterpret_cast<char *>(&ret), sizeof(ret));

        return ret;
    }

    //----------------------------------------------------------------------------
    /// Generic write method for basic data types.
    ///
    /// @param data to write.
    //
    template <typename T,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    void write(const T &data)
    {
        if (!ostr_->good()) {
            throw std::runtime_error("Invalid output stream");
        }

        ostr_->write(reinterpret_cast<const char *>(&data), sizeof(T));
    }

    //----------------------------------------------------------------------------
    /// Generic read method for arrays. It allocates new space if pointer is 0.
    //
    template <typename T,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    void read(T **array, size_t len)
    {
        if (istr_->eof()) {
            throw std::ios_base::failure("Premature end of input");
        }

        if (!istr_->good()) {
            throw std::ios_base::failure("Invalid input stream");
        }

        if (*array == 0) {
            *array = new T[len];
        }

        if (!istr_->good()) {
            throw std::runtime_error("Invalid input stream");
        }

        istr_->read(reinterpret_cast<char *>(*array), sizeof(T) * len);
    }

    //----------------------------------------------------------------------------
    /// Generic read method for arrays.
    //
    template <typename T,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    void read(T *array, size_t len)
    {
        if (istr_->eof()) {
            throw std::ios_base::failure("Premature end of input");
        }

        if (!istr_->good()) {
            throw std::ios_base::failure("Invalid input stream");
        }

        istr_->read(reinterpret_cast<char *>(array), sizeof(T) * len);
    }

    //----------------------------------------------------------------------------
    /// Writes an array to file.
    //
    template <typename T,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    void write(const T *const *data, size_t len)
    {
        if (!ostr_->good()) {
            throw std::runtime_error("Invalid output stream");
        }

        ostr_->write(reinterpret_cast<const char *const>(*data), sizeof(T) * len);
    }

    /// Serializes a string with debug data.
    void serializeDebugString(std::string &str)
    {
        uint16_t size = 0x0A60;
        serialize<uint16_t>(size);
        serializeSize<uint16_t>(size, str.size());
        serialize(str, size);
    }

    //----------------------------------------------------------------------------
    /// Serializes a string with preceding size. Template argument is the data
    /// type of the size.
    ///
    /// @param str string to serialize
    /// @param cString if enabled, string ends with a \0 (default)
    //
    template <typename T>
    void serializeSizedString(std::string &str, bool cString = true)
    {
        T size{};

        serializeSize<T>(size, str, cString);
        serialize(str, size);
    }

    /// What abomination is this?
    template <typename T>
    void serializeForcedString(std::string &str)
    {
        assert(operation_ != Operation::OP_INVALID);

        T size{};

        if (isOperation(Operation::OP_WRITE)) {
            size = str.size() + 1;
        }

        serialize<T>(size);
        serialize(str, size);
    }

    /// Serializes an array of strings with forced size
    template <typename T, size_t N>
    void serializeSizedStrings(std::array<std::string, N> &vec, bool cString = true)
    {
        assert(operation_ != Operation::OP_INVALID);

        for (size_t i = 0; i < N; ++i) {
            serializeSizedString<T>(vec[i], cString);
        }
    }

    /// Serializes a vector of strings with forced size
    template <typename T>
    void serializeSizedStrings(std::vector<std::string> &vec, size_t size,
                               bool cString = true)
    {
        assert(operation_ != Operation::OP_INVALID);

        if (isOperation(Operation::OP_READ)) {
            vec.resize(size);
        }

        for (size_t i = 0; i < size; ++i) {
            serializeSizedString<T>(vec[i], cString);
        }
    }

    //----------------------------------------------------------------------------
    /// Serialize method for basic data types.
    /// Reads or writes data dependent on set operation.
    //
    template <typename T,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    void serialize(T &data)
    {
        assert(operation_ != Operation::OP_INVALID);

        switch (getOperation()) {
        case Operation::OP_WRITE:
            write<T>(data);
            break;

        case Operation::OP_READ:
            data = read<T>();
            break;

        case Operation::OP_CALC_SIZE:
            size_ += sizeof(T);
            break;

        case Operation::OP_INVALID:
            assert(operation_ != Operation::OP_INVALID);
            break;
        }
    }

    template <typename T, typename E,
              std::enable_if_t<std::is_enum<E>::value, int> = 0
              >
    void serialize(E &data)
    {
        assert(operation_ != Operation::OP_INVALID);
        static_assert(sizeof(T) == sizeof(E));

        switch (getOperation()) {
        case Operation::OP_WRITE:
            write<T>(T(data));
            break;

        case Operation::OP_READ:
            data = E(read<T>());
            break;

        case Operation::OP_CALC_SIZE:
            size_ += sizeof(E);
            break;

        case Operation::OP_INVALID:
            assert(operation_ != Operation::OP_INVALID);
            break;
        }
    }

    /// Serializes another ISerializable object
    template <typename T>
    void serialize(ISerializable &data)
    {
        assert(operation_ != Operation::OP_INVALID);

        data.serializeSubObject(this);

        if (isOperation(Operation::OP_CALC_SIZE)) {
            size_ += data.objectSize();
        }
    }

    /// Reads or writes an array of data dependent on Write_ flag.
    template <typename T>
    void serialize(T **data, size_t len)
    {

        switch (getOperation()) {
        case Operation::OP_WRITE:
            write<T>(data, len);
            break;

        case Operation::OP_READ:
            read<T>(data, len);
            break;

        case Operation::OP_CALC_SIZE:
            size_ += sizeof(T) * len;
            break;

        case Operation::OP_INVALID:
            assert(operation_ != Operation::OP_INVALID);
            break;
        }
    }

    /// Spezialization of std::strings.
    void serialize(std::string &str, size_t len)
    {
        assert(operation_ != Operation::OP_INVALID);

        if (len > 0) {
            switch (getOperation()) {
            case Operation::OP_WRITE:
                writeString(str, len);
                break;

            case Operation::OP_READ:
                str = readString(len);
                break;

            case Operation::OP_CALC_SIZE:
                size_ += sizeof(char) * len;
                break;

            case Operation::OP_INVALID:
                assert(operation_ != Operation::OP_INVALID);
                break;
            }
        }
    }

    /// std::array with a plain old data type (could probably use is_trivial, but not needed)
    template <typename T, std::size_t N,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    void serialize(std::array<T, N> &vec)
    {
        switch (getOperation()) {
        case Operation::OP_WRITE:
            if (!ostr_->good()) {
                throw std::runtime_error("Invalid output stream");
            }

            ostr_->write(reinterpret_cast<const char *const>(vec.data()), sizeof(T) *  N);

            break;

        case Operation::OP_READ:
            if (istr_->eof()) {
                throw std::ios_base::failure("Premature end of input");
            }

            if (!istr_->good()) {
                throw std::ios_base::failure("Invalid input stream");
            }

            istr_->read(reinterpret_cast<char *>(vec.data()), sizeof(T) * N);

            break;

        case Operation::OP_CALC_SIZE:
            size_ += vec.size() * sizeof(T);
            break;

        case Operation::OP_INVALID:
            assert(operation_ != Operation::OP_INVALID);
            break;
        }
    }

    /// Reads or writes array of complex data types to/from a vector dependent on operation.
    template <typename T,
              std::size_t N,
              std::enable_if_t<std::is_base_of<ISerializable, T>::value, int> = 0
              >
    void serialize(std::array<T, N> &vec)
    {
        assert(operation_ != Operation::OP_INVALID);

        for (T &item : vec) {
            item.serializeSubObject(this);

            if (isOperation(Operation::OP_CALC_SIZE)) {
                size_ += item.objectSize();
            }
        }
    }

    /// two dimensional std::array
    template <typename T, std::size_t N, std::size_t N2>
    void serialize(std::array<std::array<T, N2>, N> &vec)
    {
        for (std::array<T, N2> &item : vec) {
            serialize<T, N2>(item);
        }
    }

    /// two dimensional std::array with vectors
    template <typename T, std::size_t N>
    void serialize(std::array<std::vector<T>, N> &vec, size_t size)
    {
        for (std::vector<T> &item : vec) {
            serialize<T>(item, size);
        }
    }

    /// Reads or writes an array of data to/from a vector dependent on operation.
    template <typename T,
              std::enable_if_t<std::is_standard_layout<T>::value, int> = 0,
              std::enable_if_t<std::is_trivial<T>::value, int> = 0,
              std::enable_if_t<std::negation<std::is_base_of<ISerializable, T>>::value, int> = 0
              >
    void serialize(std::vector<T> &vec, size_t size)
    {
        switch (getOperation()) {
        case Operation::OP_WRITE:
            if (vec.size() != size) {
                std::cerr << "Warning!: vector size differs len!" << vec.size() << " " << size << std::endl;
            }

            if (!ostr_->good()) {
                throw std::runtime_error("Invalid output stream");
            }

            ostr_->write(reinterpret_cast<const char *const>(vec.data()), sizeof(T) * std::min(size, vec.size()));
            break;

        case Operation::OP_READ:
            if (istr_->eof()) {
                throw std::ios_base::failure("Premature end of input");
            }

            if (!istr_->good()) {
                throw std::ios_base::failure("Invalid input stream");
            }

            vec.resize(size);
            istr_->read(reinterpret_cast<char *>(vec.data()), sizeof(T) * size);

            break;

        case Operation::OP_CALC_SIZE:
            size_ += size * sizeof(T);
            break;

        case Operation::OP_INVALID:
            assert(operation_ != Operation::OP_INVALID);
            break;
        }
    }

    /// Reads or writes a vector of data to/from a vector of vectors dependent on operation.
    template <typename T>
    void serialize(std::vector<std::vector<T>> &vec, size_t size, size_t size2)
    {
        switch (getOperation()) {
        case Operation::OP_WRITE:
            if (vec.size() != size) {
                std::cerr << "Warning!: vector size differs len!" << vec.size() << " " << size << std::endl;
            }

            for (size_t i = 0; i < size; ++i) {
                for (typename std::vector<T>::iterator it = vec[i].begin(); it != vec[i].end(); ++it) {
                    write<T>(*it);
                }
            }

            break;

        case Operation::OP_READ:
            vec.resize(size);

            for (size_t i = 0; i < size; ++i) {
                vec[i].resize(size2);

                for (size_t j = 0; j < size2; ++j) {
                    vec[i][j] = read<T>();
                }
            }

            break;

        case Operation::OP_CALC_SIZE:
            size_ += size * size2 * sizeof(T);
            break;

        case Operation::OP_INVALID:
            assert(operation_ != Operation::OP_INVALID);
            break;
        }
    }

    /// Serializes a vector of objects that inherit from ISerializable.
    template <typename T,
              std::enable_if_t<std::is_base_of<ISerializable, T>::value, int> = 0
              >
    void serialize(std::vector<T> &vec, size_t size)
    {
        assert(operation_ != Operation::OP_INVALID);
        if (isOperation(Operation::OP_READ)) {
            vec.resize(size);
        } else if (isOperation(Operation::OP_WRITE)) {
            assert(vec.size() == size);

            if (vec.size() != size) {
                std::cerr << "Warning!: vector size differs size!" << vec.size() << " " << size << std::endl;
                vec.resize(size);
            }
        }

        for (T &item : vec) {
            item.serializeSubObject(this);

            if (isOperation(Operation::OP_CALC_SIZE)) {
                size_ += item.objectSize();
            }
        }
    }

    /// Serializes a vector of objects that inherit from ISerializable.
    template <typename T,
              std::enable_if_t<std::is_base_of<ISerializable, T>::value, int> = 0
              >
    void serialize(std::vector<std::shared_ptr<T>> &vec, size_t size)
    {
        assert(operation_ != Operation::OP_INVALID);

        if (isOperation(Operation::OP_WRITE) || isOperation(Operation::OP_CALC_SIZE)) {
            assert(vec.size() == size);

            if (vec.size() != size) {
                std::cerr << "Warning!: vector size differs size!" << vec.size() << " " << size << std::endl;
                vec.resize(size);
            }

            for (typename std::vector<std::shared_ptr<T>>::iterator it = vec.begin(); it != vec.end(); ++it) {
                assert(*it);
                if (!*it) {
                    *it = std::make_shared<T>();
                }

                (*it)->serializeSubObject(this);

                if (isOperation(Operation::OP_CALC_SIZE)) {
                    size_ += (*it)->objectSize();
                }
            }
        } else {
            vec.resize(size);

            for (size_t i = 0; i < size; ++i) {
                if (istr_->eof()) {
                    throw std::ios_base::failure("Premature end of input");
                }

                if (!istr_->good()) {
                    throw std::ios_base::failure("Invalid input stream");
                }

                if (!vec[i]) {
                    vec[i] = std::make_shared<T>();
                }
                vec[i]->serializeSubObject(this);
            }
        }
    }

    /// Serialize a vector size number with a unsigned value. If size differs, the number will be
    /// updated.
    template <typename T,
              std::enable_if_t<std::is_signed<T>::value, int> = 0
              >
    void serializeSize(T &data, size_t size)
    {
        serializeSizeInternal(data, size);

        if (data < 0) {
            throw std::ios_base::failure("Invalid size " + std::to_string(data));
        }
    }

    /// Serialize a vector size number with a unsigned value. If size differs, the number will be
    /// updated.
    template <typename T,
              std::enable_if_t<std::is_unsigned<T>::value, int> = 0
              >
    void serializeSize(T &data, size_t size)
    {
        serializeSizeInternal(data, size);
    }

    /// Spezialization of @ref serializeSize for strings with unsigned value
    ///
    /// @param data size to serialize
    /// @param str string to get size from
    /// @param c_str true if cstring (ending with \0).
    template <typename T,
              std::enable_if_t<std::is_unsigned<T>::value, int> = 0
              >
    void serializeSize(T &data, const std::string &str, bool cString = true)
    {
        serializeStringSizeInternal(data, str, cString);
    }

    /// Spezialization of @ref serializeSize for strings with signed value.
    ///
    /// @param data size to serialize
    /// @param str string to get size from
    /// @param c_str true if cstring (ending with \0).
    template <typename T,
              std::enable_if_t<std::is_signed<T>::value, int> = 0
              >
    void serializeSize(T &data, const std::string &str, bool cString = true)
    {
        serializeStringSizeInternal(data, str, cString);

        if (data < 0) {
            throw std::ios_base::failure("Invalid size " + std::to_string(data));
        }
    }

    /// Necessary for graphic objects. The pointer array contains entries with
    /// value 0. If a pointer is 0, a empty graphic object will be inserted into
    /// the vector.
    template <typename T,
              std::enable_if_t<std::is_base_of<ISerializable, T>::value, int> = 0
              >
    void serializeSubWithPointers(std::vector<T> &vec, size_t size,
                                  std::vector<int32_t> &pointers)
    {
        assert(operation_ != Operation::OP_INVALID);
        if (isOperation(Operation::OP_READ)) {
            vec.resize(size);
        }
        for (size_t i = 0; i < size; ++i) {
            if (!pointers[i]) {
                continue;
            }

            vec[i].serializeSubObject(this);

            if (isOperation(Operation::OP_CALC_SIZE)) {
                size_ += vec[i].objectSize();
            }
        }
    }

    /// Spezialization of serialize for std::pair.
    ///
    /// @param p pair
    /// @param only_first if true only the first element will be serialized
    template <typename T>
    void serializePair(std::pair<T, T> &p, bool only_first = false)
    {
        assert(operation_ != Operation::OP_INVALID);

        switch (getOperation()) {
        case Operation::OP_WRITE:
            write<T>(p.first);

            if (!only_first) {
                write<T>(p.second);
            }

            break;

        case Operation::OP_READ:
            p.first = read<T>();

            if (!only_first) {
                p.second = read<T>();
            }

            break;

        case Operation::OP_CALC_SIZE:
            size_ += sizeof(T);

            if (!only_first) {
                size_ += sizeof(T);
            }

            break;

        case Operation::OP_INVALID:
            assert(operation_ != Operation::OP_INVALID);
            break;
        }
    }

private:
    template <typename T>
    void serializeSizeInternal(T &data, size_t size)
    {
        assert(operation_ != Operation::OP_INVALID);

        if (isOperation(Operation::OP_WRITE)) {
            data = size;
        }

        serialize<T>(data);
    }
    template <typename T>
    void serializeStringSizeInternal(T &data, const std::string &str, bool cString)
    {
        assert(operation_ != Operation::OP_INVALID);

        // calculate new size
        if (isOperation(Operation::OP_WRITE)) {
            size_t size = str.size();

            if (cString && size != 0) {
                size++;    //counting \0
            }

            data = size;
        }

        serialize<T>(data);
    }

    std::istream *istr_ = nullptr;
    std::ostream *ostr_ = nullptr;

    std::streampos init_read_pos_ = 0;

    Operation operation_ = Operation::OP_INVALID;

    GameVersion gameVersion_ = GameVersion::GV_None;

    size_t size_ = 0;
};

//----------------------------------------------------------------------------
/// Copies data from src to dest, but also allocates memory for dest or
/// sets dest to 0 if src is 0.
/*/
template <typename T>
void arraycpy(T **dest, const T *src, size_t size)
{
  if (src == 0)
    *dest = 0;
  else
  {
    *dest = new T[size];
    memcpy(*dest, src, size * sizeof(T));
  }
}*/
} // namespace genie

