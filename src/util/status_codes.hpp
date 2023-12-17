#pragma once
namespace util {
    enum class status_codes {
        SUCCESS = 0,
        ERROR,
        FILE_READ_SUCCESS,
        FILE_READ_FAIL,
        BAD_CMDL_ARGS,
    };
}