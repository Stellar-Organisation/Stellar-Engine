/*
**    _____ _       _ _                   ______             _                 *
**   / ____| |     | | |                 |  ____|           (_)                *
**  | (___ | |_ ___| | | __ _ _ __ ______| |__   _ __   __ _ _ _ __   ___      *
**   \___ \| __/ _ \ | |/ _` | '__|______|  __| | '_ \ / _` | | '_ \ / _ \     *
**   ____) | ||  __/ | | (_| | |         | |____| | | | (_| | | | | |  __/     *
**  |_____/ \__\___|_|_|\__,_|_|         |______|_| |_|\__, |_|_| |_|\___|     *
**                                                      __/ |                  *
**                                                     |___/                   *
**                                                                             *
*
** File: SparseArray.hpp                                                       *
** Project: Stellar-Engine                                                     *
** Created Date: We Feb 2024                                                   *
** Author: GlassAlo                                                            *
** Email: ofourpatat@gmail.com                                                 *
** -----                                                                       *
** Description: {Enter a description for the file}                             *
** -----                                                                       *
** Last Modified: Thu Feb 22 2024                                              *
** Modified By: GlassAlo                                                       *
** -----                                                                       *
** Copyright (c) 2024 Stellar-Organisation                                     *
** -----                                                                       *
** HISTORY:                                                                    *
** Date      	By	Comments                                                   *
** ----------	---	---------------------------------------------------------  *
*/

#ifndef SPARSEARRAY_HPP_
#define SPARSEARRAY_HPP_

#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include "Component.hpp"
#include "Exception.hpp"

namespace Engine::Core {
    DEFINE_EXCEPTION(SparseArrayException);
    DEFINE_EXCEPTION_FROM(SparseArrayExceptionOutOfRange, SparseArrayException);
    DEFINE_EXCEPTION_FROM(SparseArrayExceptionEmpty, SparseArrayException);

    class ISparseArray
    {
        public:
            ISparseArray() = default;
            virtual ~ISparseArray() = default;

            ISparseArray(const ISparseArray &other) = default;
            ISparseArray &operator=(const ISparseArray &other) = default;

            ISparseArray(ISparseArray &&other) noexcept = default;
            ISparseArray &operator=(ISparseArray &&other) noexcept = default;

            virtual void init(std::size_t aIndex) = 0;
            virtual void erase(std::size_t aIndex) = 0;
            virtual void clear() = 0;
    };

    /**
     * @brief SparseArray is a class that store a vector of optional of a given type
     * It represents a ONE component type, each index in the array represent the component of the entity at the same
     * index
     *
     * @tparam Component The type of the components to store
     */
    template<ComponentConcept Component>
    class SparseArray final : public ISparseArray
    {
        public:
            using compRef = Component &;
            using constCompRef = const Component &;
            using optComponent = std::optional<Component>;
            using optCompRef = optComponent &;
            using vectArray = std::vector<optComponent>;
            using vectIndex = typename vectArray::size_type;
            using iterator = typename vectArray::iterator;
            using constIterator = typename vectArray::const_iterator;

        private:
            vectArray _array;

        public:
#pragma region constructors / destructors
            SparseArray() = default;
            ~SparseArray() override = default;

            SparseArray(const SparseArray &other) = default;
            SparseArray &operator=(const SparseArray &other) = default;

            SparseArray(SparseArray &&other) noexcept = default;
            SparseArray &operator=(SparseArray &&other) noexcept = default;
#pragma endregion constructors / destructors

#pragma region operators

            /**
             * @brief Get the component at the given index
             * @throw SparseArrayExceptionOutOfRange if the index is out of range or if the index is empty
             * @throw SparseArrayExceptionEmpty if the component is empty
             * @param index The index to get
             * @return compRef The component at the given index
             */
            compRef operator[](vectIndex aIndex)
            {
                if (aIndex >= _array.size() || aIndex < 0) {
                    throw SparseArrayExceptionOutOfRange("index out of range: " + std::to_string(aIndex));
                }
                if (!_array[aIndex].has_value()) {
                    throw SparseArrayExceptionEmpty("index is empty: " + std::to_string(aIndex));
                }
                return _array[aIndex].value();
            }

