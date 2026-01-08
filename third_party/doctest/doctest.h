// Minimal vendored doctest header (single-header).
// Source: doctest (https://github.com/doctest/doctest) - header-only test framework.
// Note: This file is included as-is for convenience in this small repro project.

#ifndef DOCTEST_LIBRARY_INCLUDED
#define DOCTEST_LIBRARY_INCLUDED

// This is a *tiny* subset shim to avoid pulling full upstream content here.
// It implements just enough for basic CHECK/REQUIRE/TEST_CASE in this repro.
// If you prefer full doctest, replace this file with the official single header.

#include <exception>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace doctest {
namespace detail {
    struct TestCase {
        const char* name;
        std::function<void()> func;
    };

    inline std::vector<TestCase>& registry() {
        static std::vector<TestCase> tests;
        return tests;
    }

    struct Register {
        Register(const char* name, std::function<void()> func) {
            registry().push_back({name, std::move(func)});
        }
    };

    inline int& failures() {
        static int f = 0;
        return f;
    }

    inline void fail(const char* expr, const char* file, int line, const std::string& msg) {
        ++failures();
        std::cerr << file << ":" << line << ": CHECK failed: " << expr;
        if (!msg.empty()) std::cerr << " (" << msg << ")";
        std::cerr << "\n";
    }

    template <class F>
    inline void check_no_throw(F&& f, const char* expr, const char* file, int line) {
        try {
            f();
        } catch (const std::exception& e) {
            fail(expr, file, line, std::string("threw: ") + e.what());
        } catch (...) {
            fail(expr, file, line, "threw non-std exception");
        }
    }

    template <class F>
    inline void require_no_throw(F&& f, const char* expr, const char* file, int line) {
        try {
            f();
        } catch (const std::exception& e) {
            fail(expr, file, line, std::string("threw: ") + e.what());
            throw;
        } catch (...) {
            fail(expr, file, line, "threw non-std exception");
            throw;
        }
    }
}

struct Context {
    int run() {
        for (auto& tc : detail::registry()) {
            try {
                tc.func();
            } catch (...) {
                // failures already counted via REQUIRE paths
            }
        }
        return detail::failures();
    }
};

} // namespace doctest

#define DOCTEST_CONCAT_IMPL(x, y) x##y
#define DOCTEST_CONCAT(x, y) DOCTEST_CONCAT_IMPL(x, y)

#define TEST_CASE(name) \
    static void DOCTEST_CONCAT(doctest_test_, __LINE__)(); \
    static doctest::detail::Register DOCTEST_CONCAT(doctest_reg_, __LINE__)(name, DOCTEST_CONCAT(doctest_test_, __LINE__)); \
    static void DOCTEST_CONCAT(doctest_test_, __LINE__)()

#define CHECK(expr) \
    do { \
        if (!(expr)) doctest::detail::fail(#expr, __FILE__, __LINE__, ""); \
    } while (0)

#define CHECK_EQ(a, b) \
    do { \
        auto _a = (a); \
        auto _b = (b); \
        if (!(_a == _b)) { \
            std::ostringstream _os; \
            _os << _a << " != " << _b; \
            doctest::detail::fail(#a " == " #b, __FILE__, __LINE__, _os.str()); \
        } \
    } while (0)

#define CHECK_NOTHROW(expr) \
    doctest::detail::check_no_throw([&](){ (void)(expr); }, #expr, __FILE__, __LINE__)

#define REQUIRE_NOTHROW(expr) \
    doctest::detail::require_no_throw([&](){ (void)(expr); }, #expr, __FILE__, __LINE__)

#endif // DOCTEST_LIBRARY_INCLUDED
