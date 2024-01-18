#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_
#include <type_traits>

namespace Engine {
    struct Component
    {
        public:
            virtual ~Component() = default;
    };

    template<typename T>
    concept ComponentConcept = std::is_base_of_v<Component, T>;
} // namespace Engine
#endif /* !COMPONENT_HPP_ */
