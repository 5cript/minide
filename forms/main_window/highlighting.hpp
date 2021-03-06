#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

namespace MinIDE
{
    struct HighlightingColorScheme
    {
        std::unordered_map <std::string, std::pair <nana::color, nana::color>> keywordToColor;
    };

    namespace DefaultHighlighting
    {
        const HighlightingColorScheme CppDefault {
            {
                {"core_lang_operators", {static_cast <nana::color_rgb>(0x569CD6), static_cast <nana::color_rgb>(0x303030)}},
                {"core_lang_other", {static_cast <nana::color_rgb>(0x569CD6), static_cast <nana::color_rgb>(0x303030)}},
                {"core_lang_casts", {static_cast <nana::color_rgb>(0x569CD6), static_cast <nana::color_rgb>(0x303030)}},
                {"core_lang_types", {static_cast <nana::color_rgb>(0x569CD6), static_cast <nana::color_rgb>(0x303030)}},
                {"core_lang_qualifiers", {static_cast <nana::color_rgb>(0x569CD6), static_cast <nana::color_rgb>(0x303030)}},
                {"core_lang_ctrl", {static_cast <nana::color_rgb>(0x569CD6), static_cast <nana::color_rgb>(0x303030)}},
                {"core_lang_typesys", {static_cast <nana::color_rgb>(0x569CD6), static_cast <nana::color_rgb>(0x303030)}},
                {"stl", {static_cast <nana::color_rgb>(0x008080), static_cast <nana::color_rgb>(0x303030)}}
            }
        };
    }

    struct Highlighting
    {
        HighlightingColorScheme colors;
    };