            /**
             * @brief Get the component at the given index
             * @throw SparseArrayExceptionOutOfRange if the index is out of range or if the index is empty
             * @throw SparseArrayExceptionEmpty if the component is empty
             * @param index The index to get
             * @return compRef The component at the given index
             */
            constCompRef operator[](vectIndex aIndex) const
            {
                if (aIndex >= _array.size() || aIndex < 0) {
                    throw SparseArrayExceptionOutOfRange("index out of range: " + std::to_string(aIndex));
                }
                if (!_array[aIndex].has_value()) {
                    throw SparseArrayExceptionEmpty("index is empty: " + std::to_string(aIndex));
                }
                return _array[aIndex].value();
            }

#pragma endregion operators

#pragma region methods

            /**
             * @brief Get the component at the given index
             * @throw SparseArrayExceptionOutOfRange if the index is out of range or if the index is empty
             * @throw SparseArrayExceptionEmpty if the component is empty
             * @param index The index to get
             * @return compRef The component at the given index
             */
            compRef get(vectIndex aIndex)
            {
                if (aIndex >= _array.size()) {
                    throw SparseArrayExceptionOutOfRange("index out of range: " + std::to_string(aIndex));
                }
                if (!_array[aIndex].has_value()) {
                    throw SparseArrayExceptionEmpty("index is empty: " + std::to_string(aIndex));
                }
                return _array[aIndex].value();
            }

            /**
             * @brief Set the component at the given index
             * @throw SparseArrayExceptionOutOfRange if the index is out of range
             * @param index The index to set
             * @param value The value to set
             */
            void set(vectIndex aIndex, Component &&aValue)
            {
                if (aIndex >= _array.size()) {
                    throw SparseArrayExceptionOutOfRange("index out of range: " + std::to_string(aIndex));
                }
                _array[aIndex] = std::move(aValue);
            }

            /**
             * @brief Check if the component at the given index is set
             * @throw SparseArrayExceptionOutOfRange if the index is out of range
             * @param index The index to check
             * @return true if the component is set
             * @return false if the component is not set
             */
            bool has(vectIndex aIndex) const
            {
                if (aIndex >= _array.size() || aIndex < 0) {
                    throw SparseArrayExceptionOutOfRange("index out of range: " + std::to_string(aIndex));
                }
                return _array[aIndex].has_value();
            }

            /**
             * @brief Init the component at the given index, will resize the array if needed and set each value to
             * std::nullopt
             * @param index The index to set
             */
            void init(vectIndex aIndex) override
            {
                if (aIndex >= _array.size()) {
                    _array.resize(aIndex + 1, std::nullopt);
                }
            }

            /**
             * @brief Emplace the component at the given index, will resize the array if needed and set each value to
             * std::nullopt
             * @param index The index to set
             * @param args The arguments to emplace
             * @return compRef The component at the given index (should be the one inserted)
             */
            template<typename... Args>
            compRef emplace(vectIndex aIndex, Args &&...aArgs)
            {
                if (aIndex >= _array.size()) {
                    _array.resize(aIndex + 1);
                }
                _array[aIndex].emplace(Component(std::forward<Args>(aArgs)...));
                return _array[aIndex].value();
            }

            /**
             * @brief Erase the component at the given index, will change the value of the component to std::nullopt,
             * won't resize the array
             * @throw SparseArrayExceptionOutOfRange if the index is out of range
             * @param index The index to erase
             */
            void erase(vectIndex aIndex) override
            {
                if (aIndex >= _array.size()) {
                    throw SparseArrayExceptionOutOfRange("index out of range: " + std::to_string(aIndex));
                }
                _array[aIndex].reset();
            }

            /**
             * @brief Destroy all the components
             */
            void clear() override
            {
                _array.clear();
            }

#pragma endregion methods

#pragma region iterator

            iterator begin()
            {
                return _array.begin();
            }

            iterator end()
            {
                return _array.end();
            }

            constIterator begin() const
            {
                return _array.begin();
            }

            constIterator end() const
            {
                return _array.end();
            }

            constIterator cbegin() const
            {
                return _array.cbegin();
            }

            constIterator cend() const
            {
                return _array.cend();
            }

            vectIndex size() const
            {
                return _array.size();
            }

#pragma endregion iterator
    };
} // namespace Engine::Core

#endif /* !SPARSEARRAY_HPP_ */
