#pragma once

#include <string>

bool StartsWith(const std::string& str, const std::string& pref) {
    if (pref.size() > str.size()) {
        return false;
    }
    for (size_t i = 0; i < pref.size(); i++) {
        if (str[i] != pref[i]) {
            return false;
        }
    }
    return true;
}

const std::string CurrentDate() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
    return buf;
}

int64_t ToInt(const std::string& s) {
    int64_t ans = 0;
    bool negate = false;
    for (size_t i = 0; i < s.size(); i++) {
        char c = s[i];
        if (c >= '0' && c <= '9') {
            ans = ans * 10LL + (c - '0');
        } else if (i == 0 && c == '-') {
            negate = true;
        } else {
            return 0;
        }
    }
    if (negate) {
        ans = -ans;
    }
    return ans;
}

std::string GetImageBytes(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    std::string bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string ans;
    ans.reserve(bytes.size() << 3);
    for (size_t i = 0; i < bytes.size(); i++) {
        for (int j = 7; j >= 0; j--) {
            ans.push_back(((bytes[i] >> j) & 1) + '0');
        }
    }
    return ans;
}