    template <typename NanaTextboxT>
    void applyCppScheme(NanaTextboxT& tbox)
    {
        tbox.set_keywords("core_lang_operators", true, true, {"and", "and_eq", "bitand", "bitor", "compl", "not", "not_eq", "or", "or_eq", "xor", "xor_eq"});
        tbox.set_keywords("core_lang_other", true, true, {"asm", "delete", "explicit", "extern", "false", "inline", "mutable", "new", "operator", "register", "sizeof", "this", "true"});
        tbox.set_keywords("core_lang_casts", true, true, {"const_cast", "dynamic_cast", "reinterpret_cast", "static_cast"});
        tbox.set_keywords("core_lang_types", true, true, {"auto", "bool", "char", "double", "float", "int", "long", "short", "signed", "unsigned", "void", "wchar_t"});
        tbox.set_keywords("core_lang_qualifiers", true, true, {"const", "volatile", "static", "constexpr", "virtual"});
        tbox.set_keywords("core_lang_ctrl", true, true, {"return", "break", "case", "catch", "continue", "default", "do", "else", "for", "goto", "if", "switch", "throw", "while"});
        tbox.set_keywords("core_lang_typesys", true, true, {"class", "enum", "private", "protected", "public", "struct", "struct", "template", "friend", "typedef", "using", "namespace", "typeid", "typename", "union"});

        tbox.set_keywords("stl", true, true, {
            "__gnu_cxx", "accumulate", "add_const", "add_cv", "add_lvalue_reference", "add_pointer", "add_reference", "add_rvalue_reference",
            "add_volatile", "adjacent_difference", "adjacent_find", "aligned_storage", "Alignment", "alignment_of", "all_of", "allocate_shared",
            "allocator", "allocator_base", "allocator_chunklist", "allocator_fixed_size", "allocator_newdel", "allocator_suballoc", "allocator_unbounded", "allocator_variable_size",
            "any_of", "array", "assign", "at", "atomic_bool", "atomic_char", "atomic_char16_t", "atomic_char32_t",
            "atomic_compare_exchange_strong", "atomic_compare_exchange_strong_explicit", "atomic_compare_exchange_weak", "atomic_compare_exchange_weak_explicit", "atomic_exchange", "atomic_exchange_explicit", "atomic_fetch_add", "atomic_fetch_and",
            "atomic_fetch_or", "atomic_fetch_sub", "atomic_fetch_xor", "atomic_int", "atomic_int_fast16_t", "atomic_int_fast32_t", "atomic_int_fast64_t", "atomic_int_fast8_t",
            "atomic_int_least16_t", "atomic_int_least32_t", "atomic_int_least64_t", "atomic_int_least8_t", "atomic_intmax_t", "atomic_intptr_t", "atomic_is_lock_free", "atomic_llong",
            "atomic_load", "atomic_load_explicit", "atomic_long", "atomic_ptrdiff_t", "atomic_schar", "atomic_short", "atomic_size_t", "atomic_ssize_t",
            "atomic_store", "atomic_store_explicit", "atomic_uchar", "atomic_uint", "atomic_uint_fast16_t", "atomic_uint_fast32_t", "atomic_uint_fast64_t", "atomic_uint_fast8_t",
            "atomic_uint_least16_t", "atomic_uint_least32_t", "atomic_uint_least64_t", "atomic_uint_least8_t", "atomic_uintmax_t", "atomic_uintptr_t", "atomic_ullong", "atomic_ulong",
            "atomic_ushort", "atomic_wchar_t", "auto_ptr", "back", "back_insert_iterator", "back_item", "bad_alloc", "bad_function_call",
            "bad_weak_ptr", "basic_filebuf", "basic_fstream", "basic_ifstream", "basic_ofstream", "basic_regex", "basic_streambuf", "basic_string",
            "begin", "bernoulli_distribution", "bidirectional_iterator_tag", "binary_function", "binary_negate", "binary_search", "bind", "bind1st",
            "bind2nd", "binder1st", "binder2nd", "binomial_distribution", "bit_and", "bit_or", "bit_xor", "bitset",
            "boost", "cache_chunklist", "cache_freelist", "cache_suballoc", "cauchy_distribution", "cbegin", "cend", "cerr",
            "char_traits", "checked_array_iterator", "checked_uninitialized_copy", "checked_uninitialized_fill_n", "chi_squared_distribution", "cin", "clear", "codecvt",
            "codecvt_base", "codecvt_byname", "codecvt_mode", "codecvt_utf16", "codecvt_utf8", "codecvt_utf8_utf16", "collate", "collate_byname",
            "common_type", "compare_exchange_strong", "compare_exchange_weak", "complex", "condition_variable", "conditional", "const_iterator", "const_mem_fun_ref_t",
            "const_mem_fun_t", "const_mem_fun1_ref_t", "const_mem_fun1_t", "const_pointer_cast", "const_reference", "const_reverse_iterator", "copy", "copy_backward",
            "copy_if", "copy_n", "count", "count_if", "cout", "crbegin", "cref", "crend",
            "ctype", "ctype_base", "ctype_byname", "decay", "declare_no_pointers", "declare_reachable", "declval", "default_delete",
            "default_random_engine", "deque", "difference_type", "discard_block", "discard_block_engine", "discrete_distribution", "divides", "domain_error",
            "dynamic_pointer_cast", "empty", "enable_if", "enable_shared_from_this", "end", "endl", "equal", "equal_range",
            "equal_to", "EqualityComparable", "erase", "error_category", "error_code", "error_condition", "exception", "exponential_distribution",
            "extent", "extreme_value_distribution", "fetch_add", "fetch_and", "fetch_or", "fetch_sub", "fetch_xor", "filebuf",
            "fill", "fill_n", "find", "find_end", "find_first_of", "find_first_not_of", "find_if", "find_if_not",
            "find_last_of", "find_last_not_of", "fisher_f_distribution", "float_denorm_style", "float_round_style", "for_each", "forward", "forward_iterator_tag",
            "forward_list", "freelist", "front", "front_insert_iterator", "front_item", "fstream", "function", "gamma_distribution",
            "generate", "generate_n", "generic_container", "generic_iterator", "generic_reverse_iterator", "generic_value", "geometric_distribution", "get_deleter",
            "get_pointer_safety", "get_temporary_buffer", "greater", "greater_equal", "has_nothrow_assign", "has_nothrow_constructor", "has_nothrow_copy", "has_nothrow_copy_assign",
            "has_nothrow_copy_constructor", "has_nothrow_default_constructor", "has_trivial_assign", "has_trivial_constructor", "has_trivial_copy", "has_trivial_copy_assign", "has_trivial_copy_constructor", "has_trivial_default_constructor",
            "has_trivial_destructor", "has_virtual_destructor", "hash", "hash_map", "hash_set", "ifstream", "includes", "independent_bits_engine",
            "initializer_list", "inner_product", "inplace_merge", "input_iterator_tag", "insert", "insert_iterator", "integral_constant", "invalid_argument",
            "ios_base", "iostream", "is_abstract", "is_arithmetic", "is_array", "is_base_of", "is_bind_expression", "is_class",
            "is_compound", "is_const", "is_constructible", "is_convertible", "is_empty", "is_enum", "is_error_code_enum", "is_error_condition_enum",
            "is_explicitly_convertible", "is_floating_point", "is_function", "is_fundamental", "is_heap", "is_heap_until", "is_integral", "is_literal_type",
            "is_lock_free", "is_lvalue_reference", "is_member_function_pointer", "is_member_object_pointer", "is_member_pointer", "is_nothrow_constructible", "is_object", "is_partitioned",
            "is_placeholder", "is_pod", "is_pointer", "is_polymorphic", "is_reference", "is_rvalue_reference", "is_same", "is_scalar",
            "is_signed", "is_sorted", "is_sorted_until", "is_standard_layout", "is_trivial", "is_union", "is_unsigned", "is_void",
            "is_volatile", "istream", "istream_iterator", "istreambuf_iterator", "iter_swap", "iterator", "iterator_traits", "knuth_b",
            "length_error", "less", "less_equal", "LessThanComparable", "lexicographical_compare", "linear_congruential", "linear_congruential_engine", "list",
            "locale", "logic_error", "logical_and", "logical_not", "logical_or", "lognormal_distribution", "lower_bound", "make_checked_array_iterator",
            "make_heap", "make_shared", "make_signed", "make_unsigned", "map", "match_results", "max", "max_element",
            "max_fixed_size", "max_none", "max_unbounded", "max_variable_size", "mem_fn", "mem_fun", "mem_fun_ref", "mem_fun_ref_t",
            "mem_fun_t", "mem_fun1_ref_t", "mem_fun1_t", "merge", "mersenne_twister", "mersenne_twister_engine", "messages", "messages_base",
            "messages_byname", "min", "min_element", "minmax", "minmax_element", "minstd_rand", "minstd_rand0", "minus",
            "mismatch", "modulus", "money_base", "money_get", "money_put", "moneypunct", "moneypunct_byname", "move",
            "move_backward", "move_iterator", "mt19937", "mt19937_64", "multimap", "multiplies", "multiset", "negate",
            "negative_binomial_distribution", "new_handler", "next_permutation", "none_of", "normal_distribution", "not_equal_to", "not1", "not2",
            "nothrow", "nothrow_t", "npos", "nth_element", "num_get", "num_put", "numeric_limits", "numpunct",
            "numpunct_byname", "ofstream", "ostream_iterator", "ostreambuf_iterator", "out_of_range", "output_iterator_tag", "overflow_error", "owner_less",
            "pair", "partial_sort", "partial_sort_copy", "partial_sum", "partition", "partition_copy", "partition_point", "piecewise_constant_distribution",
            "piecewise_linear_distribution", "plus", "pointer_safety", "pointer_to_binary_function", "pointer_to_unary_function", "poisson_distribution", "pop_back", "pop_front",
            "pop_heap", "prev_permutation", "priority_queue", "ptr_fun", "push_back", "push_front", "push_heap", "queue",
            "random_access_iterator_tag", "random_device", "random_shuffle", "range_error", "rank", "ranlux_base_01", "ranlux24", "ranlux24_base",
            "ranlux3", "ranlux3_01", "ranlux4", "ranlux4_01", "ranlux48", "ranlux48_base", "ranlux64_base_01", "ratio",
            "ratio_add", "ratio_divide", "ratio_multiply", "ratio_subtract", "raw_storage_iterator", "rbegin", "rdbuf", "ref",
            "reference", "reference_wrapper", "regex", "regex_constants", "regex_error", "regex_iterator", "regex_token_iterator", "regex_traits",
            "remove", "remove_all_extents", "remove_const", "remove_copy", "remove_copy_if", "remove_cv", "remove_extent", "remove_if",
            "remove_pointer", "remove_reference", "remove_volatile", "rend", "replace", "replace_copy", "replace_copy_if", "replace_if",
            "requires", "resize", "result_of", "return_temporary_buffer", "reverse", "reverse_copy", "reverse_iterator", "rotate",
            "rotate_copy", "rts_alloc", "runtime_error", "search", "search_n", "seed_seq", "set", "set_difference",
            "set_intersection", "set_new_handler", "set_symmetric_difference", "set_union", "setprecision", "setw", "shared_ptr", "shuffle_order_engine",
            "size", "size_type", "sort", "sort_heap", "stable_partition", "stable_sort", "stack", "static_pointer_cast",
            "std", "streambuf", "string", "stringstream", "student_t_distribution", "sub_match", "substr", "subtract_with_carry",
            "subtract_with_carry_01", "subtract_with_carry_engine", "swap", "swap_ranges", "sync_none", "sync_per_container", "sync_per_thread", "sync_shared",
            "system_error", "time_base", "time_get", "time_get_byname", "time_put", "time_put_byname", "to_array", "tr1",
            "transform", "tuple", "tuple_element", "tuple_size", "type_info", "unary_function", "unary_negate", "unchecked_uninitialized_copy",
            "unchecked_uninitialized_fill_n", "undeclare_no_pointers", "undeclare_reachable", "underflow_error", "uniform_int", "uniform_int_distribution", "uniform_real", "uniform_real_distribution",
            "uninitialized_copy", "uninitialized_copy_n", "uninitialized_fill", "uninitialized_fill_n", "unique", "unique_copy", "unique_ptr", "unordered_map",
            "unordered_multimap", "unordered_multiset", "unordered_set", "upper_bound", "valarray", "value_type", "variate_generator", "vector",
            "wcerr", "wcin", "wcout", "weak_ptr", "weibull_distribution", "wfilebuf", "wfstream", "wifstream",
            "wiostream", "wistream", "wofstream", "wregex", "xor_combine"
        });
    }
}
