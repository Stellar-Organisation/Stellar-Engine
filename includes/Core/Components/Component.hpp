#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_
#include <type_traits>

namespace Engine {
    struct Component
    {
        public:
            Component() = default;
            virtual ~Component() = default;

            Component(const Component &) = default;
            Component &operator=(const Component &) = default;

            Component(Component &&) noexcept = default;
            Component &operator=(Component &&) noexcept = default;
    };

    template<typename T>
    concept ComponentConcept = std::is_base_of_v<Component, T>;
} // namespace Engine
#endif /* !COMPONENT_HPP_ */
