#pragma once

#include <unordered_map>
#include <functional>
#include <variant>
#include <optional>
#include <tuple>
#include <utility>

#include <mplex/tuple/apply.hpp>
#include <mplex/tuple/push_back.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace EventManagement
{
    namespace Detail
    {
        template <std::size_t Size, typename ElementType>
        struct TupleMaker
        {
            using type = typename mplex::push_back <typename TupleMaker <Size-1, ElementType>::type, ElementType>::type;
        };

        template <typename ElementType>
        struct TupleMaker <0, ElementType>
        {
            using type = std::tuple<>;
        };

        template <typename T>
        struct FunctionTraits : public FunctionTraits<decltype(&T::operator())>
        {};

        template <typename ClassType, typename ReturnType, typename... Args>
        struct FunctionTraits<ReturnType(ClassType::*)(Args...) const>
        // we specialize for pointers to member function
        {
            using result_type = ReturnType;
            using arg_tuple = std::tuple<Args...>;
            static constexpr auto arity = sizeof...(Args);
        };

        template <class F, std::size_t ... Is, class T>
        auto LambdaToFuncImpl(F f, std::index_sequence<Is...>, T) {
            return std::function<typename T::result_type(std::tuple_element_t<Is, typename T::arg_tuple>...)>(f);
        }

        template <class F>
        auto LambdaToFunc(F f) {
            using traits = FunctionTraits<F>;
            return LambdaToFuncImpl(f, std::make_index_sequence<traits::arity>{}, traits{});
        }
    }

    /**
     *  A class that can store Functors with a given maximum amount of parameters and parameter types.
     *  These functors can then be called later as part of an observer or event system.
     *
     *  @param MaxParams The maximum amount of parameters a functor can possess.
     *  @param ParamTypes Possible parameter types.
     */
    template <typename EnumType, std::size_t MaxParams, typename... ParamTypes>
    class EventManager
    {
    public:
        using enum_type = EnumType;

        using parameter_type = std::variant <
            ParamTypes...
        >;

        using parameter_list_type = typename Detail::TupleMaker <MaxParams, parameter_type>::type;

    private:
        template <typename T>
        struct functionFactoryHelper
        {
        };
        template <std::size_t... indice>
        struct functionFactoryHelper <std::index_sequence<indice...>>
        {
            template <typename... T>
            static constexpr std::function <void(parameter_list_type const&)> convert(std::function <void(T const&...)>&& func)
            {
                return [func](parameter_list_type const& list) -> void
                {
                    return func(
                        std::get<T>(std::get<indice>(list))...
                    );
                };
            }
        };
        template <typename T>
        struct functionCallHelper
        {
        };
        template <std::size_t... indice>
        struct functionCallHelper <std::index_sequence<indice...>>
        {
            template <typename... List>
            static constexpr void fillParameters(parameter_list_type& tup, List... params)
            {
                ((std::get <indice>(tup) = params), ...);
            }
        };

        template <typename... T>
        std::function <void(parameter_list_type const&)> makeFunction(std::function <void(T const&...)>&& func)
        {
            return functionFactoryHelper <std::make_index_sequence <sizeof...(T)>>::template convert<T...>(std::move(func));
        }

    public:
        /**
         *  Adds a functor to the list.
         */
        template <typename... ParameterType>
        void add(enum_type type, std::function <void(ParameterType const&...)> const& cb)
        {
            unnamedEvents[type].push_back(makeFunction(std::move(cb)));
        }

        /**
         *  Adds a named functor to the list, useful to avoid accidental duplicates.
         */
        template <typename... ParameterType>
        void add(enum_type type, std::string const& name, std::function <void(ParameterType const&...)> const& cb)
        {
            namedEvents[type][name] = std::move(makeFunction(cb));
        }

        /**
         *  Adds a functor to the list.
         */
        template <typename LambdaT, typename... ParameterType>
        void add(enum_type type, LambdaT const& cb)
        {
            unnamedEvents[type].push_back(makeFunction(Detail::LambdaToFunc(cb)));
        }

        /**
         *  Adds a named functor to the list, useful to avoid accidental duplicates.
         */
        template <typename LambdaT, typename... ParameterType>
        void add(enum_type type, std::string const& name, LambdaT const& cb)
        {
            namedEvents[type][name] = std::move(makeFunction(Detail::LambdaToFunc(cb)));
        }

        void clear()
        {
            namedEvents.clear();
            unnamedEvents.clear();
        }

        template <typename... Params>
        void call(enum_type type, Params&&... params) const
        {
            auto parms = parameter_list_type{};
            functionCallHelper<std::make_index_sequence<sizeof...(params)>>::template fillParameters <Params...>(parms, params...);

            if (auto iter = namedEvents.find(type); iter != std::end(namedEvents))
                for ([[maybe_unused]] auto const& [dummy, func] : iter->second)
                    func(parms);

            if (auto iter = unnamedEvents.find(type); iter != std::end(unnamedEvents))
                for (auto& func : iter->second)
                    func(parms);
        }

        template <typename... Params>
        void callNamed(enum_type type, std::string const& name, Params&&... params) const
        {
            auto parms = parameter_list_type{};
            functionCallHelper<std::make_index_sequence<sizeof...(params)>>::template fillParameters <Params...>(parms, params...);

            namedEvents.at(type).at(name)(std::move(parms));
        }

    private:
        std::unordered_map <enum_type, std::unordered_map <std::string, std::function <void(parameter_list_type const&)>>> namedEvents;
        std::unordered_map <enum_type, std::vector <std::function <void(parameter_list_type const&)>>> unnamedEvents;
    };
}
